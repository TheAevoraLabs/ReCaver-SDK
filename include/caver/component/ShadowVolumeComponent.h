#ifndef CAVER_SHADOWVOLUMECOMPONENT_H
#define CAVER_SHADOWVOLUMECOMPONENT_H

#include "Component.h"
#include "../core/Matrix4.h"
#include <boost/intrusive_ptr.hpp>

namespace Caver {

class ModelComponent;
class LightComponent;
class RenderingContext;

// Recovered from libswordigo.so (1.4.13) evidence — vtable _ZTVN5Caver21ShadowVolumeComponentE
// at 0x61AE30 (sizeof 0x88 = 136 bytes).
class ShadowVolumeComponent : public Component {
public:
    ShadowVolumeComponent();
    virtual ~ShadowVolumeComponent();

    static long Interface();

    virtual ShadowVolumeComponent* Clone() const override;
    void InitWithComponent(const ShadowVolumeComponent* other);

    virtual void LoadFromProtobufMessage(const Proto::Component& message) override;
    virtual void SaveToProtobufMessage(Proto::Component* message) const override;
    virtual bool ShouldSave() override;

    virtual void GetBindings(std::vector<Binding>* bindings) override;
    virtual void GetEnumValuesForBindedProperty(int property, std::vector<BindingValue>* values) override;
    virtual BindingValue ValueForBindedProperty(int property) override;
    virtual void SetValueForBindedProperty(int property, const BindingValue& value) override;
    virtual void PerformBindedAction(int action) override;

    virtual bool RequiresUpdate() const override;
    virtual void Update(float deltaTime) override;

    virtual bool ImplementsInterface(long interfaceId) override;
    virtual void RegisterInterfaces(ComponentManager* manager) override;
    virtual std::string className() const override;

    void SetModel(const boost::intrusive_ptr<ModelComponent>& model);
    void UpdateVolume();
    void Draw(RenderingContext* context, const boost::intrusive_ptr<LightComponent>& light,
              const Matrix4& matrix);

    boost::intrusive_ptr<ModelComponent> m_modelComponent; // +104 (0x68)
    boost::intrusive_ptr<LightComponent> m_lightComponent; // +112 (0x70)
    int m_vertexCount;                                     // +120 (0x78)
    char _pad124[4];                                       // +124
    float* m_vertices;                                     // +128 (0x80)
};

static_assert(sizeof(ShadowVolumeComponent) == 136, "ShadowVolumeComponent size must be 136 bytes");

} // namespace Caver

#endif // CAVER_SHADOWVOLUMECOMPONENT_H
