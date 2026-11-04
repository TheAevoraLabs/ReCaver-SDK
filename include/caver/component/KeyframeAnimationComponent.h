// Recovered from libswordigo.so (1.4.13) evidence — vtable
// _ZTVN5Caver26KeyframeAnimationComponentE at 0x619920 (0x178 bytes:
// primary address point 0x619920, IBindable secondary 0x619A40,
// IUpdateable secondary 0x619A80).
//
// sizeof 0xA0 (160 bytes) is proven by:
//   operator new(0xA0u) in Create() @0x34FFD8 and Clone() @0x330C98.
#ifndef CAVER_KEYFRAMEANIMATIONCOMPONENT_H
#define CAVER_KEYFRAMEANIMATIONCOMPONENT_H

#include <string>
#include <vector>
#include <boost/intrusive_ptr.hpp>
#include <boost/shared_ptr.hpp>

#include "Component.h"
#include "ComponentOutlet.h"
#include "ModelComponent.h"
#include "AnimationComponent.h"
#include "../animation/AnimKeysNode.h"
#include "../animation/KeyframeAnimation.h"

namespace Caver {

class KeyframeAnimationComponent : public Component {
public:
    KeyframeAnimationComponent();
    virtual ~KeyframeAnimationComponent();

    static KeyframeAnimationComponent* Create();
    static long Interface();

    virtual KeyframeAnimationComponent* Clone() const;
    void InitWithComponent(const KeyframeAnimationComponent* other);
    void InitWithAnimation(const boost::shared_ptr<const KeyframeAnimation>& animation);

    virtual void LoadFromProtobufMessage(const Proto::Component& message);
    virtual void SaveToProtobufMessage(Proto::Component* message) const;
    virtual bool ShouldSave();
    virtual void Prepare();
    // Not virtual: the golden vtable (address point 0x619920) holds only
    // 34 primary slots, ending at className (+256) and animationNode
    // (+264). RegisterLibrary is a GLOBAL out-of-line member at
    // 0x32FAEC and does not appear in either secondary vtable, so
    // declaring it virtual here would shift animationNode to slot 34.
    void RegisterLibrary();

    virtual void GetBindings(std::vector<Binding>* bindings);
    virtual void GetEnumValuesForBindedProperty(int property, std::vector<BindingValue>* values);
    virtual BindingValue ValueForBindedProperty(int property);
    virtual void SetValueForBindedProperty(int property, const BindingValue& value);

    virtual int componentCategories() const;
    virtual bool ImplementsInterface(long interfaceId);
    virtual void RegisterInterfaces(ComponentManager* manager);
    virtual std::string className() const;

    virtual AnimKeysNode* animationNode(); // primary vtable slot 33 (byte offset 264) — 0x330D84, the last primary slot
    AnimKeysNode* AnimationNode() { return animationNode(); }

    void LoadAnimation();

    ComponentOutlet<ModelComponent> m_modelOutlet;                       // +104 (24 bytes)
    std::string m_name;                                                  // +128 (24 bytes)
    boost::intrusive_ptr<AnimKeysNode> m_animKeysNode;                   // +152 (8 bytes)
};

// Golden aarch64 NDK-libc++ layout anchor; MSVC STL member sizes differ,
// so this assert is Android/Linux-only (the Windows build is self-consistent
// under MSVC STL and does not claim golden byte-layout parity).
#ifndef _MSC_VER
static_assert(sizeof(KeyframeAnimationComponent) == 0xA0, "KeyframeAnimationComponent sizeof must be 160 bytes");
#endif

} // namespace Caver

#endif // CAVER_KEYFRAMEANIMATIONCOMPONENT_H
