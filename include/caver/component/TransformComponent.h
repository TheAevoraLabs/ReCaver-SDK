#ifndef CAVER_TRANSFORMCOMPONENT_H
#define CAVER_TRANSFORMCOMPONENT_H

#include "Component.h"
#include "../core/Matrix4.h"

namespace Caver {

// Recovered from libswordigo.so (1.4.13) evidence — vtable _ZTVN5Caver18TransformComponentE
// at 0x61BC28 (sizeof 0xA8 = 168 bytes).
class TransformComponent : public Component {
public:
    TransformComponent();
    virtual ~TransformComponent();

    static TransformComponent* Create();
    static long Interface();

    virtual TransformComponent* Clone() const override;
    void InitWithComponent(const TransformComponent* other);

    virtual void LoadFromProtobufMessage(const Proto::Component& message) override;
    virtual void SaveToProtobufMessage(Proto::Component* message) const override;
    virtual bool ShouldSave() override;

    virtual void GetBindings(std::vector<Binding>* bindings) override;
    virtual void GetEnumValuesForBindedProperty(int property, std::vector<BindingValue>* values) override;
    virtual BindingValue ValueForBindedProperty(int property) override;
    virtual void SetValueForBindedProperty(int property, const BindingValue& value) override;
    virtual void PerformBindedAction(int action) override;

    virtual bool ImplementsInterface(long interfaceId) override;
    virtual void RegisterInterfaces(ComponentManager* manager) override;
    virtual std::string className() const override;

    Matrix4 WorldMatrix();

    Matrix4 m_matrix; // +104 (0x68..0xA7)
};

static_assert(sizeof(TransformComponent) == 168, "TransformComponent size must be 168 bytes");

} // namespace Caver

#endif // CAVER_TRANSFORMCOMPONENT_H
