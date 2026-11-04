// Recovered from libswordigo.so (1.4.13) evidence — vtable
// _ZTVN5Caver33WalkingMonsterControllerComponentE at 0x622200.
// sizeof 0xE8 (232 bytes) proven by:
//   operator new(0xE8u) in Create() @0x350A08 and Clone() @0x393070.
#ifndef CAVER_WALKINGMONSTERCONTROLLERCOMPONENT_H
#define CAVER_WALKINGMONSTERCONTROLLERCOMPONENT_H

#include "MonsterControllerComponent.h"
#include "KeyframeAnimationComponent.h"

namespace Caver {

class WalkingMonsterControllerComponent : public MonsterControllerComponent {
public:
    WalkingMonsterControllerComponent();
    virtual ~WalkingMonsterControllerComponent();

    static WalkingMonsterControllerComponent* Create();
    static long Interface();

    virtual WalkingMonsterControllerComponent* Clone() const;
    void InitWithComponent(const WalkingMonsterControllerComponent* other);

    virtual void LoadFromProtobufMessage(const Proto::Component& message);
    virtual void SaveToProtobufMessage(Proto::Component* message) const;
    virtual bool ShouldSave();

    virtual void GetBindings(std::vector<Binding>* bindings);
    virtual void GetEnumValuesForBindedProperty(int property, std::vector<BindingValue>* values);
    virtual BindingValue ValueForBindedProperty(int property);
    virtual void SetValueForBindedProperty(int property, const BindingValue& value);
    virtual void PerformBindedAction(int action);

    virtual void Update(float deltaTime);
    virtual bool HandleMessage(int message, void* data);

    virtual bool ImplementsInterface(long interfaceId);
    virtual void RegisterInterfaces(ComponentManager* manager);
    virtual std::string className() const;

    void StartMovingToDirection(int direction);

    KeyframeAnimationComponent* walkAnimation();

    ComponentOutlet<KeyframeAnimationComponent> m_walkAnimationOutlet;   // +200 — ID 100 "walkAnimation"
    int m_direction;                                                    // +224 — facing / movement direction (-1, 1, 0 initially)
    char _pad228[4];                                                    // +228 — pad to 0xE8 (232 bytes)
};

static_assert(sizeof(WalkingMonsterControllerComponent) == 0xE8,
              "WalkingMonsterControllerComponent sizeof must be exactly 232 bytes");

} // namespace Caver

#endif // CAVER_WALKINGMONSTERCONTROLLERCOMPONENT_H
