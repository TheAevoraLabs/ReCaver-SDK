#ifndef CAVER_MODELTRANSFORMCONTROLLERCOMPONENT_H
#define CAVER_MODELTRANSFORMCONTROLLERCOMPONENT_H

#include "Component.h"
#include "ComponentOutlet.h"
#include "ModelComponent.h"
#include "../core/Vector3.h"

namespace Caver {

class ComponentManager;

class ModelTransformControllerComponent : public Component {
public:
    ModelTransformControllerComponent();
    virtual ~ModelTransformControllerComponent();
    static ModelTransformControllerComponent* Create();
    static long Interface();
    virtual ModelTransformControllerComponent* Clone() const;
    void InitWithComponent(const ModelTransformControllerComponent* other);
    virtual void LoadFromProtobufMessage(const Proto::Component& message);
    virtual void SaveToProtobufMessage(Proto::Component* message) const;
    virtual bool ShouldSave();
    virtual void GetBindings(std::vector<Binding>* bindings);
    virtual void GetEnumValuesForBindedProperty(int property,
                                                std::vector<BindingValue>* values);
    virtual BindingValue ValueForBindedProperty(int property);
    virtual void SetValueForBindedProperty(int property,
                                           const BindingValue& value);
    virtual void PerformBindedAction(int action);
    virtual void Prepare();
    void RegisterLibrary();
    virtual bool RequiresUpdate() const;
    virtual bool UpdateWhenPaused() const;
    virtual void Update(float deltaTime);
    virtual bool ImplementsInterface(long interfaceId);
    virtual void RegisterInterfaces(ComponentManager* manager);
    virtual std::string className() const;

    ComponentOutlet<ModelComponent> m_model; // +104
    bool m_transformDirty; // +128
    char _pad129[3];
    Vector3 m_origin; // +132
    Vector3 m_rotationAxis; // +144
    float m_rotationAngle; // +156
    float m_rotationSpeed; // +160
    char _pad164[4];
};

static_assert(sizeof(ModelTransformControllerComponent) == 0xA8,
              "ModelTransformControllerComponent size must be 168 bytes");

} // namespace Caver

#endif
