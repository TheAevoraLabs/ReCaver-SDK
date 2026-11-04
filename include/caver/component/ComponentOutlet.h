// Recovered from libswordigo.so (1.4.13) evidence — see docs/CORE_HEADERS.md.
// Typed connection points owned by components. The vtable of
// ComponentOutlet<ModelComponent> (_ZTV at 0x618F28) has exactly five
// function slots and no destructor entries, fixing the base interface:
//
//   slot 0  Interface() const     — the interface token (see below)
//   slot 1  identifier() const
//   slot 2  setIdentifier(int)
//   slot 3  target(owner)         — covariant T* in the template
//   slot 4  ConnectTo(Component*)
//
// Interface tokens: every component class T has a static member
// T::Interface() (a 12-byte weak function) whose body returns its own
// address loaded through the GOT — the function's address IS the
// interface identifier for that class. ComponentOutlet<T>::Interface()
// returns that same address.
#ifndef CAVER_COMPONENTOUTLET_H
#define CAVER_COMPONENTOUTLET_H

#include "ReferenceCountedObject.h"
#include "Component.h"

namespace Caver {

class ComponentOutletBase {
public:
    ComponentOutletBase() : m_identifier(0) {}
    ~ComponentOutletBase() {}

    virtual long Interface() const = 0;                          // slot 0
    virtual int identifier() const = 0;                          // slot 1
    virtual void setIdentifier(int identifier) = 0;              // slot 2
    virtual Component* target(const Component* owner) = 0;       // slot 3
    virtual void ConnectTo(Component* component) = 0;            // slot 4

    // Non-virtual. If this outlet has an identifier, resolve the
    // component with that identifier on the owner's SceneObject and
    // ConnectTo() it.
    void Connect(const Component* component);

    int m_identifier;   // +8
};

template <typename T>
class ComponentOutlet : public ComponentOutletBase {
public:
    ComponentOutlet() : m_target(0) {}
    ~ComponentOutlet()
    {
        if (m_target)
            intrusive_ptr_release(m_target);
    }

    long Interface() const
    {
        return reinterpret_cast<long>(&T::Interface);
    }

    int identifier() const
    {
        return m_identifier;
    }

    void setIdentifier(int identifier)
    {
        if (m_identifier != identifier) {
            m_identifier = identifier;
            T* oldTarget = m_target;
            m_target = 0;
            if (oldTarget)
                intrusive_ptr_release(oldTarget);
        }
    }

    T* target(const Component* owner)
    {
        if (!m_target)
            Connect(owner);
        return m_target;
    }

    void ConnectTo(Component* component)
    {
        T* oldTarget = m_target;
        if (oldTarget != component) {
            if (component) {
                m_target = static_cast<T*>(component);
                intrusive_ptr_add_ref(m_target);
                if (oldTarget)
                    intrusive_ptr_release(oldTarget);
                m_identifier = component->m_identifier;
            } else {
                m_target = 0;
                if (oldTarget)
                    intrusive_ptr_release(oldTarget);
            }
        }
    }

    // +16 — manually reference counted: the golden stores the new
    // pointer before calling add_ref on it (see ConnectTo above), which
    // is not boost::intrusive_ptr's assignment order.
    T* m_target;
};

} // namespace Caver

#endif
