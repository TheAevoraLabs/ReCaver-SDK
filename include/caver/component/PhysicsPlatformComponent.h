// Recovered from libswordigo.so (1.4.13) evidence.
// Attribution: Quantum Creeper / Aevora Labs.
#ifndef CAVER_PHYSICSPLATFORMCOMPONENT_H
#define CAVER_PHYSICSPLATFORMCOMPONENT_H

#include <vector>
#include <string>

#include "Component.h"
#include "../core/Vector2.h"

namespace Caver {

struct CollisionMessageData;

class PhysicsPlatformComponent : public Component {
public:
    PhysicsPlatformComponent();
    virtual ~PhysicsPlatformComponent();

    static PhysicsPlatformComponent* Create();
    static long Interface();

    virtual PhysicsPlatformComponent* Clone() const override;
    void InitWithComponent(const PhysicsPlatformComponent* other);

    virtual void LoadFromProtobufMessage(const Proto::Component& message) override;
    virtual void SaveToProtobufMessage(Proto::Component* message) const override;
    virtual bool ShouldSave() override;
    virtual void Prepare() override;

    virtual void GetBindings(std::vector<Binding>* bindings) override;
    virtual void GetEnumValuesForBindedProperty(int property, std::vector<BindingValue>* values) override;
    virtual BindingValue ValueForBindedProperty(int property) override;
    virtual void SetValueForBindedProperty(int property, const BindingValue& value) override;
    virtual void PerformBindedAction(int action) override;

    virtual bool ImplementsInterface(long interfaceId) override;
    virtual void RegisterInterfaces(ComponentManager* manager) override;
    virtual bool RequiresUpdate() const override;
    virtual void Update(float deltaTime) override;
    virtual bool HandleMessage(int message, void* data) override;
    virtual std::string className() const override;

    void HandleObjectCollision(const CollisionMessageData& message);

    int m_mode;                    // +104
    Vector2 m_force;               // +108
    float m_collisionTimer;        // +116
    float m_torque;                // +120
    int _pad124;                   // +124
    Vector2 m_initialPosition;     // +128
    float m_initialRotation;       // +136
    float m_mass;                  // +140
    float m_springForce;           // +144
    float m_decelerationForce;     // +148
    float m_minSpeed;              // +152
    char _pad156[4];               // +156
};

static_assert(sizeof(PhysicsPlatformComponent) == 0xA0, "PhysicsPlatformComponent sizeof must be 160 bytes");

} // namespace Caver

#endif // CAVER_PHYSICSPLATFORMCOMPONENT_H
