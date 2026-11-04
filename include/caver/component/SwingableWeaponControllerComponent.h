// Recovered from libswordigo.so (1.4.13) evidence — see the combat
// slice scratch board (SwingableWeaponControllerComponent/RESULTS.md)
// for per-function parity status. Weapon controller component;
// sizeof 0xC8 (proven by Create()'s operator new(0xC8)). Member
// offsets are proven from the ctor store map, ~D2's release order,
// InitWithComponent/Clone and the Load/Save protobuf field maps.
#ifndef CAVER_SWINGABLEWEAPONCONTROLLERCOMPONENT_H
#define CAVER_SWINGABLEWEAPONCONTROLLERCOMPONENT_H

#include "Component.h"
#include "ComponentOutlet.h"
#include "SwingableWeaponComponent.h"
#include <boost/intrusive_ptr.hpp>

namespace Caver {

struct BoneInstanceData;
struct Matrix4;
class SwingableWeaponControllerComponent;





class SwingableWeaponControllerComponent : public Component {
public:
    SwingableWeaponControllerComponent();
    virtual ~SwingableWeaponControllerComponent();

    static SwingableWeaponControllerComponent* Create();
    static long Interface();

    virtual Component* Clone() const;                                       // slot 2
    // NOT an override: vtable slot 3 keeps Component::InitWithComponent;
    // this is a non-virtual narrowed overload (mangling EPKS0_), the
    // HealthComponent pattern (SHARED.md).
    void InitWithComponent(const SwingableWeaponControllerComponent* other);
    virtual void LoadFromProtobufMessage(const Proto::Component& message);  // slot 4
    virtual void SaveToProtobufMessage(Proto::Component* message) const;    // slot 5
    virtual bool ShouldSave();                                              // slot 6 — true
    virtual void Prepare();                                                 // slot 8
    virtual void GetBindings(std::vector<Binding>* bindings);               // slot 14
    virtual void GetEnumValuesForBindedProperty(int property, std::vector<BindingValue>* values); // slot 15
    virtual BindingValue ValueForBindedProperty(int property);              // slot 16
    virtual void SetValueForBindedProperty(int property, const BindingValue& value); // slot 17
    virtual int componentCategories() const;                                // slot 19 — 2
    virtual bool ImplementsInterface(long interfaceId);                     // slot 20
    virtual void RegisterInterfaces(ComponentManager* manager);             // slot 21
    virtual void Update(float deltaTime);                                   // slot 25
    virtual bool HandleMessage(int message, void* data);                    // slot 26
    virtual std::string className() const;                                  // slot 32 — "SwingableWeaponController"

    // Non-virtual, recovered:
    BoneInstanceData* BoneForName(const std::string& name);
    void CreateWeapon();
    void SetWeaponComponent(const boost::intrusive_ptr<SwingableWeaponComponent>& weapon);
    void SetWeaponHidden(bool hidden);
    void StartSwing(int swing, float duration, float damage);
    void StopSwing();
    void KnockBack();
    void UpdateWeaponTransform();

    ComponentOutlet<ModelComponent> m_controllingModel; // +104 — BindOutlet(0, "controllingModel"); its identifier doubles as the proto controlling_model_id
    std::string m_controllingBoneName;                  // +128 — proto field 2
    std::string m_weaponTemplateName;                   // +152 — proto field 3
    boost::intrusive_ptr<SwingableWeaponComponent> m_weapon; // +176
    BoneInstanceData* m_bone;                           // +184 — cached bone, invalidated (set 0) by SetValueForBindedProperty
    int m_field192;                                     // +192 — last StartSwing index; KnockBack reads its sign (name UNRECOVERED)
    bool m_weaponHidden;                                // +196 — SetWeaponHidden's stored flag
    char _pad197[3];                                    // +197 — sizeof 0xC8
};

} // namespace Caver

#endif
