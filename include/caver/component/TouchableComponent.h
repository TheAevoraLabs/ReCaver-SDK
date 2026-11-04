#ifndef CAVER_TOUCHABLECOMPONENT_H
#define CAVER_TOUCHABLECOMPONENT_H

#include "Component.h"
#include "../scripting/Program.h"
#include <string>
#include <vector>

namespace Caver {

struct FWTouch;
class SceneObject;

// Recovered from libswordigo.so (1.4.13) evidence — vtable _ZTVN5Caver18TouchableComponentE
// at 0x61BA78 (sizeof 0xB0 = 176 bytes).
class TouchableComponent : public Component {
public:
    TouchableComponent();
    virtual ~TouchableComponent();

    static TouchableComponent* Create();
    static long Interface();

    virtual TouchableComponent* Clone() const;
    void InitWithComponent(const TouchableComponent* other);

    virtual void LoadFromProtobufMessage(const Proto::Component& message);
    virtual void SaveToProtobufMessage(Proto::Component* message) const;
    virtual bool ShouldSave();
    virtual int componentCategories() const;
    virtual void Prepare();
    void RegisterLibrary();

    virtual void GetBindings(std::vector<Binding>* bindings);
    virtual void GetEnumValuesForBindedProperty(int property, std::vector<BindingValue>* values);
    virtual BindingValue ValueForBindedProperty(int property);
    virtual void SetValueForBindedProperty(int property, const BindingValue& value);
    virtual void PerformBindedAction(int action);
    virtual bool ImplementsInterface(long interfaceId);
    virtual void RegisterInterfaces(ComponentManager* manager);
    virtual std::string className() const;

    void TouchBegan(const FWTouch& touch);
    void Trigger();

    float m_touchRadius;          // +104 (default 90.0f)
    SceneObject* m_target;        // +112 (default nullptr)
    Program m_onTouch;            // +120
};

} // namespace Caver

#endif // CAVER_TOUCHABLECOMPONENT_H
