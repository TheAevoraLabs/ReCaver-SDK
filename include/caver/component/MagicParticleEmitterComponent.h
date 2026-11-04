// MagicParticleEmitterComponent.h
// TouchFoo CaverEngine — recovered from libswordigo.so (1.4.13) evidence.
// Attribution: Quantum Creeper / Aevora Labs.

#ifndef CAVER_MAGICPARTICLEEMITTERCOMPONENT_H
#define CAVER_MAGICPARTICLEEMITTERCOMPONENT_H

#include "ParticleEmitterComponent.h"

namespace Caver {

class MagicParticleEmitterComponent : public ParticleEmitterComponent {
public:
    MagicParticleEmitterComponent();
    virtual ~MagicParticleEmitterComponent();

    static MagicParticleEmitterComponent* Create();
    static long Interface();

    void InitWithComponent(const MagicParticleEmitterComponent* other);

    virtual void LoadFromProtobufMessage(const Proto::Component& proto) override;
    virtual void SaveToProtobufMessage(Proto::Component* proto) const override;
    virtual void GetBindings(std::vector<Binding>* bindings) override;
    virtual void GetEnumValuesForBindedProperty(int property, std::vector<BindingValue>* values) override;
    virtual BindingValue ValueForBindedProperty(int property) override;
    virtual void SetValueForBindedProperty(int property, const BindingValue& value) override;
    virtual void PerformBindedAction(int action) override;

    virtual void Update(float dt) override;
    virtual MagicParticleEmitterComponent* Clone() const override;
    virtual bool ShouldSave() override;
    virtual bool ImplementsInterface(long iface) override;
    virtual void RegisterInterfaces(ComponentManager* manager) override;
    virtual std::string className() const override;

    float m_time; // +464 (0x1D0)
};

static_assert(sizeof(MagicParticleEmitterComponent) == 0x1D8,
              "sizeof(MagicParticleEmitterComponent) must be 0x1D8");

} // namespace Caver

#endif // CAVER_MAGICPARTICLEEMITTERCOMPONENT_H
