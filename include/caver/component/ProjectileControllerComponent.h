#ifndef CAVER_PROJECTILECONTROLLERCOMPONENT_H
#define CAVER_PROJECTILECONTROLLERCOMPONENT_H

#include "Component.h"
#include "../physics/PhysicsObjectState.h"
#include <boost/intrusive_ptr.hpp>

namespace Caver {

class DamageComponent;
class ComponentManager;

class ProjectileControllerComponent : public Component {
public:
    ProjectileControllerComponent();
    virtual ~ProjectileControllerComponent();
    static ProjectileControllerComponent* Create();
    static long Interface();
    virtual ProjectileControllerComponent* Clone() const;
    void InitWithComponent(const ProjectileControllerComponent* other);
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
    virtual bool RequiresUpdate() const;
    virtual bool ImplementsInterface(long interfaceId);
    virtual void RegisterInterfaces(ComponentManager* manager);
    virtual std::string className() const;
    virtual void Update(float deltaTime);
    virtual bool HandleMessage(int message, void* data);

    void CreateBlast();

    bool m_alignObjectRotation; // +104
    bool m_breakOnGroundCollision; // +105
    char _pad106[6];
    boost::intrusive_ptr<DamageComponent> m_damage; // +112
};

static_assert(sizeof(ProjectileControllerComponent) == 0x78,
              "ProjectileControllerComponent size must be 120 bytes");

} // namespace Caver

#endif
