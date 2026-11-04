#ifndef CAVER_PARTICLEOBJECTCOMPONENT_H
#define CAVER_PARTICLEOBJECTCOMPONENT_H

#include "Component.h"
#include "ComponentOutlet.h"
#include "ParticleEmitterComponent.h"
#include "../physics/PhysicsObjectState.h"

namespace Caver {

class ParticleObjectComponent : public Component {
public:
    ParticleObjectComponent();
    virtual ~ParticleObjectComponent();

    static ParticleObjectComponent* Create();
    static long Interface();

    virtual Component* Clone() const;
    void InitWithComponent(const ParticleObjectComponent* other);
    virtual void LoadFromProtobufMessage(const Proto::Component& message);
    virtual void SaveToProtobufMessage(Proto::Component* message) const;
    virtual bool ShouldSave();
    virtual void Prepare();
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
    virtual bool HandleMessage(int message, void* data);
    virtual std::string className() const;

    void HandleGroundCollision(const CollisionMessageData& data);

    ComponentOutlet<ModelComponent> m_modelOutlet; // +104
    PhysicsObjectState m_physicsState;             // +128
};

static_assert(sizeof(ParticleObjectComponent) == 0x118,
              "ParticleObjectComponent size must be 280 bytes");

} // namespace Caver

#endif
