// Recovered from libswordigo.so (1.4.13) evidence — vtable
// _ZTVN5Caver29BatMonsterControllerComponentE at 0x620D40.
// sizeof 0x100 (256 bytes) proven by:
//   operator new(0x100u) in Create() @0x350C08 and Clone() @0x387324.
#ifndef CAVER_BATMONSTERCONTROLLERCOMPONENT_H
#define CAVER_BATMONSTERCONTROLLERCOMPONENT_H

#include "MonsterControllerComponent.h"
#include "KeyframeAnimationComponent.h"
#include "SoundEffectComponent.h"

namespace Caver {

class BatMonsterControllerComponent : public MonsterControllerComponent {
public:
    BatMonsterControllerComponent();
    virtual ~BatMonsterControllerComponent();

    static BatMonsterControllerComponent* Create();
    static long Interface();

    virtual BatMonsterControllerComponent* Clone() const;
    void InitWithComponent(const BatMonsterControllerComponent* other);

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

    ComponentOutlet<KeyframeAnimationComponent> m_flyAnimationOutlet;   // +200 — ID 100 "flyAnimation"
    ComponentOutlet<SoundEffectComponent> m_flapSoundOutlet;            // +224 — ID 101 "flapSound"
    float m_flightTimer;                                                // +248 — Flight oscillation phase timer
    float m_flapTimer;                                                  // +252 — Flap sound interval timer
};

static_assert(sizeof(BatMonsterControllerComponent) == 0x100, "BatMonsterControllerComponent must be exactly 256 bytes");

} // namespace Caver

#endif // CAVER_BATMONSTERCONTROLLERCOMPONENT_H
