// Recovered from libswordigo.so (1.4.13) evidence — vtable
// _ZTVN5Caver22PhysicsObjectComponentE at 0x605240.
//
// sizeof 0x110 (272 bytes) proven by:
//   operator new(0x110u) in Create() @0x350888 and Clone() @0x3778AC.
//
// Primary vtable at 0x61F230, IBindable secondary at 0x61F348,
// IUpdateable secondary at 0x61F388.
#ifndef CAVER_PHYSICSOBJECTCOMPONENT_H
#define CAVER_PHYSICSOBJECTCOMPONENT_H

#include <vector>
#include <string>

#include "Component.h"
#include "../physics/PhysicsObjectState.h"
#include "../core/Binding.h"
#include "../core/BindingValue.h"

namespace Caver {

class CollisionShapeComponent;

class PhysicsObjectComponent : public Component {
public:
    PhysicsObjectComponent();
    virtual ~PhysicsObjectComponent();

    static PhysicsObjectComponent* Create();
    static long Interface();

    virtual PhysicsObjectComponent* Clone() const;
    void InitWithComponent(const PhysicsObjectComponent* other);

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
    virtual int componentCategories() const;
    virtual bool ImplementsInterface(long interfaceId);
    virtual void RegisterInterfaces(ComponentManager* manager);
    virtual bool RequiresUpdate() const;
    virtual void Update(float deltaTime);
    virtual bool HandleMessage(int message, void* data);
    virtual std::string className() const;

    void SetPhysicsEnabled(bool enabled);
    void HandleGroundCollision(const CollisionMessageData& data);
    void RegisterLibrary();

    // Layout past Component base (104 / 0x68 bytes):
    bool m_physicsEnabled;                 // +104 (byte, default true)
    char _pad105[7];                       // +105..+111 (align state to 8)
    PhysicsObjectState m_state;            // +112..+263 (152 bytes, embedded state)
    float m_groundDeceleration;            // +264 (*((float*)this + 66))
    bool m_groundContactState;             // +268 (*((_BYTE*)this + 268))
    char _pad269[3];                       // +269..+271 (tail pad to 0x110 = 272)
};

static_assert(sizeof(PhysicsObjectComponent) == 0x110,
              "PhysicsObjectComponent sizeof must be 272 bytes");

} // namespace Caver

#endif // CAVER_PHYSICSOBJECTCOMPONENT_H
