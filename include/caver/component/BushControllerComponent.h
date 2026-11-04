// Recovered from libswordigo.so (1.4.13) evidence — vtable
// _ZTVN5Caver23BushControllerComponentE at 0x6236D0 (primary address
// point 0x6236E0, IBindable secondary 0x6237F8, IUpdateable secondary
// 0x623838): 33 primary slots, no extra virtuals beyond Component's.
//
// sizeof 0xB8 (184 bytes) is proven by the two allocation sites,
// operator new(0xB8u) in Create() @0x3502D8 and in Clone() @0x39FE1C.
//
// Member map, from the ctor store sequence @0x39F150, the destructor
// teardown order @0x39FC10, InitWithComponent @0x39F3C4 and the outlet
// vtable stores:
//   +104 ComponentOutlet<KeyframeAnimationComponent> wobbleAnimation
//        (vtable off_61D8C0 = _ZTV ..ComponentOutlet<KeyframeAnimationComponent>)
//   +128 ComponentOutlet<SoundEffectComponent> wobbleSound (off_61DB90)
//   +152 ComponentOutlet<SoundEffectComponent> cutSound    (off_61DB90)
//   +176 bool, set once the bush has been cut down (HandleMessage
//        guard and Update early-out; name descriptive only)
#ifndef CAVER_BUSHCONTROLLERCOMPONENT_H
#define CAVER_BUSHCONTROLLERCOMPONENT_H

#include <string>
#include <vector>

#include "Component.h"
#include "ComponentOutlet.h"
#include "KeyframeAnimationComponent.h"
#include "SoundEffectComponent.h"

namespace Caver {

class BushControllerComponent : public Component {
public:
    BushControllerComponent();
    virtual ~BushControllerComponent();

    static BushControllerComponent* Create();
    static long Interface();

    virtual BushControllerComponent* Clone() const;
    void InitWithComponent(const BushControllerComponent* other);

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

    ComponentOutlet<KeyframeAnimationComponent> m_wobbleAnimation; // +104
    ComponentOutlet<SoundEffectComponent> m_wobbleSound;          // +128
    ComponentOutlet<SoundEffectComponent> m_cutSound;             // +152
    bool m_cut;                                                   // +176
    char _pad177[7];                                              // +177
};

static_assert(sizeof(BushControllerComponent) == 0xB8,
              "BushControllerComponent sizeof must be 184 bytes");

} // namespace Caver

#endif // CAVER_BUSHCONTROLLERCOMPONENT_H
