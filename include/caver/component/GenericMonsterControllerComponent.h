#ifndef CAVER_GENERICMONSTERCONTROLLERCOMPONENT_H
#define CAVER_GENERICMONSTERCONTROLLERCOMPONENT_H

#include "MonsterControllerComponent.h"
#include "KeyframeAnimationComponent.h"
#include "AttackComponent.h"
#include "../animation/AnimBlendNode.h"
#include <vector>
#include <boost/intrusive_ptr.hpp>

namespace Caver {

class GenericMonsterControllerComponent : public MonsterControllerComponent {
public:
    GenericMonsterControllerComponent();
    virtual ~GenericMonsterControllerComponent();

    static GenericMonsterControllerComponent* Create();
    static long Interface();

    virtual GenericMonsterControllerComponent* Clone() const;
    void InitWithComponent(const GenericMonsterControllerComponent* other);

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
    void SetFacingDirection(int direction);
    void SetCurrentMoveSpeed(float speed);

    KeyframeAnimationComponent* walkAnimation();

    ComponentOutlet<KeyframeAnimationComponent> m_walkAnimationOutlet; // +200 — ID 100 "walkAnimation"
    std::vector<boost::intrusive_ptr<AttackComponent>> m_attackComponents;      // +224
    boost::intrusive_ptr<AttackComponent> m_currentAttack;                     // +248
    int m_currentDirection;                                                    // +256
    float m_stateTimer;                                                        // +260 — ctor FLT_MAX, += dt, 0 on direction change, FLT_MAX on enemy acquire
    float m_field264;                                                          // +264 — ctor FLT_MAX; mirrored from entity physics m_field8 each Update (no reader in this component; kept for layout parity)
    float m_currentMoveSpeed;                                                  // +268
    boost::intrusive_ptr<SceneObject> m_targetEnemy;                            // +272
};

static_assert(sizeof(GenericMonsterControllerComponent) == 0x118,
              "GenericMonsterControllerComponent sizeof must be exactly 280 bytes");

} // namespace Caver

#endif // CAVER_GENERICMONSTERCONTROLLERCOMPONENT_H
