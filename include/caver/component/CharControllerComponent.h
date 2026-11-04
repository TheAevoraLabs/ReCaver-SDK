// Recovered from libswordigo.so (1.4.13) evidence — CharControllerComponent
// worker A (hero-entity lane, march 2). Layout PROVEN from the ctor store
// map (0x358CBC, Create's operator new(0x300)), the dtor release order
// (0x35F228), InitWithComponent/Clone copy lists and the Load/Save
// protobuf field maps (0x359A00 / 0x359F28). sizeof = 0x300 (768).
// Member NAMES are recovered where the proto schema / BindOutlet names /
// binding names prove them; the rest are m_field<offset> (UNRECOVERED).
#ifndef CAVER_CHARCONTROLLERCOMPONENT_H
#define CAVER_CHARCONTROLLERCOMPONENT_H

#include "Component.h"
#include "ComponentOutlet.h"
#include "AnimationComponent.h"
#include "SoundEffectComponent.h"
#include "SwingableWeaponControllerComponent.h"
#include "../core/Vector2.h"
#include "../core/Vector3.h"
#include <boost/intrusive_ptr.hpp>

#include "CharAnimControllerComponent.h"
#include "EntityComponent.h"
#include "SwingComponent.h"
#include "SkillComponent.h"

namespace Caver {

class SoundEffectComponent;
class SceneObject;

class CharControllerComponent : public Component {
public:
    CharControllerComponent();
    virtual ~CharControllerComponent();

    static CharControllerComponent* Create();
    static long Interface();

    virtual Component* Clone() const;                                       // slot 2
    // NOT an override: vtable slot 3 keeps Component::InitWithComponent;
    // this is a non-virtual narrowed overload (mangling EPKS0_), exactly
    // the HealthComponent pattern (SHARED.md).
    void InitWithComponent(const CharControllerComponent* other);
    virtual void LoadFromProtobufMessage(const Proto::Component& message);  // slot 4
    virtual void SaveToProtobufMessage(Proto::Component* message) const;    // slot 5
    virtual bool ShouldSave();                                              // slot 6 — true
    virtual void Prepare();                                                 // slot 8
    virtual void GetBindings(std::vector<Binding>* bindings);               // slot 14
    virtual void GetEnumValuesForBindedProperty(int property, std::vector<BindingValue>* values); // slot 15 — golden body is a thunk to the base
    virtual BindingValue ValueForBindedProperty(int property);             // slot 16
    virtual void SetValueForBindedProperty(int property, const BindingValue& value); // slot 17
    virtual bool ImplementsInterface(long interfaceId);                     // slot 20
    virtual void RegisterInterfaces(ComponentManager* manager);           // slot 21
    virtual bool RequiresUpdate() const;                                  // slot 23 — true
    virtual void Update(float deltaTime);                                   // slot 25 — worker B
    virtual bool HandleMessage(int message, void* data);                    // slot 26 — worker B
    virtual std::string className() const;                                  // slot 32 — "CharController"

    // Non-virtual, this worker (A):
    void RegisterLibrary();
    void SetInitialAnimationControllerState();
    CharAnimControllerComponent* animationController();

    // Non-virtual, worker B (declared here; defined in
    // CharControllerComponentB.cpp):
    void Swing();
    void StopSwing();
    bool CanSwing();
    void Hurt();
    void Die();
    void Pickup(SceneObject* object, bool flag);
    void PickupWithoutAnimation();
    void BeginCasting(const boost::intrusive_ptr<SkillComponent>& skill);
    void FinishCasting();
    void CancelCasting();
    void SetWeaponsHidden(bool hidden);
    void DropQuickly();
    void DropWithoutAnimation();
    void UpdatePickupObjectPosition();
    Vector2 TargetPickupObjectPosition();
    void StartMovingToDirection(int direction);
    void StopMovingToDirection(int direction);
    void StartJumping();
    void StopJumping();
    void ResetAirJumpCount();
    bool CanDoSomething();
    bool CanJump();
    bool CanUse();
    bool CanPickup();
    bool CanBeginCasting();
    void Use();

