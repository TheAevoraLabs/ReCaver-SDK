#ifndef CAVER_FIREBREATHCOMPONENT_H
#define CAVER_FIREBREATHCOMPONENT_H

#include "../scene/SceneObject.h"
#include "ParticleEmitterComponent.h"
#include "SoundEffectComponent.h"
#include "SpellComponent.h"
#include "ComponentOutlet.h"
#include "../core/FloatColor.h"

namespace Caver {

class FireBreathComponent : public SpellComponent {
public:
    FireBreathComponent();
    virtual ~FireBreathComponent();
    static FireBreathComponent* Create();
    static long Interface();
    // 0x379F74: allocate 0x110, run the ctor, SpellComponent::InitWithComponent,
    // then copy the two outlet identifiers (+208, +232) and the 16-byte
    // colour at +248. The result pointer is the clone (mov x0, x19).
    virtual Component* Clone() const;
    // 0x37A04C: `return 1;`
    virtual bool RequiresUpdate() const;
    void InitWithComponent(const FireBreathComponent* other);
    virtual void LoadFromProtobufMessage(const Proto::Component& message);
    virtual void SaveToProtobufMessage(Proto::Component* message) const;
    virtual void Update(float deltaTime);
    virtual void Prepare();
    virtual void PerformBindedAction(int action);
    virtual bool ShouldSave();
    virtual bool ImplementsInterface(long interfaceId);
    virtual void RegisterInterfaces(ComponentManager* manager);
    virtual void GetBindings(std::vector<Binding>* bindings);
    virtual void GetEnumValuesForBindedProperty(int property, std::vector<BindingValue>* values);
    virtual BindingValue ValueForBindedProperty(int property);
    virtual void SetValueForBindedProperty(int property, const BindingValue& value);
    virtual bool ShouldChannel();
    virtual void StartChanneling();
    virtual void StopChanneling();
    virtual std::string className() const;

    char _padding192[8];                                               // +192
    ComponentOutlet<ParticleEmitterComponent> m_particleEmitterOutlet; // +200
    ComponentOutlet<SoundEffectComponent> m_swooshSoundOutlet;         // +224
    FloatColor m_color;                                                // +248
    float m_particleTimer;                                             // +264
    float m_channelTimer;                                              // +268
};

// Golden aarch64 NDK-libc++ layout anchor; MSVC STL member sizes differ,
// so this assert is Android/Linux-only (the Windows build is self-consistent
// under MSVC STL and does not claim golden byte-layout parity).
#ifndef _MSC_VER
static_assert(sizeof(FireBreathComponent) == 0x110, "sizeof must be 272 bytes");
#endif

} // namespace Caver

#endif
