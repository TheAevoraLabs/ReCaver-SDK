// Recovered from libswordigo.so (1.4.13) evidence — see
// ~/workspace/.scratch/parity/combat/DamageComponent/RESULTS.md.
// Damage-dealing component; sizeof 0xD0 (proven by Create()'s
// operator new(0xD0)). Member offsets are proven from the ctor,
// InitWithComponent, Load/SaveToProtobufMessage and
// AddPotentialImpact/ResolvePotentialImpacts bodies.
#ifndef CAVER_DAMAGECOMPONENT_H
#define CAVER_DAMAGECOMPONENT_H

#include "Component.h"
#include "../core/Vector2.h"
#include "../core/Vector3.h"    // Vector4

namespace Caver {

class DamageComponent;

// One queued damage impact. Layout proven from AddPotentialImpact's
// stack construction and the 64-byte element stride in the
// ResolvePotentialImpacts / ~DamageComponent vector loops:
//   +0  the DamageComponent that produced the impact
//   +8  target object (intrusive_ptr)
//   +16 position (AddPotentialImpact's 2nd vector argument)
//   +24 direction (AddPotentialImpact's 3rd vector argument)
//   +32 squared distance from the component's owner to +16
//   +36 blocked flag (set by ResolvePotentialImpacts)
//   +40 int, always 0 at construction (name UNRECOVERED)
//   +44 Vector4 payload (AddPotentialImpact's 4th argument; stored at
//       the unaligned offset 44 in the golden too)
// sizeof 64. The full home of this type is undecided (HealthComponent
// also consumes it via TakeDamageWithImpact(DamageImpact*), reading
// +0's component fields); it is defined here because this lane's
// methods prove the layout.
struct DamageImpact {
    DamageComponent* m_component;               // +0
    boost::intrusive_ptr<SceneObject> m_object; // +8
    Vector2 m_position;                         // +16
    Vector2 m_direction;                        // +24
    float m_distanceSquared;                    // +32
    bool m_blocked;                             // +36
    int m_field40;                              // +40 (UNRECOVERED)
    Vector4 m_impact;                           // +44
    char _pad60[4];                             // +60 — sizeof 64
};

// Damage type enums (values proven from the proto conversion clamps at
// 0x3B5098..0x3B50D8: DamageType passes 0/1, SpecialDamageType 0/1/2).
// Enumerator spellings UNRECOVERED.
enum DamageType {
    DamageType_V0 = 0,
    DamageType_V1 = 1
};
enum SpecialDamageType {
    SpecialDamageType_V0 = 0,
    SpecialDamageType_V1 = 1,
    SpecialDamageType_V2 = 2
};

// Free helpers (the conversion bodies at 0x3B5098.. belong to a
// shared damage-type TU and take the Proto enum types; they are
// declared at the use site in DamageComponent.cpp, after the
// generated proto header. The StringFor* bodies live in this TU in
// the golden, between SaveToProtobufMessage and GetBindings).
std::string StringForDamageType(DamageType value);
std::string StringForSpecialDamageType(SpecialDamageType value);

class DamageComponent : public Component {
public:
    DamageComponent();
    virtual ~DamageComponent();

    static DamageComponent* Create();
    static long Interface();

    virtual Component* Clone() const;
    // NOT an override: same narrowed non-virtual overload pattern as
    // HealthComponent (mangling EPKS0_).
    void InitWithComponent(const DamageComponent* other);
    virtual void LoadFromProtobufMessage(const Proto::Component& message);
    virtual void SaveToProtobufMessage(Proto::Component* message) const;
    virtual bool ShouldSave();                                              // true
    virtual void GetBindings(std::vector<Binding>* bindings);
    virtual void GetEnumValuesForBindedProperty(int property, std::vector<BindingValue>* values);
    virtual BindingValue ValueForBindedProperty(int property);
    virtual void SetValueForBindedProperty(int property, const BindingValue& value);
    virtual void Prepare();
    void RegisterLibrary();
    virtual void PerformBindedAction(int action);                           // empty
    virtual int componentCategories() const;                                // 2
    virtual bool ImplementsInterface(long interfaceId);
    virtual void RegisterInterfaces(ComponentManager* manager);
    virtual bool RequiresUpdate() const;                                    // m_standAlone
    virtual std::string className() const;                                  // "Damage" (golden, sic)

    virtual void Update(float deltaTime);
    virtual bool HandleMessage(int message, void* data);

    void AddPotentialImpact(const boost::intrusive_ptr<SceneObject>& object,
                            const Vector2& position, const Vector2& direction,
                            bool immediate, const Vector4& impact);
    void ResolvePotentialImpacts(bool deliverAll);
    void DeliverImpact(DamageImpact* impact);
    bool IsImpactBlocked(const DamageImpact& impact);

    std::vector<DamageImpact> m_impacts;        // +104 — impacts pending resolution
    std::vector<DamageImpact> m_blockableImpacts; // +128 — impacts that can be blocked (matched against m_impacts)
    bool m_ignoreTargetImmunity;                // +152 — proto ignore_target_immunity
    int m_minDamage;                            // +156 — proto min_damage
    int m_damageRange;                          // +160 — max_damage - min_damage (SetValue clamps >= 0)
    int m_field164;                             // +164 — Load stores min_damage here too (name UNRECOVERED)
    int m_field168;                             // +168 — Load stores the range here too (name UNRECOVERED)
    DamageType m_damageType;                    // +172 — proto damage_type
    SpecialDamageType m_specialDamageType;      // +176 — proto special_damage_type
    bool m_field180;                            // +180 — true in ctor; gates HandleMessage damage intake (name UNRECOVERED)
    int m_field184;                             // +184 — copied by InitWithComponent/Clone; read by HealthComponent::TakeDamageWithImpact as a counter (name UNRECOVERED)
    int m_field188;                             // +188 — read by HealthComponent::TakeDamageWithImpact as a counter (name UNRECOVERED)
    float m_physicalDamageFactor;               // +192 — proto physical_damage_factor, ctor 1.0f
    float m_magicDamageFactor;                  // +196 — proto magic_damage_factor, ctor 0.0f
    bool m_standAlone;                          // +200 — proto stand_alone; RequiresUpdate/Update gate
    bool m_canBeBlocked;                        // +201 — proto can_be_blocked, ctor true
    char _pad202[6];                            // +202 — sizeof 0xD0
};

} // namespace Caver

#endif
