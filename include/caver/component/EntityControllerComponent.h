// Recovered from libswordigo.so (1.4.13) evidence — IDA/Hex-Rays exports
// in ~/workspace/analysis/swordigo/functions/Caver/EntityControllerComponent/
// plus Ghidra arbitration and the golden binary's relocation records.
//
// AI / movement controller for an entity: drives the sibling
// EntityComponent (physics state) toward roam / follow / fight
// behaviours, feeds the move AnimKeysNode to the sibling
// AnimationControllerComponent, and substeps the entity, the animation
// controller and any swinging weapons in Update.
//
// Layout proven from the ctor store map (0x366304), Create's
// operator new(0x148), the destructor's member-teardown order
// (0x369DEC) and the field accesses of every method body:
//   sizeof = 0x148 (328). Component base occupies +0..+103.
//   +104 ComponentOutlet<EntityComponent>               "entity"
//   +128 ComponentOutlet<AnimationControllerComponent>  "animationController"
//   +152 ComponentOutlet<KeyframeAnimationComponent>    "defaultMoveAnimation"
//   +176 ComponentOutlet<ShapeComponent>                "roamArea"
//   (outlet target types proven by the four outlet vtable
//   relocations 0x61DC30 / 0x61E740 / 0x61D8C0 / 0x61E6F0.)
//   +200 Rectangle roam bounds (Shape::Bounds of the roam-area
//        shape, scaled by the owner's effective scale and translated
//        to the owner's position — computed in Prepare)
//   +216 float default move speed (ctor 100.0)   [proto field 5]
//   +220 float default acceleration (ctor 800.0) [proto field 6]
//   +224 float targeting distance (ctor 0)       [proto field 7]
//   +232 intrusive_ptr<SceneObject> target object
//   +248 intrusive_ptr<AnimKeysNode> move animation
//   +256 int   move direction (-1 / 0 / +1)
//   +260 float move speed (live copy of +216, set by Prepare /
//        SetMoveSpeed)
//   +264 float acceleration (live copy of +220)
//   +272 intrusive_ptr<EntityActionComponent> current action
//   +280 bool  action-cancelled flag (cleared by PerformAction;
//        name from the "EntityController" Lua library's
//        CancelAction / IsActionCancelled entries — tentative)
//   +284 float idle time (ctor 1000.0; reset by PerformAction,
//        accumulated in Update while no action is active; the Lua
//        library's IdleTime / IsIdle entries read this member)
//   +288 int   facing direction (the pending-facing target when a
//        facing delay is running)
//   +292 int   pending move direction (applied when +300 expires)
//   +296 float facing delay countdown
//   +300 float move delay countdown
//   +304 std::vector<intrusive_ptr<SwingableWeaponControllerComponent>>
//        weapons (filled by Prepare via
//        SceneObject::GetComponentsWithInterface<...>)
// The four sibling component classes are only forward-declared here;
// their minimal definitions live in EntityControllerComponent.cpp
// until their own slices land headers (see the results file's
// Hand-offs section).
#ifndef CAVER_ENTITYCONTROLLERCOMPONENT_H
#define CAVER_ENTITYCONTROLLERCOMPONENT_H

#include <vector>
#include <boost/intrusive_ptr.hpp>

#include "Component.h"
#include "EntityActionComponent.h"
#include "ComponentOutlet.h"
#include "../core/Rectangle.h"
#include "../animation/AnimKeysNode.h"
#include "../physics/CollisionShape.h"
#include "EntityComponent.h"
#include "SwingableWeaponControllerComponent.h"
#include "AnimationControllerComponent.h"
#include "KeyframeAnimationComponent.h"
#include "ShapeComponent.h"

namespace Caver {

class EntityControllerComponent : public Component {
public:
    EntityControllerComponent();
    virtual ~EntityControllerComponent();

    static EntityControllerComponent* Create();
    static long Interface();

