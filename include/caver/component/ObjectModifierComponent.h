#ifndef CAVER_OBJECTMODIFIERCOMPONENT_H
#define CAVER_OBJECTMODIFIERCOMPONENT_H

#include "Component.h"
#include "ModelComponent.h"
#include <boost/intrusive_ptr.hpp>

namespace Caver {

class ComponentManager;
class EntityInfoComponent;

enum ObjectModifierType {
    ObjectModifierTypeNone = 0,
    ObjectModifierTypeFreeze = 1
};

class ObjectModifierComponent : public Component {
public:
    ObjectModifierComponent();
    virtual ~ObjectModifierComponent();
    static ObjectModifierComponent* Create();
    static long Interface();
    virtual ObjectModifierComponent* Clone() const;
    void InitWithComponent(const ObjectModifierComponent* other);
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

    float ApplyModifier(ObjectModifierType type, float value, float duration);

    int m_modifierType; // +104
    float m_elapsed; // +108
    float m_duration; // +112
    float m_value; // +116
    boost::intrusive_ptr<ModelComponent> m_model; // +120
};

static_assert(sizeof(ObjectModifierComponent) == 0x80,
              "ObjectModifierComponent size must be 128 bytes");

} // namespace Caver

#endif
