// Recovered from libswordigo.so (1.4.13) evidence — see the combat
// slice board (SpellComponent) for per-function parity status.
// Spell-casting component: holds a caster SceneObject, a cast-point
// delta, and an "onCast" Program that Cast() executes with the caster
// pushed. sizeof 0xC0 (192) — proven from the ctor store map and the
// dtor teardown: the last member (Program at +136, sizeof 0x38) ends
// exactly at +192. (No Create()/Clone() exists in the golden for this
// class: the vtable's Clone slot keeps Component::Clone and there is
// no SpellComponent::Create symbol.)
#ifndef CAVER_SPELLCOMPONENT_H
#define CAVER_SPELLCOMPONENT_H

#include "Component.h"
#include "../core/Vector3.h"
#include "../scripting/Program.h"

namespace Caver {

class SpellComponent : public Component {
public:
    SpellComponent();
    // Defined in-class (as in the golden, where D1/D2/D0 and the
    // small accessors below are weak symbols — header-inline in the
    // original source).
    virtual ~SpellComponent() {}

    static long Interface()
    {
        // The interface token is this function's own address (the
        // golden body is a single GOT load of &SpellComponent::Interface).
        return reinterpret_cast<long>(&SpellComponent::Interface);
    }

    // NOT an override: the vtable slot 3 holds Component::InitWithComponent.
    // This is a non-virtual overload taking the narrowed type (its mangled
    // name uses EPKS0_ — the parameter is const SpellComponent*).
    void InitWithComponent(const SpellComponent* other);
    virtual void LoadFromProtobufMessage(const Proto::Component& message);
    virtual void SaveToProtobufMessage(Proto::Component* message) const;
    virtual bool ShouldSave() { return true; }
    virtual void Prepare();
    virtual void GetBindings(std::vector<Binding>* bindings);
    virtual void GetEnumValuesForBindedProperty(int property, std::vector<BindingValue>* values);
    virtual BindingValue ValueForBindedProperty(int property);
    virtual void SetValueForBindedProperty(int property, const BindingValue& value);
    virtual void PerformBindedAction(int action);                           // empty
    virtual bool ImplementsInterface(long interfaceId)
    {
        return interfaceId == reinterpret_cast<long>(&SpellComponent::Interface);
    }
    virtual void RegisterInterfaces(ComponentManager* manager)
    {
        RegisterInterface(manager, reinterpret_cast<long>(&SpellComponent::Interface));
        Component::RegisterInterfaces(manager);
    }
    virtual bool HandleMessage(int message, void* data);                    // false
    virtual std::string className() const
    {
        // Proven from the golden body: the SSO string it builds is
        // "Spell" (length byte 10 = 5 chars) — the same string as the
        // Lua library name, NOT "SpellComponent".
        return "Spell";
    }

    void RegisterLibrary();

    // Virtuals this class ADDS to the Component vtable, in golden slot
    // order (33..36).
    virtual void Cast();
    virtual bool ShouldChannel() { return false; }
    virtual void StartChanneling() {}
    virtual void StopChanneling() {}

    void SetCasterObject(const boost::intrusive_ptr<SceneObject>& object);
    boost::intrusive_ptr<SceneObject> CasterObject();
    Vector3 CastPoint();

    boost::intrusive_ptr<SceneObject> m_casterObject;   // +104 — the spell's caster, set via SetCasterObject
    Vector3 m_field112;         // +112 — cast delta: Cast() writes (caster position - owner position), CastPoint() subtracts it from the caster position (name UNRECOVERED). NOT initialised by the ctor in the golden (bug-for-bug).
    Vector3 m_field124;         // +124 — zeroed by the ctor, never read or written anywhere else in this class (name UNRECOVERED)
    Program m_onCast;           // +136 — the "onCast" program (Proto::SpellComponent extension field 558, message field 1 "on_cast")
};

} // namespace Caver

#endif
