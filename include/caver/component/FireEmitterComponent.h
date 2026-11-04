// FireEmitterComponent.h
// TouchFoo CaverEngine — recovered from libswordigo.so (1.4.13) evidence.
// Attribution: Quantum Creeper / Aevora Labs.

#ifndef CAVER_FIREEMITTERCOMPONENT_H
#define CAVER_FIREEMITTERCOMPONENT_H

#include "Component.h"
#include "ComponentOutlet.h"
#include "LightComponent.h"
#include "ParticleEmitterComponent.h"
#include "../core/FloatColor.h"
#include "../core/Vector3.h"

namespace Caver {

class FireEmitterComponent : public Component {
public:
    FireEmitterComponent();
    virtual ~FireEmitterComponent();

    static FireEmitterComponent* Create();
    static long Interface();

    void InitWithComponent(const FireEmitterComponent* other);

    virtual void LoadFromProtobufMessage(const Proto::Component& proto) override;
    virtual void SaveToProtobufMessage(Proto::Component* proto) const override;
    virtual bool ShouldSave() override;
    virtual void Prepare() override;
    virtual void GetBindings(std::vector<Binding>* bindings) override;
    virtual void GetEnumValuesForBindedProperty(int property, std::vector<BindingValue>* values) override;
    virtual BindingValue ValueForBindedProperty(int property) override;
    virtual void SetValueForBindedProperty(int property, const BindingValue& value) override;
    virtual void PerformBindedAction(int action) override;

    virtual bool RequiresUpdate() const override;
    virtual void Update(float dt) override;
    virtual FireEmitterComponent* Clone() const override;
    virtual bool ImplementsInterface(long iface) override;
    virtual void RegisterInterfaces(ComponentManager* manager) override;
    virtual std::string className() const override;

    ComponentOutlet<ParticleEmitterComponent> m_particleEmitterOutlet; // +104
    ComponentOutlet<LightComponent> m_lightOutlet;                     // +128
    float m_particleInterval;                                          // +152
    float m_particleMaxAge;                                            // +156
    Vector3 m_particleSpread;                                          // +160
    Vector3 m_origin;                                                  // +172
    FloatColor m_color;                                                // +184
    Vector3 m_hsl;                                                     // +200
    float m_time;                                                      // +212
    float m_baseLightIntensity;                                        // +216
    float m_flicker;                                                   // +220
    float m_flickerTarget;                                             // +224
    float m_flickerSpeed;                                              // +228
};

static_assert(sizeof(FireEmitterComponent) == 0xE8,
              "sizeof(FireEmitterComponent) must be 0xE8");

} // namespace Caver

#endif // CAVER_FIREEMITTERCOMPONENT_H