    virtual Component* Clone() const;                                       // slot 2
    // NOT an override: the vtable slot 3 holds Component::InitWithComponent.
    // This is a non-virtual overload taking the narrowed type (its mangled
    // name uses EPKS0_ — the parameter is const EntityControllerComponent*).
    void InitWithComponent(const EntityControllerComponent* other);
    virtual void LoadFromProtobufMessage(const Proto::Component& message);  // slot 4
    virtual void SaveToProtobufMessage(Proto::Component* message) const;    // slot 5
    virtual bool ShouldSave();                                              // slot 6 — true
    virtual void Prepare();                                                 // slot 8
    virtual void GetBindings(std::vector<Binding>* bindings);               // slot 14
    virtual void GetEnumValuesForBindedProperty(int property, std::vector<BindingValue>* values); // slot 15
    virtual BindingValue ValueForBindedProperty(int property);               // slot 16
    virtual void SetValueForBindedProperty(int property, const BindingValue& value); // slot 17
    virtual void PerformBindedAction(int action);                           // slot 18 — empty
    virtual int componentCategories() const;                                // slot 19 — 2
    virtual bool ImplementsInterface(long interfaceId);                     // slot 20
    virtual void RegisterInterfaces(ComponentManager* manager);             // slot 21
    virtual bool RequiresUpdate() const;                                    // slot 23 — true
    virtual void Update(float deltaTime);                                   // slot 25
    virtual bool HandleMessage(int message, void* data);                    // slot 26 — always false
    virtual std::string className() const;                                  // slot 32 — "EntityController" (16 chars, proven by the SSO length byte 0x20 in the golden body; NOT the full C++ class name)

    void RegisterLibrary();

    void SetMoveAnimation(const boost::intrusive_ptr<AnimKeysNode>& animation);
    void SetMoveSpeed(float speed);
    void SetAcceleration(float acceleration);
    int FacingDirection();
    int MoveDirection();
    void RoamUpdate(float deltaTime);
    void SetMoveDirection(int direction);
    void SetFacingDirection(int direction);
    void SetFacingDirectionAfterDelay(int direction, float delay);
    void SetMoveDirectionAfterDelay(int direction, float delay);
    float UpdateMoveAnimationSpeed();
    bool ShouldReverseMoveAnimation();
    void PerformAction(const boost::intrusive_ptr<EntityActionComponent>& action);
    void SetTargetObject(const boost::intrusive_ptr<SceneObject>& object);

    ComponentOutlet<EntityComponent> m_entityOutlet;                        // +104
    ComponentOutlet<AnimationControllerComponent> m_animationControllerOutlet; // +128
    ComponentOutlet<KeyframeAnimationComponent> m_defaultMoveAnimationOutlet;  // +152
    ComponentOutlet<ShapeComponent> m_roamAreaOutlet;                       // +176
    Rectangle m_roamBounds;                                                 // +200
    float m_defaultMoveSpeed;                                               // +216
    float m_defaultAcceleration;                                            // +220
    float m_targetingDistance;                                              // +224
    boost::intrusive_ptr<SceneObject> m_targetObject;                       // +232
    int m_movementBehavior;                                                 // +240 — 0 none, 1 roam, 2 follow, 3 fight
    boost::intrusive_ptr<AnimKeysNode> m_moveAnimation;                     // +248
    int m_moveDirection;                                                    // +256
    float m_moveSpeed;                                                      // +260
    float m_acceleration;                                                   // +264
    boost::intrusive_ptr<EntityActionComponent> m_currentAction;            // +272
    bool m_actionCancelled;                                                 // +280
    float m_idleTime;                                                       // +284
    int m_facingDirection;                                                  // +288
    int m_pendingMoveDirection;                                             // +292
    float m_facingDelay;                                                    // +296
    float m_moveDelay;                                                      // +300
    std::vector<boost::intrusive_ptr<SwingableWeaponControllerComponent>> m_weapons; // +304
};

} // namespace Caver

#endif
