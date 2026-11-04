#ifndef CAVER_BLENDANIMATIONCOMPONENT_H
#define CAVER_BLENDANIMATIONCOMPONENT_H

#include <vector>
#include <boost/intrusive_ptr.hpp>

#include "Component.h"
#include "caver/animation/AnimBlendNode.h"

namespace Caver {

class AnimationComponent;
class ComponentManager;

class BlendAnimationComponent : public Component {
public:
    BlendAnimationComponent();
    virtual ~BlendAnimationComponent();

    static BlendAnimationComponent* Create();
    static long Interface();

    virtual BlendAnimationComponent* Clone() const;
    // NOT an override: narrowed non-virtual overload (EPKS0_), as the
    // golden mangles it — vtable slot 3 keeps
    // Component::InitWithComponent, the pattern used by every other
    // component in the binary.
    void InitWithComponent(const BlendAnimationComponent* other);
    virtual void LoadFromProtobufMessage(const Proto::Component& message);
    virtual void SaveToProtobufMessage(Proto::Component* message) const;
    void ConnectOutlets();
    virtual void GetBindings(std::vector<Binding>* bindings);
    virtual void GetEnumValuesForBindedProperty(int property, std::vector<BindingValue>* values);
    virtual BindingValue ValueForBindedProperty(int property);
    virtual void SetValueForBindedProperty(int property, const BindingValue& value);
    virtual bool ShouldSave();
    virtual int componentCategories() const;
    virtual bool ImplementsInterface(long interfaceId);
    virtual void RegisterInterfaces(ComponentManager* manager);
    virtual std::string className() const;
    virtual AnimBlendNode* animationNode();

    void setAnimation1(AnimationComponent* animation);
    void setAnimation2(AnimationComponent* animation);

    bool m_outletsConnected;                         // +104
    char _pad105[7];                                 // +105
    boost::intrusive_ptr<AnimBlendNode> m_animationNode; // +112
    int m_animation1Identifier;                      // +120
    char _pad124[4];                                 // +124
    boost::intrusive_ptr<Component> m_animation1;    // +128
    int m_animation2Identifier;                      // +136
    char _pad140[4];                                 // +140
    boost::intrusive_ptr<Component> m_animation2;    // +144
};

static_assert(sizeof(BlendAnimationComponent) == 0x98, "BlendAnimationComponent sizeof must be 152 bytes");

} // namespace Caver

#endif
