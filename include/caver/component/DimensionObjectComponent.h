// Recovered from libswordigo.so (1.4.13) evidence — vtable
// _ZTVN5Caver24DimensionObjectComponentE at 0x61F5C0 (address point 0x61F5D0).
// sizeof 0x68 (104 bytes, identical to Component).
#ifndef CAVER_DIMENSIONOBJECTCOMPONENT_H
#define CAVER_DIMENSIONOBJECTCOMPONENT_H

#include <string>
#include "Component.h"

namespace Caver {

class DimensionObjectComponent : public Component {
public:
    DimensionObjectComponent();
    virtual ~DimensionObjectComponent();

    static DimensionObjectComponent* Create();
    static long Interface();

    virtual Component* Clone() const;
    void InitWithComponent(const DimensionObjectComponent* other);

    virtual void Prepare();
    virtual void LoadFromProtobufMessage(const Proto::Component& message);
    // 0x378F28: thunk to Component::SaveToProtobufMessage — this class
    // saves nothing of its own.
    virtual void SaveToProtobufMessage(Proto::Component* message) const;
    virtual void GetBindings(std::vector<Binding>* bindings);
    virtual void GetEnumValuesForBindedProperty(int property, std::vector<BindingValue>* values);
    virtual BindingValue ValueForBindedProperty(int property);
    virtual void SetValueForBindedProperty(int property, const BindingValue& value);
    virtual void PerformBindedAction(int action);
    virtual bool ShouldSave();

    virtual bool ImplementsInterface(long interfaceId);
    virtual void RegisterInterfaces(ComponentManager* manager);
    virtual std::string className() const;
};

static_assert(sizeof(DimensionObjectComponent) == 0x68,
              "DimensionObjectComponent sizeof must be 104 bytes");

} // namespace Caver

#endif // CAVER_DIMENSIONOBJECTCOMPONENT_H
