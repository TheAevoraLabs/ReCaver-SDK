// Recovered from libswordigo.so (1.4.13) evidence — vtable
// _ZTVN5Caver32StaticMonsterControllerComponentE at 0x622070.
// sizeof 0x118 (280 bytes) proven by:
//   operator new(0x118u) in Create() @0x350B48 and Clone() @0x392768.
#ifndef CAVER_STATICMONSTERCONTROLLERCOMPONENT_H
#define CAVER_STATICMONSTERCONTROLLERCOMPONENT_H

#include <vector>
#include <string>
#include <limits>

#include "MonsterControllerComponent.h"
#include "KeyframeAnimationComponent.h"
#include "SoundEffectComponent.h"

namespace Caver {

class StaticMonsterControllerComponent : public MonsterControllerComponent {
public:
    StaticMonsterControllerComponent();
    virtual ~StaticMonsterControllerComponent();

    static StaticMonsterControllerComponent* Create();
    static long Interface();

    virtual StaticMonsterControllerComponent* Clone() const;
    void InitWithComponent(const StaticMonsterControllerComponent* other);

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

    ComponentOutlet<KeyframeAnimationComponent> m_animationOutlet;       // +200 — ID 100 "animation"
    ComponentOutlet<SoundEffectComponent> m_soundOutlets[2];             // +224 — ID 101 "sound1", ID 102 "sound2"
    int m_soundCount;                                                    // +272 — number of sounds in proto sound_id
    float m_lastAnimTime;                                                // +276 — previous animation time (FLT_MAX init)
};

static_assert(sizeof(StaticMonsterControllerComponent) == 0x118,
              "StaticMonsterControllerComponent sizeof must be exactly 280 bytes");

} // namespace Caver

#endif // CAVER_STATICMONSTERCONTROLLERCOMPONENT_H
