// Recovered from libswordigo.so (1.4.13) evidence — vtable
// _ZTVN5Caver28AnimationControllerComponentE at 0x619370 (0x178 bytes:
// primary address point 0x619370, IBindable secondary 0x619490,
// IUpdateable secondary 0x6194D0).
//
// sizeof 0xB0 (176 bytes) is proven by:
//   operator new(0xB0u) in Create() @0x350498 and Clone() @0x32D440.
#ifndef CAVER_ANIMATIONCONTROLLERCOMPONENT_H
#define CAVER_ANIMATIONCONTROLLERCOMPONENT_H

#include <string>
#include <vector>
#include <boost/intrusive_ptr.hpp>

#include "Component.h"
#include "ComponentOutlet.h"
#include "KeyframeAnimationComponent.h"
#include "ModelComponent.h"
#include "../animation/AnimKeysNode.h"

namespace Caver {

class AnimBlendNode;

class AnimationControllerComponent : public Component {
public:
    AnimationControllerComponent();
    virtual ~AnimationControllerComponent();

    static AnimationControllerComponent* Create();
    static long Interface();

    virtual Component* Clone() const;
    void InitWithComponent(const AnimationControllerComponent* other);

    virtual void LoadFromProtobufMessage(const Proto::Component& message);
    virtual void SaveToProtobufMessage(Proto::Component* message) const;
    virtual bool ShouldSave();
    virtual void Prepare();
    virtual void RegisterLibrary();

    virtual void GetBindings(std::vector<Binding>* bindings);
    virtual void GetEnumValuesForBindedProperty(int property, std::vector<BindingValue>* values);
    virtual BindingValue ValueForBindedProperty(int property);
    virtual void SetValueForBindedProperty(int property, const BindingValue& value);
    virtual void PerformBindedAction(int action);

    virtual int componentCategories() const;
    virtual bool ImplementsInterface(long interfaceId);
    virtual void RegisterInterfaces(ComponentManager* manager);
    virtual bool RequiresUpdate() const;
    virtual void Update(float deltaTime);
    virtual std::string className() const;

    virtual void BlendToAnimationNode(const boost::intrusive_ptr<AnimNode>& node, float duration); // primary vtable slot 33 (byte offset 264)
    void SetCurrentAnimation(const boost::intrusive_ptr<AnimKeysNode>& anim, float duration) {
        BlendToAnimationNode(boost::intrusive_ptr<AnimNode>(anim.get()), duration);
    }

    ComponentOutlet<ModelComponent> m_modelOutlet;                       // +104 (24 bytes)
    ComponentOutlet<KeyframeAnimationComponent> m_defaultAnimationOutlet; // +128 (24 bytes)
    union {
        boost::intrusive_ptr<AnimNode> m_currentNode;                        // +152 (8 bytes)
        boost::intrusive_ptr<AnimKeysNode> m_currentAnimation;
    };
    bool m_selfUpdate;                                                   // +160 (1 byte)
    // +161..+167 padding (7 bytes)
    boost::intrusive_ptr<AnimNode> m_currentBlend;                       // +168 (8 bytes)
};

static_assert(sizeof(AnimationControllerComponent) == 0xB0, "AnimationControllerComponent sizeof must be 176 bytes");

} // namespace Caver

#endif // CAVER_ANIMATIONCONTROLLERCOMPONENT_H
