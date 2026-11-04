// Recovered from libswordigo.so (1.4.13) evidence — see the world
// lane's MonsterControllerComponent.results.md for per-function parity.
// Base controller for every monster AI component (Walking / Charging /
// Bat / Generic / ... derive from it). sizeof 0xC8 (200) — proven by
// the first derived member in BatMonsterControllerComponent's ctor
// (its first own outlet is bound at +200) and by this class's own ctor
// store map. Bases: Component only (__si_class_type_info, base
// _ZTI Component — RTTI @0x621768). The primary vtable @0x621600 has
// exactly the Component slot order: this class overrides Load / Save /
// ShouldSave / Prepare / the four IBindable binding methods /
// PerformBindedAction / ImplementsInterface / RegisterInterfaces /
// RequiresUpdate / UpdateWhenPaused / Update / className, and slot 3
// keeps the BASE InitWithComponent (the EPKS0_ function below is a
// non-virtual narrowed overload, the HealthComponent pattern).
#ifndef CAVER_MONSTERCONTROLLERCOMPONENT_H
#define CAVER_MONSTERCONTROLLERCOMPONENT_H

#include "Component.h"
#include "ComponentOutlet.h"
#include "AnimationControllerComponent.h"
#include "EntityComponent.h"
#include "MonsterEntityComponent.h"
#include "UtilityShapeComponent.h"
#include "../core/Rectangle.h"
#include "../physics/CollisionShape.h"

namespace Caver {

class MonsterControllerComponent : public Component {
public:
    MonsterControllerComponent();
    virtual ~MonsterControllerComponent();

    static long Interface();

    // NOT an override: the vtable slot 3 holds
    // Component::InitWithComponent. This is a non-virtual overload
    // taking the narrowed type (mangling EPKS0_).
    void InitWithComponent(const MonsterControllerComponent* other);
    virtual void LoadFromProtobufMessage(const Proto::Component& message);
    virtual void SaveToProtobufMessage(Proto::Component* message) const;
    virtual bool ShouldSave();                                              // true
    virtual void GetBindings(std::vector<Binding>* bindings);
    virtual void GetEnumValuesForBindedProperty(int property, std::vector<BindingValue>* values);
    virtual BindingValue ValueForBindedProperty(int property);
    virtual void SetValueForBindedProperty(int property, const BindingValue& value);
    virtual void PerformBindedAction(int action);                           // empty
    virtual void Prepare();
    virtual bool ImplementsInterface(long interfaceId);
    virtual void RegisterInterfaces(ComponentManager* manager);
    virtual bool RequiresUpdate() const;                                    // true
    virtual bool UpdateWhenPaused() const;                                  // false
    virtual void Update(float deltaTime);                                   // empty
    virtual std::string className() const;                                  // "MonsterController"

    void UpdateRoamBounds();
    SceneObject* FindNearestEnemy();

    ComponentOutlet<MonsterEntityComponent> m_entityOutlet;                 // +104 — BindOutlet(0, "entity")
    ComponentOutlet<AnimationControllerComponent> m_animationControllerOutlet; // +128 — BindOutlet(1, "animationController")
    ComponentOutlet<UtilityShapeComponent> m_roamAreaOutlet;                // +152 — BindOutlet(2, "roamArea")
    Rectangle m_roamBounds;                                                 // +176 — scaled+translated roam-area shape bounds (Prepare/UpdateRoamBounds)
    float m_walkSpeed;                                                      // +192 — ctor 100.0f; binding property 3 "walkSpeed"; proto walk_speed
    char _pad196[4];                                                        // +196 — sizeof 200
};

} // namespace Caver

#endif
