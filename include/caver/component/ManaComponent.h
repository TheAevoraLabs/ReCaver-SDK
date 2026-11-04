#ifndef CAVER_MANACOMPONENT_H
#define CAVER_MANACOMPONENT_H

#include "Component.h"

namespace Caver {

class ManaComponent : public Component {
public:
    ManaComponent();
    virtual ~ManaComponent();

    static long Interface();
    virtual Component* Clone() const;
    using Component::InitWithComponent;
    void InitWithComponent(const ManaComponent* other);
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
    virtual void Update(float deltaTime);
    virtual int componentCategories() const;
    virtual bool ImplementsInterface(long interfaceId);
    virtual void RegisterInterfaces(ComponentManager* manager);
    virtual std::string className() const;

    void GainMana(int amount);

    int m_maxMana;          // +104
    int m_mana;             // +108
    int m_regenerationRate; // +112
    float m_regenerationTimer; // +116
    float m_eventTimer;     // +120
};

static_assert(sizeof(ManaComponent) == 0x80,
              "ManaComponent size must be 128 bytes");

} // namespace Caver

#endif
