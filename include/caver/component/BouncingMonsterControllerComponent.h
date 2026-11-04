// Recovered from libswordigo.so (1.4.13) evidence — vtable
// _ZTVN5Caver34BouncingMonsterControllerComponentE at 0x620F70.
// sizeof 0x118 (280 bytes) proven by:
//   operator new(0x118u) in Create() @0x350C48 and Clone() @0x3885E8.
#ifndef CAVER_BOUNCINGMONSTERCONTROLLERCOMPONENT_H
#define CAVER_BOUNCINGMONSTERCONTROLLERCOMPONENT_H

#include "MonsterControllerComponent.h"
#include "KeyframeAnimationComponent.h"
#include "../animation/AnimBlendNode.h"

namespace Caver {

class BouncingMonsterControllerComponent : public MonsterControllerComponent {
public:
    BouncingMonsterControllerComponent();
    virtual ~BouncingMonsterControllerComponent();

    static BouncingMonsterControllerComponent* Create();
    static long Interface();

    virtual BouncingMonsterControllerComponent* Clone() const;
    void InitWithComponent(const BouncingMonsterControllerComponent* other);

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
    void Jump();
    void Fall();

    KeyframeAnimationComponent* jumpAnimation();
    KeyframeAnimationComponent* fallAnimation();

    ComponentOutlet<KeyframeAnimationComponent> m_jumpAnimationOutlet; // +200 — ID 100 "jumpAnimation"
    ComponentOutlet<KeyframeAnimationComponent> m_fallAnimationOutlet; // +224 — ID 101 "fallAnimation"
    boost::intrusive_ptr<AnimBlendNode> m_blendNode;                  // +248 — cross-fades jump and fall
    float m_jumpAngle;                                                // +256 — stored in radians (proto jump_angle)
    float m_jumpSpeed;                                                // +260 — default 450.0f (proto jump_speed)
    int m_direction;                                                  // +264 — facing / horizontal direction (-1, 1)
    float m_jumpXVelocity;                                            // +268 — cached horizontal jump velocity
    bool m_isJumping;                                                 // +272
    bool m_isFalling;                                                 // +273
    char _pad274[6];                                                  // +274 — pad to 0x118 (280)
};

static_assert(sizeof(BouncingMonsterControllerComponent) == 0x118,
              "BouncingMonsterControllerComponent sizeof must be exactly 280 bytes");

} // namespace Caver

#endif // CAVER_BOUNCINGMONSTERCONTROLLERCOMPONENT_H
