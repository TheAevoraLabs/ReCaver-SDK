// Recovered from libswordigo.so (1.4.13) evidence — see the combat
// slice board (~/workspace/.scratch/parity/combat/SkillComponent/
// RESULTS.md) for per-function parity status.
// Spell-casting component; sizeof 0xD8 (216, proven by Create()'s
// operator new(0xD8)). Member offsets proven from the ctor,
// InitWithComponent, LoadFromProtobufMessage and the casting bodies.
// Component base is sizeof 104 — first own member at +104.
#ifndef CAVER_SKILLCOMPONENT_H
#define CAVER_SKILLCOMPONENT_H

#include "Component.h"
#include "ComponentOutlet.h"
#include "../core/Vector2.h"
#include "../core/Vector3.h"

#include "KeyframeAnimationComponent.h"

namespace Caver {

class ObjectTemplate;
class SpellComponent;

class SkillComponent : public Component {
public:
    SkillComponent();
    virtual ~SkillComponent();

    static SkillComponent* Create();
    static long Interface();

    virtual Component* Clone() const;                                       // returns SkillComponent* (raw new)
    // NOT an override: the vtable slot 3 holds Component::InitWithComponent.
    // This is a non-virtual overload taking the narrowed type (its mangled
    // name uses EPKS0_ — the parameter is const SkillComponent*).
    void InitWithComponent(const SkillComponent* other);
    virtual void LoadFromProtobufMessage(const Proto::Component& message);
    virtual void SaveToProtobufMessage(Proto::Component* message) const;
    virtual bool ShouldSave();                                              // true
    virtual void GetBindings(std::vector<Binding>* bindings);
    virtual void GetEnumValuesForBindedProperty(int property, std::vector<BindingValue>* values); // tail-calls base
    virtual BindingValue ValueForBindedProperty(int property);
    virtual void SetValueForBindedProperty(int property, const BindingValue& value);
    virtual void Prepare();
    void RegisterLibrary();
    virtual void PerformBindedAction(int action);                           // empty
    virtual bool ImplementsInterface(long interfaceId);
    virtual void RegisterInterfaces(ComponentManager* manager);
    virtual bool RequiresUpdate() const;                                    // true
    virtual std::string className() const;                                  // "Skill" (proven: 5-char SSO string in the golden body — NOT "SkillComponent")

    virtual void Update(float deltaTime);
    virtual bool HandleMessage(int message, void* data);                    // returns false

    void SetCastObjectTemplate(const boost::intrusive_ptr<ObjectTemplate>& objectTemplate);
    void SetSpellTemplate(const boost::intrusive_ptr<ObjectTemplate>& objectTemplate);
    void CreateSpellObject();
    void BeginCasting();
    void FinishCasting();
    void CancelCasting();
    bool HasFinishedCasting();
    void UpdateCastObjectPosition();
    void UpdateSpellObjectPosition();

    ComponentOutlet<KeyframeAnimationComponent> m_castFinishAnimation; // +104 (vptr +104, identifier +112, target +120) — BindOutlet(0, "castFinishAnimation", this+104, false) in the ctor; the identifier doubles as the proto extension's cast_finish_animation_id
    Vector2 m_origin;                       // +128 — proto extension field "origin"; binding property 2
    Vector3 m_field136;                     // +136 — name UNRECOVERED: written by the Skill Lua library's BeginCasting from its 3rd argument (a Vector3) and copied into the SpellComponent at +124 by BeginCasting(); never initialised by the ctor in the golden (bug-for-bug)
    std::string m_castObjectTemplateName;   // +152 — proto extension field "cast_object_template_name"; binding property 1 ("$castObjectTemplate")
    boost::intrusive_ptr<ObjectTemplate> m_castObjectTemplate;  // +176
    boost::intrusive_ptr<SceneObject> m_castObject;             // +184
    boost::intrusive_ptr<ObjectTemplate> m_spellTemplate;       // +192
    boost::intrusive_ptr<SceneObject> m_spellObject;            // +200
    boost::intrusive_ptr<SpellComponent> m_spellComponent;      // +208 — the spell object's SpellComponent, cached by CreateSpellObject
};

} // namespace Caver

#endif
