// Recovered from libswordigo.so (1.4.13) evidence.
// Attribution: Quantum Creeper / Aevora Labs.
#ifndef CAVER_SWINGCOMPONENT_H
#define CAVER_SWINGCOMPONENT_H

#include <vector>
#include <string>

#include "Component.h"
#include "ComponentOutlet.h"
#include "KeyframeAnimationComponent.h"

namespace Caver {

class SwingComponent : public Component {
public:
    SwingComponent();
    virtual ~SwingComponent();

    static SwingComponent* Create();
    static long Interface();

    virtual SwingComponent* Clone() const override;
    void InitWithComponent(const SwingComponent* other);

    virtual void LoadFromProtobufMessage(const Proto::Component& message) override;
    virtual void SaveToProtobufMessage(Proto::Component* message) const override;
    virtual bool ShouldSave() override;

    virtual void GetBindings(std::vector<Binding>* bindings) override;
    virtual void GetEnumValuesForBindedProperty(int property, std::vector<BindingValue>* values) override;
    virtual BindingValue ValueForBindedProperty(int property) override;
    virtual void SetValueForBindedProperty(int property, const BindingValue& value) override;
    virtual void PerformBindedAction(int action) override;

    virtual int componentCategories() const override;
    virtual bool ImplementsInterface(long interfaceId) override;
    virtual void RegisterInterfaces(ComponentManager* manager) override;
    virtual std::string className() const override;

    ComponentOutlet<KeyframeAnimationComponent> m_animationOutlet; // +104 (24 bytes)
    float m_startFrame;                                           // +128
    float m_endFrame;                                             // +132
    bool m_swingLeftWeapon;                                       // +136
    bool m_swingRightWeapon;                                      // +137
    char _pad138[6];                                              // +138
};

static_assert(sizeof(SwingComponent) == 0x90, "SwingComponent sizeof must be 144 bytes");

} // namespace Caver

#endif // CAVER_SWINGCOMPONENT_H
