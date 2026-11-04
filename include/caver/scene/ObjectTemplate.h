#ifndef CAVER_OBJECTTEMPLATE_H
#define CAVER_OBJECTTEMPLATE_H

#include "SceneObject.h"

namespace Caver {

namespace Proto {
class ObjectTemplate;
}

class ObjectLibrary;

class ObjectTemplate : public SceneObject {
public:
    ObjectTemplate();
    virtual ~ObjectTemplate();

    void Process();

    // IBindable slot family re-overridden on the template
    // (0x4F3B9C..0x4F4334). Three of the six bodies are empty in the
    // golden; GetBindings publishes exactly three editor bindings,
    // ids 100/101/102 - ObjectTemplate does NOT call SceneObject's
    // binding list.
    void UpdateBindingDependencies();                       // 0x4F3B9C — empty
    void GetBindings(std::vector<Binding>* bindings);       // 0x4F3BA4
    void GetEnumValuesForBindedProperty(int property,
                                        std::vector<BindingValue>* values); // 0x4F3DF4 — empty
    BindingValue ValueForBindedProperty(int property);      // 0x4F3DFC
    void SetValueForBindedProperty(int property,
                                   const BindingValue& value); // 0x4F409C
    void PerformBindedAction(int action);                   // 0x4F4334 — empty
    void AddComponent(const boost::intrusive_ptr<Component>& component); // 0x4F46C0

    void LoadFromProtobufMessage(const Proto::ObjectTemplate& message);
    void SaveToProtobufMessage(Proto::ObjectTemplate* message) const;
    void SaveToProtobufMessage(Proto::ObjectTemplate* message);

    static boost::intrusive_ptr<ObjectTemplate> TemplateFromObject(const std::string& identifier, SceneObject* object);

    ObjectLibrary* m_library; // +312
    float m_scaling; // +320
};

// boost::intrusive_ptr support (ADL, exact match for ObjectTemplate).
// Why these exist alongside the base-class overloads: without them,
// intrusive_ptr<ObjectTemplate> destruction resolves to the
// SceneObject* overload in scripting/ProgramState.h, whose release
// calls the destructor through a plain (potentially-throwing) function
// pointer. Clang then refuses to inline intrusive_ptr<ObjectTemplate>'s
// destructor at use sites and instead emits an outlined weak copy
// (proven on BushControllerComponent: the real header alone adds a
// .text._ZN5boost13intrusive_ptrIN5Caver14ObjectTemplateEED2Ev
// section and a call to it from HandleMessage, where the golden
// binary inlines the release: refcount decrement at +8, branch, and a
// vtable-slot-1 call, e.g. golden Bush HandleMessage @0x39F860 and
// @0x39FA5C, and no standalone intrusive_ptr<ObjectTemplate> or
// intrusive_ptr<SceneObject> destructor exists anywhere in the golden
// .so). The delete form below carries the destructor's known noexcept,
// so the release inlines exactly as in the golden (Bush .text is
// instruction-identical with these overloads versus the old local
// stub). Semantics are identical to both existing overloads: the
// counter lives at +8 (ReferenceCountedObject) and the final release
// deletes through the virtual destructor. The upstream alternative,
// for the scripting owner, is a one-token fix in ProgramState.h:
// noexcept-qualify that manual vtable call, which inlines every
// SceneObject-derived release too (verified in trial, not committed
// here: ProgramState.h is outside this batch's scope).
inline void intrusive_ptr_add_ref(ObjectTemplate* object)
{
    ++object->m_referenceCount;
}

inline void intrusive_ptr_release(ObjectTemplate* object)
{
    if (--object->m_referenceCount == 0)
        delete object;
}

} // namespace Caver

#endif
