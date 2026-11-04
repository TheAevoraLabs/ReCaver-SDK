// Recovered from libswordigo.so (1.4.13) evidence.
//
// AnimKeysNode is the animation-tree leaf that samples one
// KeyframeAnimation clip into a FloatDataBuffer. It derives from
// AnimNode, which derives from ReferenceCountedObject (RTTI at
// 0x618CD8 is a __si_class_type_info whose base is
// ReferenceCountedObject's typeinfo at 0x618CC8; the intrusive
// reference count therefore sits at +8, and AnimKeysNode's first own
// member reuses the base's tail padding at +12).
//
// AnimNode has NO vtable symbol in the golden binary (only its
// typeinfo): every one of its virtuals is pure or inline, and its
// destructor was inline — it is an abstract interface, declared here
// because no AnimNode.h exists yet. Its vtable slot order is proven
// by the AnimKeysNode vtable (0x618D08) and AnimBlendNode vtable
// (0x618C78) relocation entries:
//   +0 ~AnimNode (D2), +8 ~AnimNode (D0), +16 Update(float),
//   +24 Evaluate(), +32 Evaluate(int, FloatDataTrack const*),
//   +40 resultBuffer(), +48 setPhase(float), +56 timeToFinish() const
// AnimNode::setPhase is a WEAK 4-byte `ret` at 0x3252CC (empty inline
// body); AnimNode::UpdateWithModelInstance (0x325898) is its only
// out-of-line member — declared here, defined by the AnimNode slice.
//
// AnimKeysNode layout — sizeof 80, from the ctor store map
// (0x325320), InitWithAnimation (0x325364), Update (0x3254B0),
// UpdateFrameBlend (0x325438), timeToFinish (0x32586C) and the
// destructor's member-teardown order (0x325748):
//   +0   vptr
//   +8   int referenceCount                     (ReferenceCountedObject)
//   +12  float time                             (init 0)
//   +16  int currentFrame                       (init -1)
//   +20  float frameBlend                       (init 0)
//   +24  boost::shared_ptr<KeyframeAnimation const> animation
//   +40  float length   (NOT initialised by the ctor; InitWithAnimation
//                        sets it to animation->duration - 0.001f, and
//                        Update/UpdateFrameBlend/setPhase clamp or
//                        scale the time against it)
//   +44  bool loop                              (init true)
//   +48  float rate                             (init 1.0f)
//   +52  float speed                            (init 1.0f)
//   +56  bool playing                           (init true)
//   +57  bool reverse                           (init false)
//   +64  FloatDataBuffer resultBuffer
// Update advances the time by dt * rate * speed (reversed when
// `reverse` is set); timeToFinish divides by rate * speed. The
// original names/semantics split of the +48/+52 pair is UNRECOVERED;
// only the arithmetic is proven.
//
// Weak-vs-strong symbol bindings in the golden binary show which
// members were inline in the original header: ~AnimKeysNode (WEAK),
// resultBuffer (WEAK), setPhase (WEAK) and timeToFinish (WEAK) are
// defined inline below; the ctor and the remaining functions are
// GLOBAL and live in AnimKeysNode.cpp.
#ifndef CAVER_ANIMKEYSNODE_H
#define CAVER_ANIMKEYSNODE_H

#include <boost/shared_ptr.hpp>

#include "caver/component/ReferenceCountedObject.h"
#include "caver/animation/FloatDataBuffer.h"
#include "caver/animation/KeyframeAnimation.h"

namespace Caver {

// One bone's slice of a FloatDataBuffer (12 bytes — proven by the
// track-pointer stride in AnimKeysNode::Evaluate(int, ...) 0x32564C
// and the field reads in FloatDataBuffer::Accumulate(..., track)
// 0x325AD4: type at +0 (4 = quaternion, 2 = vector3), start at +4,
// stride at +8).
struct FloatDataTrack {
    int type;   // +0
    int start;  // +4
    int stride; // +8
};

class ModelInstance;

class AnimNode : public ReferenceCountedObject {
public:
    virtual ~AnimNode() {}

    virtual bool Update(float deltaTime) = 0;
    virtual void Evaluate() = 0;
    virtual void Evaluate(int numTracks, const FloatDataTrack* tracks) = 0;
    virtual FloatDataBuffer* resultBuffer() = 0;
    // Golden AnimNode::setPhase @0x3252CC: empty body.
    virtual void setPhase(float phase) {}
    virtual float timeToFinish() const = 0;

    void UpdateWithModelInstance(float deltaTime, ModelInstance* modelInstance); // golden-proven void (was bool in the first reconstruction — the bool shape grades 132B/88.7%, void grades 124B/100%)
};

class AnimKeysNode : public AnimNode {
public:
    AnimKeysNode();
    virtual ~AnimKeysNode() {}

    void InitWithAnimation(
        const boost::shared_ptr<const KeyframeAnimation>& animation);
    void CreateBufferIfNeccessary();

    virtual bool Update(float deltaTime);
    bool UpdateFrameBlend();
    virtual void Evaluate();
    virtual void Evaluate(int numTracks, const FloatDataTrack* tracks);

    virtual FloatDataBuffer* resultBuffer() { return &m_resultBuffer; }
    virtual void setPhase(float phase) { m_time = m_length * phase; }
    virtual float timeToFinish() const {
        if (m_loop)
            return 0.0f;
        return (m_length - m_time) / (m_rate * m_speed);
    }

    float m_time;         // +12
    int m_currentFrame;   // +16
    float m_frameBlend;   // +20
    boost::shared_ptr<const KeyframeAnimation> m_animation; // +24
    float m_length;       // +40
    bool m_loop;          // +44
    float m_rate;         // +48
    float m_speed;        // +52
    bool m_playing;       // +56
    bool m_reverse;       // +57
    FloatDataBuffer m_resultBuffer; // +64
};

} // namespace Caver

#endif
