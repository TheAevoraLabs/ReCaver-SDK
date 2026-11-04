// Recovered from libswordigo.so (1.4.13) evidence — vtable
// _ZTVN5Caver34ChargingMonsterControllerComponentE at 0x621100.
// sizeof 0x150 (336 bytes) proven by:
//   operator new(0x150u) in Create() @0x350A48 and Clone() @0x389D50.
#ifndef CAVER_CHARGINGMONSTERCONTROLLERCOMPONENT_H
#define CAVER_CHARGINGMONSTERCONTROLLERCOMPONENT_H

#include "MonsterControllerComponent.h"
#include "KeyframeAnimationComponent.h"
#include "../animation/AnimBlendNode.h"

namespace Caver {

class ChargingMonsterControllerComponent : public MonsterControllerComponent {
public:
    ChargingMonsterControllerComponent();
    virtual ~ChargingMonsterControllerComponent();

    static ChargingMonsterControllerComponent* Create();
    static long Interface();

    virtual ChargingMonsterControllerComponent* Clone() const;
    void InitWithComponent(const ChargingMonsterControllerComponent* other);

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
    virtual bool HandleMessage(int message, void* data);

    virtual bool ImplementsInterface(long interfaceId);
    virtual void RegisterInterfaces(ComponentManager* manager);
    virtual std::string className() const;

    void StartMovingToDirection(int direction);
    void StartCharging();
    void StopCharging();

    KeyframeAnimationComponent* walkAnimation();
    KeyframeAnimationComponent* chargeAnimation();
    KeyframeAnimationComponent* runAnimation();

    ComponentOutlet<KeyframeAnimationComponent> m_walkAnimationOutlet;   // +200 — ID 100 "walkAnimation"
    ComponentOutlet<KeyframeAnimationComponent> m_chargeAnimationOutlet; // +224 — ID 101 "chargeAnimation"
    ComponentOutlet<KeyframeAnimationComponent> m_runAnimationOutlet;    // +248 — ID 102 "runAnimation"
    boost::intrusive_ptr<AnimBlendNode> m_blendNode;                    // +272 — cross-fades walk and (charge+run)
    boost::intrusive_ptr<AnimBlendNode> m_chargeBlendNode;               // +280 — cross-fades charge and run
    float m_runSpeed;                                                   // +288 — default 230.0f (proto run_speed)
    float m_runAcceleration;                                            // +292 — default 300.0f (proto run_acceleration)
    bool m_isCharging;                                                  // +296
    bool m_isRunning;                                                   // +297
    char _pad298[2];                                                    // +298
    float m_walkTime;                                                   // +300
    float m_runTime;                                                    // +304
    float m_turnTimer;                                                  // +308
    boost::intrusive_ptr<SceneObject> m_targetEnemy;                    // +312
    int m_direction;                                                    // +320 — facing / movement direction (-1, 1)
    float m_directionTimer;                                             // +324 — timer since direction change (init FLT_MAX)
    float m_impactCooldownTimer;                                        // +328 — timer since obstacle impact (init FLT_MAX)
    bool m_isGrounded;                                                  // +332
    char _pad333[3];                                                    // +333 — pad to 0x150 (336)
};

static_assert(sizeof(ChargingMonsterControllerComponent) == 0x150,
              "ChargingMonsterControllerComponent sizeof must be exactly 336 bytes");

} // namespace Caver

#endif // CAVER_CHARGINGMONSTERCONTROLLERCOMPONENT_H
