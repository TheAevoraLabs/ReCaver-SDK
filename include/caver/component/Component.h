// Recovered from libswordigo.so (1.4.13) evidence — see docs/CORE_HEADERS.md.
// The engine's component base class. Layout, vtable slot order and the
// base list are all recovered from the golden binary: _ZTV at 0x61BFC8
// (address point 0x61BFD8, secondary address points 0x61C0F0 for
// IBindable and 0x61C130 for IUpdateable), _ZTI is a __vmi_class_type_info
// naming ReferenceCountedObject (offset 0), IBindable (offset 16) and
// IUpdateable (offset 24) as bases. Object size is 112 bytes (the first
// derived member in HealthComponent sits at +112).
// There is NO out-of-line Component constructor anywhere in the binary:
// it was inlined at every construction site (derived ctors show the
// field initialisation sequence verbatim).
#ifndef CAVER_COMPONENT_H
#define CAVER_COMPONENT_H

#include <map>
#include <string>
#include <vector>
#include <boost/intrusive_ptr.hpp>
#include <boost/shared_ptr.hpp>

#include "ReferenceCountedObject.h"
#include "IBindable.h"
#include "IUpdateable.h"
#include "../core/Rectangle.h"
#include "../core/Binding.h"
#include "../core/BindingValue.h"

namespace Caver {

class SceneObject;
class ComponentManager;
class ComponentOutletBase;
class Program;
class ProgramState;
namespace Proto { class Component; }

// Value stored in Component::m_outlets, recovered byte-for-byte from
// BindOutlet's tree-node construction (node size 0x58: 32 bytes of tree
// header, the int key, then this struct). Name UNRECOVERED — it is not
// the ComponentOutlet<T> template in ComponentOutlet.h; renamed here to
// avoid the collision.
struct ComponentOutletBinding {
    int id;                         // +0 (node +40)
    std::string name;               // +8
    ComponentOutletBase* outlet;    // +32
    bool flag;                      // +40
};

class Component : public ReferenceCountedObject, public IBindable, public IUpdateable {
public:
    // Inline: the golden binary has no out-of-line Component ctor —
    // derived ctors (e.g. HealthComponent's, 120 bytes) show the base
    // initialisation sequence inlined verbatim: vptrs, refcount 0,
    // identifier 0, object 0, flag48 0, empty label, empty outlets map
    // (begin = &sentinel, root 0, size 0), field104 0.
    Component()
        : m_identifier(0),
          m_field36(0),
          m_object(0),
          m_flag48(false)
    {
    }
    Component(const Component&) = delete;
    Component& operator=(const Component&) = delete;
    virtual ~Component();

    // Primary vtable order (slots 2..34 of _ZTV, after the two dtor slots):
    virtual Component* Clone() const;                                        // slot 2 — base body returns 0 (UNRECOVERED: return type may be intrusive_ptr)
    virtual void InitWithComponent(const Component* other);                  // slot 3
    virtual void LoadFromProtobufMessage(const Proto::Component& message);   // slot 4
    virtual void SaveToProtobufMessage(Proto::Component* message) const;     // slot 5
    virtual bool ShouldSave();                                               // slot 6
    virtual bool Process(std::vector<boost::intrusive_ptr<Component>>* components); // slot 7
    virtual void Prepare();                                                  // slot 8
    virtual void FinishLoad();                                               // slot 9
    virtual void SetDefaultValues();                                         // slot 10
    virtual void AutoConnectOutlets();                                       // slot 11
    virtual void ComponentValuesChanged(Component* component);               // slot 12
    virtual void UpdateBindingDependencies();                                // slot 13 (IBindable)
    virtual void GetBindings(std::vector<Binding>* bindings);                // slot 14 (IBindable)
    virtual void GetEnumValuesForBindedProperty(int property, std::vector<BindingValue>* values); // slot 15 (IBindable)
    virtual BindingValue ValueForBindedProperty(int property);               // slot 16 (IBindable)
    virtual void SetValueForBindedProperty(int property, const BindingValue& value); // slot 17 (IBindable)
    virtual void PerformBindedAction(int action);                            // slot 18 (IBindable)
    virtual int componentCategories() const;                                 // slot 19 — base body returns 1 (return type UNRECOVERED)
    virtual bool ImplementsInterface(long interfaceId);                      // slot 20 — base body returns 0
    virtual void RegisterInterfaces(ComponentManager* manager);              // slot 21
    virtual bool ShouldBeUpdatedInEditor() const;                            // slot 22
    virtual bool RequiresUpdate() const;                                     // slot 23
    virtual bool UpdateWhenPaused() const;                                   // slot 24 (IUpdateable)
    virtual void Update(float deltaTime);                                    // slot 25 (IUpdateable)
    virtual bool HandleMessage(int message, void* data);                     // slot 26 — base body returns 0
    virtual bool HasBounds() const;                                          // slot 27
    virtual Rectangle localAABB() const;                                     // slot 28
    virtual float minDepth() const;                                          // slot 29 — golden returns the float in s0
    virtual float maxDepth() const;                                          // slot 30 — golden returns the float in s0
    virtual bool IsVisualComponent() const;                                  // slot 31
    virtual std::string className() const = 0;                               // slot 32 — pure (golden slot holds __cxa_pure_virtual)

    // Non-virtual (absent from the vtable):
    bool ShouldPrepare() const;
    void RegisterInterface(ComponentManager* manager, long interfaceId);
    void BindOutlet(int id, const std::string& name, ComponentOutletBase* outlet, bool flag);
    Binding BindingForOutlet(int id) const;
    void GetEnumValuesForBindedOutlet(long outlet, std::vector<BindingValue>* values) const;
    BindingValue ValueForBindedOutlet(const boost::intrusive_ptr<Component>& component) const;
    Component* ComponentWithIdentifier(int identifier) const;
    SceneObject* UpdateObjectBounds();
    boost::shared_ptr<ProgramState> NewProgramStateForProgram(const Program& program); // return type UNRECOVERED (16-byte sret)

    int m_identifier;                                   // +32
    int m_field36;                                      // +36 — proto field 2 (Load copies it, Save writes it when != 0); name UNRECOVERED
    SceneObject* m_object;                              // +40
    bool m_flag48;                                      // +48 — name UNRECOVERED
    std::string m_label;                                // +56 (proto "label")
    std::map<int, ComponentOutletBinding> m_outlets;    // +80 — sizeof(Component) == 104 (HealthComponent's first member sits at +104, proven by its dtor releasing that pointer)
};

} // namespace Caver

#ifdef _MSC_VER
// Win32-macro hygiene (same hazard as ProgramState.h): the Boost
// includes above pull in windows.h, which defines GetMessage and
// RegisterClass as function-like macros. Every component TU includes
// this header before protobuf's extension_set.h (via Scene.pb.h), so
// without these #undefs the GetExtension template's GetMessage call
// would be renamed to GetMessageA at parse time in every such TU,
// while extension_set.o defines plain GetMessage. The engine never
// calls the Win32 GetMessage/RegisterClass APIs, so undefining the
// macros here is safe for all includers.
#undef GetMessage
#undef RegisterClass
#endif

#endif
