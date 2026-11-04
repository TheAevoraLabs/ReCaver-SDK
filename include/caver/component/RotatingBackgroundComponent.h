#ifndef CAVER_ROTATINGBACKGROUNDCOMPONENT_H
#define CAVER_ROTATINGBACKGROUNDCOMPONENT_H

#include "BackgroundComponent.h"

namespace Caver {

class RotatingBackgroundComponent : public BackgroundComponent {
public:
    RotatingBackgroundComponent();
    virtual ~RotatingBackgroundComponent();

    static RotatingBackgroundComponent* Create();
    static long Interface();
    virtual Component* Clone() const;
    using BackgroundComponent::InitWithComponent;
    void InitWithComponent(const RotatingBackgroundComponent* other);

    // The five protobuf/binding entry points are thunks straight to
    // BackgroundComponent (0x3A3550..0x3A357C, every body is a single
    // tail branch): the rotating background adds no fields of its own to
    // the saved message or the binding set.
    virtual void LoadFromProtobufMessage(const Proto::Component& message);
    virtual void SaveToProtobufMessage(Proto::Component* message) const;
    virtual BindingValue ValueForBindedProperty(int property);
    virtual void SetValueForBindedProperty(int property,
                                           const BindingValue& value);
    virtual void GetBindings(std::vector<Binding>* bindings);

    // 0x3A3934 returns 1, 0x3A39A4 returns 1.
    virtual bool ShouldSave();
    virtual bool RequiresUpdate() const;

    // 0x3A393C / 0x3A395C / 0x3A39D4: this class's own interface plus
    // BackgroundComponent's - Component's list is NOT walked.
    virtual bool ImplementsInterface(long interfaceId);
    virtual void RegisterInterfaces(ComponentManager* manager);

    virtual void Update(float deltaTime);
    virtual void Draw(RenderingContext* context, Camera* camera) const;
    virtual std::string className() const;

    float m_rotation; // +136, radians
};

static_assert(sizeof(RotatingBackgroundComponent) == 0x90,
              "RotatingBackgroundComponent size must be 144 bytes");

} // namespace Caver

#endif