    // ---- layout (offsets proven; see results file for evidence) ----
    ComponentOutlet<EntityComponent> m_entityOutlet;                        // +104  BindOutlet 0 "entity"
    ComponentOutlet<CharAnimControllerComponent> m_defaultAnimationControllerOutlet; // +128  BindOutlet 1 "defaultAnimationController"
    ComponentOutlet<CharAnimControllerComponent> m_liftAnimationControllerOutlet;    // +152  BindOutlet 2 "liftAnimationController"
    ComponentOutlet<AnimationComponent> m_liftAnimationOutlet;              // +176  BindOutlet 3 "liftAnimation"
    ComponentOutlet<AnimationComponent> m_dropAnimationOutlet;              // +200  BindOutlet 4 "dropAnimation"
    ComponentOutlet<AnimationComponent> m_throwAnimationOutlet;             // +224  BindOutlet 5 "throwAnimation"
    ComponentOutlet<AnimationComponent> m_pushAnimationOutlet;              // +248  BindOutlet 6 "pushAnimation"
    ComponentOutlet<AnimationComponent> m_hurtAnimationOutlet;              // +272  BindOutlet 7 "hurtAnimation"
    ComponentOutlet<AnimationComponent> m_dieAnimationOutlet;               // +296  BindOutlet 8 "dieAnimation"
    ComponentOutlet<SoundEffectComponent> m_jumpSoundOutlet;                // +320  BindOutlet 9 "jumpSound"
    ComponentOutlet<SoundEffectComponent> m_airJumpSoundOutlet;             // +344  BindOutlet 10 "airJumpSound"
    ComponentOutlet<SoundEffectComponent> m_jumpLandSoundOutlet;            // +368  BindOutlet 11 "jumpLandSound"
    // +392: cached animation controller, RAW intrusive_ptr pattern
    // (manual refcount at pointee+8, release via vtable slot +8 — the
    // golden animationController() expansion; NOT boost::intrusive_ptr,
    // whose operator= does not reproduce it).
    CharAnimControllerComponent* m_animationControllerCache;                // +392
    ComponentOutlet<SwingableWeaponControllerComponent> m_rightWeaponControllerOutlet; // +400  BindOutlet 12 "rightWeaponController"
    ComponentOutlet<SwingableWeaponControllerComponent> m_leftWeaponControllerOutlet;  // +424  BindOutlet 13 "leftWeaponController"
    bool m_field448;                                                        // +448 — weapons-hidden flag (CanSwing/SetWeaponsHidden; name UNRECOVERED)
    ComponentOutlet<SwingComponent> m_swingOutlet0;                         // +456  BindOutlet 14 "swing1"
    ComponentOutlet<SwingComponent> m_swingOutlet1;                         // +480  BindOutlet 15 "swing2"
    ComponentOutlet<SwingComponent> m_swingOutlet2;                         // +504  BindOutlet 16 "swing3"
    int m_field528;                                                         // +528 — swing_component_id count written by Load (name UNRECOVERED)
    int m_field532;                                                         // +532
    float m_field536;                                                       // +536 — ctor FLT_MAX (0x7F7FFFFF)
    bool m_field540;                                                        // +540
    float m_field544;                                                       // +544 (541..543 tail pad before it)
    int m_facingDirection;                                                  // +548 (desktop SRE header agrees)
    int m_moveDirection;                                                    // +552 (desktop agrees; device hint confirmed)
    bool m_isMoving;                                                        // +556
    char _pad557[3];
    Vector3 m_field560;                                                     // +560 (floats 560/564/568; name UNRECOVERED)
    bool m_field572;                                                        // +572
    bool m_field573;                                                        // +573 — ctor true
    char _pad574[2];
    int m_field576;                                                         // +576
    bool m_isJumping;                                                       // +580 (desktop agrees)
    char _pad581[3];
    int m_jumpState;                                                        // +584 (desktop agrees)
    float m_jumpTimer;                                                      // +588 (desktop says int; binary treats as float in Update — modelled float, UNRECOVERED certainty)
    float m_field592;                                                       // +592 — ctor FLT_MAX
    bool m_isFalling;                                                       // +596 (desktop agrees)
    char _pad597[3];
    int m_airJumpCount;                                                     // +600 (desktop agrees)
    float m_field604;                                                       // +604
    float m_field608;                                                       // +608 — SetValue property 18 writes HERE (golden quirk, bug-for-bug)
    float m_jumpSpeed;                                                      // +612 — ctor 250.0f; proto jump_speed; binding 17
    float m_field616;                                                       // +616
    float m_field620;                                                       // +620 — SetValue property 19 writes HERE (golden quirk)
    float m_runSpeed;                                                       // +624 — ctor 230.0f; proto normal_run_speed; binding 18
    float m_maxJumpTime;                                                    // +628 — ctor 0.23f; proto normal_max_jump_time; binding 19
    float m_fastRunSpeed;                                                   // +632 — ctor 320.0f; proto fast_run_speed; binding 20
    float m_fastMaxJumpTime;                                                // +636 — ctor 0.11f; proto fast_max_jump_time; binding 21
    ReferenceCountedObject* m_field640;                                     // +640 — RAW intrusive (dtor releases it; pointee UNRECOVERED)
    int m_field648;                                                         // +648
    int m_field652;                                                         // +652
    ReferenceCountedObject* m_field656;                                     // +656 — RAW intrusive (dtor releases it)
    float m_field664;                                                       // +664
    int m_field668;                                                         // +668
    ReferenceCountedObject* m_field672;                                     // +672 — RAW intrusive (dtor releases it)
    int m_field680;                                                         // +680
    float m_field684;                                                       // +684
    float m_field688;                                                       // +688
    unsigned char m_field692;                                               // +692
    bool m_field693;                                                        // +693
    char _pad694[2];
    Vector3 m_field696;                                                     // +696 (pickup-position area; name UNRECOVERED)
    int m_field708;                                                         // +708
    int m_field712;                                                         // +712 — state field read by Hurt/Die/Update (name UNRECOVERED)
    float m_field716;                                                       // +716
    float m_field720;                                                       // +720
    float m_field724;                                                       // +724
    float m_field728;                                                       // +728
    float m_field732;                                                       // +732
    int m_field736;                                                         // +736
    bool m_isCasting;                                                        // +740 (desktop agrees)
    char _pad741[3];
    SkillComponent* m_skillComponent;                                       // +744 — RAW intrusive (desktop agrees; dtor releases it)
    float m_cooldown;                                                       // +752 (desktop agrees)
    int m_field756;                                                         // +756
    float m_field760;                                                       // +760
    char _pad764[4];
};

static_assert(sizeof(CharControllerComponent) == 0x300, "CharControllerComponent sizeof must be 0x300");

} // namespace Caver

#endif
