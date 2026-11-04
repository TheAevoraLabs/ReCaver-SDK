#ifndef CAVER_MAGICHOOKSHOTCOMPONENT_H
#define CAVER_MAGICHOOKSHOTCOMPONENT_H

#include "SpellComponent.h"
#include "ComponentOutlet.h"
#include "DamageComponent.h"
#include "ParticleEmitterComponent.h"
#include "SoundEffectComponent.h"
#include "../core/FloatColor.h"
#include <boost/intrusive_ptr.hpp>

namespace Caver {

class MagicHookshotComponent : public SpellComponent {
public:
    MagicHookshotComponent();
    virtual ~MagicHookshotComponent();
    static MagicHookshotComponent* Create();
    static long Interface();

    virtual Component* Clone() const;
    void InitWithComponent(const MagicHookshotComponent* other);
    virtual void LoadFromProtobufMessage(const Proto::Component& message);
    virtual void SaveToProtobufMessage(Proto::Component* message) const;
    virtual bool ShouldSave();
    virtual void Prepare();
    virtual void GetBindings(std::vector<Binding>* bindings);
    virtual void GetEnumValuesForBindedProperty(int property,
                                                std::vector<BindingValue>* values);
    virtual BindingValue ValueForBindedProperty(int property);
    virtual void SetValueForBindedProperty(int property,
                                           const BindingValue& value);
    virtual void PerformBindedAction(int action);
    virtual bool ImplementsInterface(long interfaceId);
    virtual void RegisterInterfaces(ComponentManager* manager);
    virtual bool RequiresUpdate() const;
    virtual void Update(float deltaTime);
    virtual bool HandleMessage(int message, void* data);
    virtual std::string className() const;
    virtual void Cast();

    boost::intrusive_ptr<SceneObject> FindGrabbableObject();
    void Destroy();
    void CreateBlast();

    bool m_active;                                                        // +192
    char _pad193[7];
    ComponentOutlet<ParticleEmitterComponent> m_particleEmitterOutlet;   // +200
    ComponentOutlet<SoundEffectComponent> m_swooshSoundOutlet;           // +224
    ComponentOutlet<SoundEffectComponent> m_hitSoundOutlet;              // +248
    ComponentOutlet<SoundEffectComponent> m_groundHitSoundOutlet;         // +272
    boost::intrusive_ptr<Component> m_trailComponent;                     // +296
    FloatColor m_color;                                                   // +304
    boost::intrusive_ptr<DamageComponent> m_damageComponent;              // +320
    char _pad328[4];
    bool m_groundLatched;                                                 // +332
    bool m_objectLatched;                                                 // +333
    char _pad334[2];
    boost::intrusive_ptr<SceneObject> m_grabbableObject;                  // +336
    Vector2 m_grabOffset;                                                 // +344
    bool m_destroyed;                                                     // +352
    char _pad353[3];
    float m_latchedDistance;                                              // +356
    float m_elapsed;                                                      // +360
    float m_springVelocity;                                               // +364
    float m_currentDistance;                                              // +368
    float m_pullSpeed;                                                    // +372
};

// Golden aarch64 NDK-libc++ layout anchor; MSVC STL member sizes differ,
// so this assert is Android/Linux-only (the Windows build is self-consistent
// under MSVC STL and does not claim golden byte-layout parity).
#ifndef _MSC_VER
static_assert(sizeof(MagicHookshotComponent) == 0x178,
              "MagicHookshotComponent sizeof must be 376 bytes");
#endif

} // namespace Caver

#endif
