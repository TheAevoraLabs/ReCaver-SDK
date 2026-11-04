// Recovered from libswordigo.so (1.4.13) evidence — vtable
// _ZTVN5Caver23DoorControllerComponentE at 0x623868 (primary address
// point 0x623870, IBindable secondary 0x623988, IUpdateable secondary 0x6239C8).
//
// sizeof 0xD0 (208 bytes) is proven by:
//   operator new(0xD0u) in Create() @0x351088 and Clone() @0x3A0EA0.
#ifndef CAVER_DOORCONTROLLERCOMPONENT_H
#define CAVER_DOORCONTROLLERCOMPONENT_H

#include <string>
#include <vector>

#include "Component.h"
#include "ComponentOutlet.h"
#include "AnimationControllerComponent.h"
#include "KeyframeAnimationComponent.h"
#include "SoundEffectComponent.h"

namespace Caver {

class DoorControllerComponent : public Component {
public:
    DoorControllerComponent();
    virtual ~DoorControllerComponent();

    static DoorControllerComponent* Create();
    static long Interface();

    virtual DoorControllerComponent* Clone() const;
    void InitWithComponent(const DoorControllerComponent* other);

    virtual void LoadFromProtobufMessage(const Proto::Component& message);
    virtual void SaveToProtobufMessage(Proto::Component* message) const;
    virtual bool ShouldSave();
    virtual void Prepare();
    virtual void RegisterLibrary();

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
    virtual std::string className() const;

    void Open();
    void Close();
    void SetOpen(bool open);
    void SetCollisionEnabled(bool enabled);

    ComponentOutlet<AnimationControllerComponent> m_animationController; // +104
    ComponentOutlet<KeyframeAnimationComponent> m_animation;             // +128
    ComponentOutlet<SoundEffectComponent> m_closeSound;                  // +152
    ComponentOutlet<SoundEffectComponent> m_openSound;                   // +176
    bool m_open;                                                         // +200
    char _pad[7];                                                        // +201..+207
};

} // namespace Caver

#endif // CAVER_DOORCONTROLLERCOMPONENT_H
