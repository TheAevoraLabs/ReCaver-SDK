// Recovered from libswordigo.so (1.4.13) evidence — vtable
// _ZTVN5Caver24PressureTriggerComponentE at 0x623BF0 (primary address
// point 0x623BF8, IBindable secondary 0x623D08, IUpdateable secondary 0x623D48).
//
// sizeof 0xF8 (248 bytes) is proven by:
//   operator new(0xF8u) in Create() @0x351048 and Clone() @0x3A3428.
#ifndef CAVER_PRESSURETRIGGERCOMPONENT_H
#define CAVER_PRESSURETRIGGERCOMPONENT_H

#include <string>
#include <vector>

#include "Component.h"
#include "../core/Vector2.h"
#include "../scripting/Program.h"

namespace Caver {

class SceneObject;

class PressureTriggerComponent : public Component {
public:
    PressureTriggerComponent();
    virtual ~PressureTriggerComponent();

    static PressureTriggerComponent* Create();
    static long Interface();

    virtual PressureTriggerComponent* Clone() const;
    void InitWithComponent(const PressureTriggerComponent* other);

    virtual void LoadFromProtobufMessage(const Proto::Component& message);
    virtual void SaveToProtobufMessage(Proto::Component* message) const;
    virtual bool ShouldSave();
    virtual void Prepare();

    virtual void GetBindings(std::vector<Binding>* bindings);
    virtual void GetEnumValuesForBindedProperty(int property, std::vector<BindingValue>* values);
    virtual BindingValue ValueForBindedProperty(int property);
    virtual void SetValueForBindedProperty(int property, const BindingValue& value);
    virtual void PerformBindedAction(int action);
    virtual int componentCategories() const;
    virtual bool ImplementsInterface(long interfaceId);
    virtual void RegisterInterfaces(ComponentManager* manager);
    virtual bool RequiresUpdate() const;
    virtual void Update(float deltaTime);
    virtual bool HandleMessage(int message, void* data);
    virtual std::string className() const;

    float m_timer;              // +104 (default FLT_MAX; reset to 0 on collision)
    float m_heightOffset;        // +108 (default 0.0f)
    float m_maxHeightOffset;     // +112 (default 5.0f)
    Vector2 m_originalPosition; // +116 (captured in Prepare)
    char _pad124[4];            // +124
    Program m_onPress;          // +128
    Program m_onRelease;        // +184
    bool m_stayPressed;         // +240 (default true)
    bool m_isPressed;           // +241 (default false)
    char _pad242[6];            // +242 (struct size 0xF8 = 248 bytes)
};

} // namespace Caver

#endif // CAVER_PRESSURETRIGGERCOMPONENT_H
