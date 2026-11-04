#ifndef CAVER_ORBITCONTROLLERCOMPONENT_H
#define CAVER_ORBITCONTROLLERCOMPONENT_H

#include "Component.h"
#include "ComponentOutlet.h"
#include "../core/Vector3.h"
#include <boost/intrusive_ptr.hpp>

namespace Caver {

class SceneObject;

class OrbitControllerComponent : public Component {
public:
    OrbitControllerComponent();
    virtual ~OrbitControllerComponent();
    static OrbitControllerComponent* Create();
    // Golden symbol _ZN5Caver24OrbitControllerComponent9InterfaceEv
    // @0x395A34 (weak, COMDAT): the interface token is the function's
    // own address, the same idiom as every other component. Declared
    // by the HeroEquipmentManager slice, whose ApplyArmorTrinket
    // @0x43E248 looks the spawned wisps' controller up through it.
    static long Interface();
    // 0x39594C: allocate 0xB8, run the ctor, then copy m_identifier
    // (+32), m_label (+56), the rotation axis and speed (one 16-byte
    // run at +116) and m_orbitDistance (+132).
    virtual Component* Clone() const;
    // 0x3959BC: this class's interface only - no base-class token.
    virtual bool ImplementsInterface(long interfaceId);
    // 0x3959D0: register this interface, then Component's own list.
    virtual void RegisterInterfaces(ComponentManager* manager);
    void InitWithComponent(const OrbitControllerComponent* other);
    virtual void LoadFromProtobufMessage(const Proto::Component& message);
    virtual void SaveToProtobufMessage(Proto::Component* message) const;
    virtual bool ShouldSave();
    virtual void GetBindings(std::vector<Binding>* bindings);
    virtual void GetEnumValuesForBindedProperty(int property, std::vector<BindingValue>* values);
    virtual BindingValue ValueForBindedProperty(int property);
    virtual void SetValueForBindedProperty(int property, const BindingValue& value);
    virtual void PerformBindedAction(int action);
    virtual void Prepare();
    virtual bool RequiresUpdate() const;
    virtual void Update(float deltaTime);
    virtual std::string className() const;
    void FollowObject(const boost::intrusive_ptr<SceneObject>& object);

    boost::intrusive_ptr<SceneObject> m_followObject; // +104
    float m_rotation;                                 // +112
    Vector3 m_rotationAxis;                           // +116
    float m_rotationSpeed;                            // +128
    float m_orbitDistance;                            // +132
    Vector3 m_targetPosition;                         // +136
    float m_angleOffset;                              // +148
    float m_angleSpread;                              // +152
    float m_angleX;                                   // +156
    float m_angleY;                                   // +160
    float m_heightPhase;                              // +164
    float m_angleRateX;                               // +168
    float m_angleRateY;                               // +172
    float m_heightRate;                               // +176
};

static_assert(sizeof(OrbitControllerComponent) == 0xB8, "sizeof must be 184 bytes");

} // namespace Caver

#endif
