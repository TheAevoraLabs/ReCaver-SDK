#ifndef CAVER_SHATTERCOMPONENT_H
#define CAVER_SHATTERCOMPONENT_H

#include <string>
#include "Component.h"

namespace Caver {

class ModelComponent;

class ShatterComponent : public Component {
public:
    ShatterComponent();
    virtual ~ShatterComponent();

    static ShatterComponent* Create();
    static long Interface();

    virtual Component* Clone() const;
    void InitWithComponent(const ShatterComponent* other);
    virtual void LoadFromProtobufMessage(const Proto::Component& message);
    virtual void SaveToProtobufMessage(Proto::Component* message) const;
    virtual bool ShouldSave();
    virtual void GetBindings(std::vector<Binding>* bindings);
    virtual void GetEnumValuesForBindedProperty(
        int property, std::vector<BindingValue>* values);
    virtual BindingValue ValueForBindedProperty(int property);
    virtual void SetValueForBindedProperty(
        int property, const BindingValue& value);
    virtual void PerformBindedAction(int action);
    virtual bool ImplementsInterface(long interfaceId);
    virtual void RegisterInterfaces(ComponentManager* manager);
    virtual bool RequiresUpdate() const;
    virtual void Update(float deltaTime);

    virtual std::string className() const;

    void ShatterModel(ModelComponent* model, bool flag);
};

static_assert(sizeof(ShatterComponent) == 0x68,
              "ShatterComponent sizeof must be 104 bytes");

} // namespace Caver

#endif // CAVER_SHATTERCOMPONENT_H
