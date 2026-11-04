#ifndef CAVER_MAGICBOLTCOMPONENT_H
#define CAVER_MAGICBOLTCOMPONENT_H

#include "ComponentOutlet.h"
#include "ParticleEmitterComponent.h"
#include "SoundEffectComponent.h"
#include "SpellComponent.h"
#include "../core/FloatColor.h"
#include "../core/Vector3.h"

namespace Caver {

class MagicBoltComponent : public SpellComponent {
public:
    MagicBoltComponent();
    virtual ~MagicBoltComponent();
    static MagicBoltComponent* Create();
    static long Interface();

    virtual Component* Clone() const;
    void InitWithComponent(const MagicBoltComponent* other);
    virtual void LoadFromProtobufMessage(const Proto::Component& message);
    virtual void SaveToProtobufMessage(Proto::Component* message) const;
    virtual bool ShouldSave();
    virtual void GetBindings(std::vector<Binding>* bindings);
    virtual void GetEnumValuesForBindedProperty(int property, std::vector<BindingValue>* values);
    virtual BindingValue ValueForBindedProperty(int property);
    virtual void SetValueForBindedProperty(int property, const BindingValue& value);
    virtual void PerformBindedAction(int action);
    virtual void Prepare();
    virtual bool ImplementsInterface(long interfaceId);
    virtual void RegisterInterfaces(ComponentManager* manager);
    virtual bool RequiresUpdate() const;
    virtual std::string className() const;
    virtual void Cast();
    virtual void Update(float deltaTime);
    virtual bool HandleMessage(int message, void* data);

    void Explode(bool hit);

    bool m_active;                                                    // +192
    char _pad193[7];
    ComponentOutlet<ParticleEmitterComponent> m_particleEmitterOutlet; // +200
    ComponentOutlet<SoundEffectComponent> m_swooshSoundOutlet;         // +224
    ComponentOutlet<SoundEffectComponent> m_hitSoundOutlet;            // +248
    FloatColor m_color;                                                 // +272
    float m_speed;                                                      // +288
    Vector3 m_direction;                                                // +292
    float m_particleTimer;                                              // +304
    float m_trailTimer;                                                 // +308
    bool m_hasPhysicsObject;                                            // +312
    bool m_soundStarted;                                                // +313
    char _pad314[6];
};

// Golden aarch64 NDK-libc++ layout anchor; MSVC STL member sizes differ,
// so this assert is Android/Linux-only (the Windows build is self-consistent
// under MSVC STL and does not claim golden byte-layout parity).
#ifndef _MSC_VER
static_assert(sizeof(MagicBoltComponent) == 0x140, "sizeof must be 320 bytes");
#endif

} // namespace Caver

#endif
