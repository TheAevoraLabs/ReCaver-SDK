// Recovered from libswordigo.so (1.4.13) evidence — vtable
// _ZTVN5Caver15AttackComponentE at 0x620BA0 (primary address point
// 0x620BB0, IBindable secondary 0x620CC8, IUpdateable secondary
// 0x620D08), _ZTI at 0x620D18 (a __si_class_type_info naming
// Caver::Component as the single base).
//
// sizeof 0x140 (320 bytes) is proven by both operator new sites:
// Clone() @0x38683C and the factory's Create() @0x3509C8.
//
// Layout (every offset proven by the ctor store map @0x384D3C, the
// destructor teardown @0x3866A4, Load/Save @0x385248/@0x3853BC and the
// binding accessors @0x385BA8/@0x385C5C):
//   +0    vptr, +8 refcount, +16/+24 secondary vptrs, +32 identifier,
//         +36 field36, +40 object, +48 flag, +56 label, +80 outlets map
//         (Component, 104 bytes)
//   +104  ComponentOutlet<KeyframeAnimationComponent> "animation"      (ID 0)
//   +128  ComponentOutlet<CollisionShapeComponent>   "collisionShape" (ID 1)
//   +152  ComponentOutlet<ShapeComponent>            "attackArea"     (ID 2)
//   +176  ComponentOutlet<SoundEffectComponent>      "soundEffect"    (ID 3)
//   +200  Program m_onAttack
//   +256  boost::intrusive_ptr<AnimBlendNode> m_animNode
//   +264  bool m_attacking
//   +265  bool m_stopping
//   +268  float m_attackInterval            (ctor 1.0f)
//   +272  float m_attackDuration            (ctor 0.0f)
//   +276  float m_attackTime                (NOT initialised by the golden ctor;
//                                            StartAttack zeroes it)
//   +280  float m_cooldownTime              (ctor 0.0f; accumulates while idle)
//   +284  float m_damageStartTime           (ctor 0.0f)
//   +288  float m_damageEndTime             (ctor 1.0f)
//   +292  float m_damageStartTime2          (ctor 0.0f)
//   +296  float m_damageEndTime2            (ctor 0.0f)
//   +300  bool m_damageEnabled
//   +301  bool m_firstDamageWindowActive
//   +302  bool m_secondDamageWindowActive
//   +304  float m_animationStartBlendTime   (ctor 0.4f)
//   +308  float m_animationEndBlendTime     (ctor 0.2f)
//   +312  float m_lastAnimationTime         (ctor -1.0f)
//   +316  4 bytes tail padding
//
// The primary vtable keeps the base Component::RequiresUpdate slot (the
// golden never overrides it), so this class is only updated when the
// scene explicitly drives it.
#ifndef CAVER_ATTACKCOMPONENT_H
#define CAVER_ATTACKCOMPONENT_H

#include <string>
#include <vector>

#include <boost/intrusive_ptr.hpp>

#include "Component.h"
#include "ComponentOutlet.h"
#include "KeyframeAnimationComponent.h"
#include "CollisionShapeComponent.h"
#include "ShapeComponent.h"
#include "SoundEffectComponent.h"
#include "../animation/AnimBlendNode.h"
#include "../scripting/Program.h"

namespace Caver {

class AttackComponent : public Component {
public:
    AttackComponent();
    virtual ~AttackComponent();

    static AttackComponent* Create();
    static long Interface();

    virtual AttackComponent* Clone() const;
    // NOT an override: vtable slot 3 holds Component::InitWithComponent.
    // This is a non-virtual overload taking the narrowed type (the
    // golden's mangled name uses EPKS0_).
    void InitWithComponent(const AttackComponent* other);

    virtual void LoadFromProtobufMessage(const Proto::Component& message);
    virtual void SaveToProtobufMessage(Proto::Component* message) const;
    virtual bool ShouldSave();

    virtual void GetBindings(std::vector<Binding>* bindings);
    virtual void GetEnumValuesForBindedProperty(int property, std::vector<BindingValue>* values);
    virtual BindingValue ValueForBindedProperty(int property);
    virtual void SetValueForBindedProperty(int property, const BindingValue& value);
    virtual void PerformBindedAction(int action);

    virtual void Prepare();
    virtual void Update(float deltaTime);

    virtual bool ImplementsInterface(long interfaceId);
    virtual void RegisterInterfaces(ComponentManager* manager);
    virtual std::string className() const;

    bool WorldPointInsideAttackArea(const Vector2& point);
    void StartAttack(const boost::intrusive_ptr<AnimNode>& node, SceneObject* target);
    void StopAttack();
    void SetDamageEnabled(bool enabled);

    ComponentOutlet<KeyframeAnimationComponent> m_animationOutlet;      // +104
    ComponentOutlet<CollisionShapeComponent> m_collisionShapeOutlet;    // +128
    ComponentOutlet<ShapeComponent> m_attackAreaOutlet;                 // +152
    ComponentOutlet<SoundEffectComponent> m_soundEffectOutlet;          // +176
    Program m_onAttack;                                                 // +200
    boost::intrusive_ptr<AnimBlendNode> m_animNode;                     // +256
    bool m_attacking;                                                   // +264
    bool m_stopping;                                                    // +265
    char _pad266[2];                                                    // +266
    float m_attackInterval;                                             // +268
    float m_attackDuration;                                             // +272
    float m_attackTime;                                                 // +276
    float m_cooldownTime;                                               // +280
    float m_damageStartTime;                                            // +284
    float m_damageEndTime;                                              // +288
    float m_damageStartTime2;                                           // +292
    float m_damageEndTime2;                                             // +296
    bool m_damageEnabled;                                               // +300
    bool m_firstDamageWindowActive;                                     // +301
    bool m_secondDamageWindowActive;                                    // +302
    char _pad303;                                                       // +303
    float m_animationStartBlendTime;                                    // +304
    float m_animationEndBlendTime;                                      // +308
    float m_lastAnimationTime;                                          // +312
    char _pad316[4];                                                    // +316
};

// Golden aarch64 NDK-libc++ layout anchor; MSVC STL member sizes differ,
// so this assert is Android/Linux-only (the Windows build is self-consistent
// under MSVC STL and does not claim golden byte-layout parity).
#ifndef _MSC_VER
static_assert(sizeof(AttackComponent) == 0x140,
              "AttackComponent sizeof must be exactly 320 bytes (0x140)");
#endif

} // namespace Caver

#endif // CAVER_ATTACKCOMPONENT_H
