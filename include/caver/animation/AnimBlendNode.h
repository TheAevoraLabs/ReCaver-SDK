// Recovered from libswordigo.so (1.4.13) evidence — IDA exports in
// ~/workspace/analysis/swordigo/functions/Caver/AnimBlendNode/ checked
// against the raw arm64 disassembly; the binary is the spec.
//
// AnimBlendNode cross-fades between two child AnimNodes. Layout
// (sizeof 48, proven by both ctor store maps @0x324DD0 / @0x324E00,
// Update @0x324E9C and the destructor teardown order @0x3251B4):
//   +0  vptr (AnimNode / ReferenceCountedObject)
//   +8  int referenceCount            (ReferenceCountedObject)
//   +16 boost::intrusive_ptr<AnimNode> m_nodeA
//   +24 boost::intrusive_ptr<AnimNode> m_nodeB
//   +32 float m_blend — 0 = fully A, 1 = fully B
//   +36 float m_field36 — blend duration used when blending forward
//        (and as the fallback duration in reverse)
//   +40 float m_field40 — blend duration used when blending in
//        reverse, when > 0 (default -1 = unset)
//   +44 bool m_field44 — reverse flag (UNRECOVERED name)
//   +45 bool m_field45 — blending-in-progress flag (UNRECOVERED name)
// Member names for +36..+45 are UNRECOVERED; only the arithmetic is
// proven. The class does NOT override setPhase (no such symbol; the
// vtable keeps AnimNode's empty inline body).
#ifndef CAVER_ANIMBLENDNODE_H
#define CAVER_ANIMBLENDNODE_H

#include <boost/intrusive_ptr.hpp>

#include "caver/animation/AnimKeysNode.h" // AnimNode + FloatDataTrack

namespace Caver {

class AnimBlendNode : public AnimNode {
public:
    AnimBlendNode();
    AnimBlendNode(const boost::intrusive_ptr<AnimNode>& nodeA,
                  const boost::intrusive_ptr<AnimNode>& nodeB,
                  float durationA, float durationB);

    virtual bool Update(float deltaTime);
    virtual void Evaluate();
    virtual void Evaluate(int numTracks, const FloatDataTrack* tracks);
    virtual FloatDataBuffer* resultBuffer();
    virtual float timeToFinish() const;
    virtual ~AnimBlendNode();

    boost::intrusive_ptr<AnimNode> m_nodeA; // +16
    boost::intrusive_ptr<AnimNode> m_nodeB; // +24
    float m_blend;                          // +32
    float m_field36;                        // +36 UNRECOVERED name
    float m_field40;                        // +40 UNRECOVERED name
    bool m_field44;                         // +44 UNRECOVERED name
    bool m_field45;                         // +45 UNRECOVERED name
};

} // namespace Caver

#endif
