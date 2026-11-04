#ifndef CAVER_LEAPINGMONSTERCONTROLLERCOMPONENT_H
#define CAVER_LEAPINGMONSTERCONTROLLERCOMPONENT_H

#include <vector>
#include <string>
#include <limits>

#include "MonsterControllerComponent.h"
#include "KeyframeAnimationComponent.h"
#include "AttackComponent.h"
#include "../physics/CollisionShape.h"

namespace Caver {

class LeapingMonsterControllerComponent : public MonsterControllerComponent {
public:
    LeapingMonsterControllerComponent();
    virtual ~LeapingMonsterControllerComponent();

    static LeapingMonsterControllerComponent* Create();
    static long Interface();

    virtual LeapingMonsterControllerComponent* Clone() const;
    void InitWithComponent(const LeapingMonsterControllerComponent* other);

    virtual void LoadFromProtobufMessage(const Proto::Component& message);
    virtual void SaveToProtobufMessage(Proto::Component* message) const;
    virtual bool ShouldSave();

    virtual bool Process(std::vector<boost::intrusive_ptr<Component>>* components);

    virtual void GetBindings(std::vector<Binding>* bindings);
    virtual void GetEnumValuesForBindedProperty(int property, std::vector<BindingValue>* values);
    virtual BindingValue ValueForBindedProperty(int property);
    virtual void SetValueForBindedProperty(int property, const BindingValue& value);
    virtual void PerformBindedAction(int action);

    virtual void Update(float deltaTime);
    void HandleGroundCollision(const CollisionInfo& info);

    virtual bool ImplementsInterface(long interfaceId);
    virtual void RegisterInterfaces(ComponentManager* manager);
    virtual std::string className() const;

    KeyframeAnimationComponent* walkAnimation() { return m_walkAnimationOutlet.target(this); }
    AttackComponent* leapAttack() { return m_leapAttackOutlet.target(this); }

    ComponentOutlet<KeyframeAnimationComponent> m_walkAnimationOutlet; // +200 — ID 100 "walkAnimation"
    ComponentOutlet<AttackComponent> m_leapAttackOutlet;              // +224 — ID 101 "leapAttack"
    void* m_field248;                                                 // +248 — init nullptr
    bool m_field256;                                                  // +256 — init false
    char _pad257[7];                                                  // +257 — alignment padding
    float m_field264;                                                 // +264 — init 0.0f
    float m_field268;                                                 // +268 — init FLT_MAX
};

static_assert(sizeof(LeapingMonsterControllerComponent) == 0x110,
              "LeapingMonsterControllerComponent sizeof must be exactly 272 bytes");

} // namespace Caver

#endif // CAVER_LEAPINGMONSTERCONTROLLERCOMPONENT_H
