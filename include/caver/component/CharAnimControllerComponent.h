// Recovered from libswordigo.so (1.4.13) evidence — hero lane, march 2.
// CharAnimControllerComponent: character animation state machine built
// on AnimationControllerComponent. sizeof 0x180 (384), proven by
// Create()'s operator new(0x180) at 0x3504D8.
//
// NOTE (hand-off): AnimationControllerComponent has no recovered header
// in the tree yet, so its proven layout is declared in this file ahead
// of the derived class. When the AnimationControllerComponent slice
// lands its own header, this block should move there unchanged:
//   sizeof 0xB0 (176), proven by its Clone()'s operator new(0xB0).
//   +104 ComponentOutlet<ModelComponent>          ("model", id 0)
//   +128 ComponentOutlet<KeyframeAnimationComponent> ("defaultAnimation", id 1)
//   +152 boost::intrusive_ptr<AnimNode>  current node
//   +160 bool selfUpdate (proto field 3; RequiresUpdate() returns it)
//   +168 boost::intrusive_ptr<AnimNode>  blend node storage
// Its BlendToAnimationNode is the first virtual added after
// Component's (primary vtable slot 33, byte offset 264 — proven by
// the call sites in this class's Update/Swing/StopAction bodies).
#ifndef CAVER_CHARANIMCONTROLLERCOMPONENT_H
#define CAVER_CHARANIMCONTROLLERCOMPONENT_H

#include "Component.h"
#include "ComponentOutlet.h"
#include "ModelComponent.h"
#include "KeyframeAnimationComponent.h"
#include "AnimationControllerComponent.h"
#include "../animation/AnimKeysNode.h"

namespace Caver {

class AnimBlendNode;
class AnimNode;
class AnimKeysNode;

// ---------------------------------------------------------------------------
// CharAnimControllerComponent
// Member order is proven by the destructor's teardown order at
// 0x358880 (368, 360, 352, 336, 328, 320, then the six outlet targets
// in reverse outlet order) and the constructor's store map at
// 0x356898. The six outlets are ComponentOutlet<KeyframeAnimation-
// Component> (their vtable, off_61D8C0, is _ZTV for that instantiation
// +16); each outlet's identifier is a proto field (stand=4, walk=5,
// jump=6, fall=7, cast=8, air_jump=9 in the proto message; stored in
// message order stand/walk/jump/airJump/fall/cast at +12..+32).
// ---------------------------------------------------------------------------
class CharAnimControllerComponent : public AnimationControllerComponent {
public:
    CharAnimControllerComponent();
    virtual ~CharAnimControllerComponent();

    static CharAnimControllerComponent* Create();
    static long Interface() { return reinterpret_cast<long>(&CharAnimControllerComponent::Interface); }

    virtual Component* Clone() const;
    // Non-virtual narrowed overload (mangled EPKS0_), not an override.
    void InitWithComponent(const CharAnimControllerComponent* other);
    virtual void LoadFromProtobufMessage(const Proto::Component& message);
    virtual void SaveToProtobufMessage(Proto::Component* message) const;
    virtual bool ShouldSave() { return true; }
    virtual void GetBindings(std::vector<Binding>* bindings);
    virtual void GetEnumValuesForBindedProperty(int property, std::vector<BindingValue>* values);
    virtual BindingValue ValueForBindedProperty(int property);
    virtual void SetValueForBindedProperty(int property, const BindingValue& value);
    virtual void Prepare();
    virtual bool ImplementsInterface(long interfaceId) {
        return interfaceId == reinterpret_cast<long>(&CharAnimControllerComponent::Interface)
            || interfaceId == reinterpret_cast<long>(&AnimationControllerComponent::Interface);
    }
    virtual void RegisterInterfaces(ComponentManager* manager) {
        RegisterInterface(manager, reinterpret_cast<long>(&CharAnimControllerComponent::Interface));
        RegisterInterface(manager, reinterpret_cast<long>(&AnimationControllerComponent::Interface));
        Component::RegisterInterfaces(manager);
    }
    virtual std::string className() const { return "CharAnimController"; }

    virtual void Update(float deltaTime);
    virtual void BlendToAnimationNode(const boost::intrusive_ptr<AnimNode>& node, float duration);

    void Reset();
    bool ActionNearlyFinished();
    void SetCurrentRunSpeed(float speed);
    bool IsMoving();
    void StartMoving();
    void StopMoving();
    void StartJumping(bool airJump);
    bool CanStartFalling();
    void StartFalling();
    void StandOnGround();
    bool IsReadyToJump();
    void SwingWithAnimation(const boost::intrusive_ptr<AnimKeysNode>& animation);
    void BeginCasting();
    void FinishCasting(const boost::intrusive_ptr<AnimKeysNode>& animation);
    void StopAction();
    void BlendFromAnimationNode(const boost::intrusive_ptr<AnimKeysNode>& animation, float duration);
    void BlendToMoveAnimation(float duration);

    ComponentOutlet<KeyframeAnimationComponent> m_standAnimationOutlet;   // +176, BindOutlet 100 "standAnimation"
    ComponentOutlet<KeyframeAnimationComponent> m_walkAnimationOutlet;    // +200, BindOutlet 101 "walkAnimation"
    ComponentOutlet<KeyframeAnimationComponent> m_jumpAnimationOutlet;    // +224, BindOutlet 102 "jumpAnimation"
    ComponentOutlet<KeyframeAnimationComponent> m_airJumpAnimationOutlet; // +248, BindOutlet 103 "airJumpAnimation"
    ComponentOutlet<KeyframeAnimationComponent> m_fallAnimationOutlet;    // +272, BindOutlet 104 "fallAnimation"
    ComponentOutlet<KeyframeAnimationComponent> m_castAnimationOutlet;    // +296, BindOutlet 105 "castAnimation"
    boost::intrusive_ptr<AnimBlendNode> m_moveBlend;                      // +320 (stand<->walk, 0.1)
    boost::intrusive_ptr<AnimBlendNode> m_jumpFallBlend;                  // +328 (jump<->fall, 0.35)
    boost::intrusive_ptr<AnimBlendNode> m_rootBlend;                      // +336 (move<->jumpFall, 0.1)
    bool m_inAction;                                                      // +344
    boost::intrusive_ptr<AnimKeysNode> m_actionNode;                      // +352
    boost::intrusive_ptr<AnimKeysNode> m_field360;                        // +360 — name UNRECOVERED (mirrors m_actionNode in Swing/StopAction)
    boost::intrusive_ptr<AnimBlendNode> m_blendNode;                      // +368 (BlendFrom/BlendTo storage)
    float m_blendTimer;                                                   // +376
};

} // namespace Caver

#endif
