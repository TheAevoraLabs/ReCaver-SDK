#ifndef CAVER_MAGICSPELLCASTCOMPONENT_H
#define CAVER_MAGICSPELLCASTCOMPONENT_H

#include "Component.h"
#include "ComponentOutlet.h"
#include "ParticleEmitterComponent.h"
#include "SoundEffectComponent.h"

namespace Caver {

class MagicSpellCastComponent : public Component {
public:
    MagicSpellCastComponent();
    virtual ~MagicSpellCastComponent();
    static MagicSpellCastComponent* Create();
    static long Interface();

    virtual Component* Clone() const;
    // 0x380BA4: copies m_identifier (+32), m_label (+56) and the two
    // outlet identifiers inside m_particleEmitterOutlet (+112) and
    // m_soundEffectOutlet (+136).
    void InitWithComponent(const MagicSpellCastComponent* other);
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

    ComponentOutlet<ParticleEmitterComponent> m_particleEmitterOutlet; // +104
    ComponentOutlet<SoundEffectComponent> m_soundEffectOutlet;         // +128
    float m_particleTimer;                                              // +152
    char _pad156[4];
};

static_assert(sizeof(MagicSpellCastComponent) == 0xA0,
              "MagicSpellCastComponent sizeof must be 160 bytes");

} // namespace Caver

#endif
