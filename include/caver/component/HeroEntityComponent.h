// Recovered from libswordigo.so (1.4.13) evidence — IDA/Hex-Rays exports
// in ~/workspace/analysis/swordigo/functions/Caver/HeroEntityComponent/,
// the golden vtable _ZTVN5Caver19HeroEntityComponentE @0x61EB30
// (address points 0x61EB40 / 0x61EC58 / 0x61EC98, RTTI is a
// __si_class_type_info naming EntityComponent as the sole base) and
// Create()'s operator new(0x1B8).
//
// Layout: EntityComponent base occupies +0..+295 (its own sizeof is
// 0x128, proven by EntityComponent::Create's operator new(0x128);
// PhysicsObjectState is embedded at +112 inside it). Every Hero member
// offset below is proven from the ctor (zeroes +296..+439), the dtor
// (member teardown order +392 deque, +328 Program, then the raw
// intrusive releases at +320/+312/+304/+296) and the method bodies.
// sizeof(HeroEntityComponent) == 0x1B8 (440).
#ifndef CAVER_HEROENTITYCOMPONENT_H
#define CAVER_HEROENTITYCOMPONENT_H

#include <deque>

#include "EntityComponent.h"
#include "../core/Vector3.h"
#include "../scripting/Program.h"

namespace Caver {

class CollisionShapeComponent;
class DamageImpact;
class SceneObject;
struct CollisionInfo;

// One remembered safe landing spot, the element of the deque at +392.
// Layout proven from AddSafePosition / ResetToSafePosition: the hero's
// position at +0 (12 bytes), the ground object (the PhysicsObjectState's
// intrusive ground pointer) at +16, that object's position at +24 and
// its rotation at +36. sizeof 40 proven by the deque chunk arithmetic
// in the golden (102 elements per 4080-byte chunk block).
struct SafePosition {
    Vector3 m_position;                             // +0
    char _pad12[4];                                 // +12 — never written by any body
    boost::intrusive_ptr<SceneObject> m_groundObject; // +16
    Vector3 m_groundPosition;                       // +24
    float m_groundRotation;                         // +36
};

class HeroEntityComponent : public EntityComponent {
public:
    HeroEntityComponent();
    virtual ~HeroEntityComponent();

    static HeroEntityComponent* Create();
    static long Interface();

    virtual HeroEntityComponent* Clone() const;                             // slot 2 (covariant return)
    // NOT an override: the vtable slot 3 holds
    // Component::InitWithComponent (vtable relocation at 0x61EB58).
    // Non-virtual overload taking the narrowed type (mangled EPKS0_).
    void InitWithComponent(const HeroEntityComponent* other);
    virtual void LoadFromProtobufMessage(const Proto::Component& message);  // slot 4
    virtual void SaveToProtobufMessage(Proto::Component* message) const;    // slot 5
    virtual bool ShouldSave();                                              // slot 6 — true
    virtual void Prepare();                                                 // slot 8
    virtual void GetBindings(std::vector<Binding>* bindings);               // slot 14
    virtual void GetEnumValuesForBindedProperty(int property, std::vector<BindingValue>* values); // slot 15 — tail-calls EntityComponent's
    virtual BindingValue ValueForBindedProperty(int property);              // slot 16
    virtual void SetValueForBindedProperty(int property, const BindingValue& value); // slot 17
    virtual void PerformBindedAction(int action);                           // slot 18 — tail-calls EntityComponent's
    virtual int componentCategories() const;                                // slot 19 — 2
    virtual bool ImplementsInterface(long interfaceId);                     // slot 20
    virtual void RegisterInterfaces(ComponentManager* manager);             // slot 21
    virtual void Update(float deltaTime);                                   // slot 25
    virtual bool HandleMessage(int message, void* data);                    // slot 26
    virtual std::string className() const;                                  // slot 32 — "HeroEntity"

    // Non-virtual gameplay entry points (proven callees of
    // HandleMessage / PortalComponent logic; absent from the vtable):
    void AddSafePosition();
    void ResetToSafePosition();
    void HandleDamageCollision(CollisionShapeComponent* shape, CollisionShapeComponent* otherShape, const CollisionInfo& info);
    void HandleDamageImpact(DamageImpact* impact);
    void HandleItemCollection(SceneObject* object);
    void TargetObject(const boost::intrusive_ptr<SceneObject>& object, int value);

    boost::intrusive_ptr<Component> m_healthComponent;  // +296 — cached by Prepare via HealthComponent::Interface
    boost::intrusive_ptr<Component> m_manaComponent;    // +304 — cached by Prepare via ManaComponent::Interface
    boost::intrusive_ptr<Component> m_portalComponent;  // +312 — the active PortalComponent (HandleMessage 7)
    boost::intrusive_ptr<Component> m_portalShape;      // +320 — the portal's CollisionShapeComponent (HandleMessage 7)
    Program m_onItemGet;                                // +328 — the "onItemGet" binding (property 100), sizeof Program 0x38
    float m_safePositionTimer;                          // +384 — accumulated by Update, reset by AddSafePosition
    char _pad388[4];                                    // +388
    std::deque<SafePosition> m_safePositions;           // +392 (48 bytes: map begin/end/capacity, start, size)
};

} // namespace Caver

#endif
