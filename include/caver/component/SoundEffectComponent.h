#ifndef CAVER_SOUNDEFFECTCOMPONENT_H
#define CAVER_SOUNDEFFECTCOMPONENT_H

#include "Component.h"
#include "../audio/SoundEffect.h"
#include "../audio/SoundEffectSource.h"
#include <boost/intrusive_ptr.hpp>
#include <string>

namespace Caver {

// Recovered from libswordigo.so (1.4.13) evidence — vtable _ZTVN5Caver20SoundEffectComponentE
// at 0x61B1D0 (sizeof 0xA0 = 160 bytes). Plays sound effects with optional delay.
// Primary vtable slots:
//   0..32  Component virtual overrides
//   33     SoundEffectSourceDidFinishPlaying(SoundEffectSource*) (primary slot 35 / secondary slot 0 at offset 104)
class SoundEffectComponent : public Component, public ISoundEffectSourceDelegate {
public:
    SoundEffectComponent();
    virtual ~SoundEffectComponent();

    static SoundEffectComponent* Create();
    static long Interface();

    virtual SoundEffectComponent* Clone() const;
    void InitWithComponent(const SoundEffectComponent* other);

    virtual void LoadFromProtobufMessage(const Proto::Component& message);
    virtual void SaveToProtobufMessage(Proto::Component* message) const;
    virtual bool ShouldSave();
    virtual void Prepare();
    virtual void GetBindings(std::vector<Binding>* bindings);
    virtual void GetEnumValuesForBindedProperty(int property, std::vector<BindingValue>* values);
    virtual BindingValue ValueForBindedProperty(int property);
    virtual void SetValueForBindedProperty(int property, const BindingValue& value);
    virtual void PerformBindedAction(int action);
    virtual bool ImplementsInterface(long interfaceId);
    virtual void RegisterInterfaces(ComponentManager* manager);
    virtual std::string className() const;

    virtual void SoundEffectSourceDidFinishPlaying(SoundEffectSource* source);

    void CancelDelayed();
    void Play();
    void PlayWithDelay(float delay);
    bool IsPlaying();
    void Stop();

    std::string m_name;                                          // +112
    float m_volume;                                              // +136 (default 1.0f)
    float m_delay;                                               // +140 (default 0.0f)
    boost::intrusive_ptr<SoundEffect> m_effect;                  // +144
    boost::intrusive_ptr<SoundEffectSource> m_source;           // +152
};

} // namespace Caver

#endif // CAVER_SOUNDEFFECTCOMPONENT_H
