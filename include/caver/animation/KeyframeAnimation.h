// Recovered from libswordigo.so (1.4.13) evidence.
//
// KeyframeAnimation is the keyframe clip data: a name, a frame count, a
// playback rate, one FloatDataBuffer per frame, and the clip duration.
// It has NO standalone member functions in the golden binary: the census
// (docs/census/FUNCTION_CENSUS.tsv) contains no Caver::KeyframeAnimation::
// row at all, and the symbol table has no ctor/dtor/accessor symbol for
// it. Its constructor and (implicit) destructor exist only inlined into
// other functions, which is the evidence this header is built from:
//
//   Layout — sizeof 0x38 (56), from the operator new(0x38) in
//   PODLoader::CreateAnimationFromFile (0x4E59C0), whose inlined
//   construction initialises, in order:
//     +0   std::string name            (three zero qwords; assigned from
//                                       the animation name in
//                                       ModelLibrary::AnimationForModel
//                                       0x4E2D08 via std::string::operator=
//                                       on the object base, and copied out
//                                       the same way in
//                                       KeyframeAnimationComponent::
//                                       InitWithAnimation 0x32F7C8)
//     +24  int numFrames               (init 0; set to the POD frame
//                                       count by CreateAnimationFromFile,
//                                       read as ==1 single-frame test in
//                                       AnimKeysNode::Evaluate 0x32559C)
//     +28  float frameRate             (init 24.0f, stored as the qword
//                                       0x41C0000000000000 together with
//                                       numFrames; AnimKeysNode::Update
//                                       0x3254B0 / UpdateFrameBlend
//                                       0x325438 compute frame position
//                                       as time * *(float*)(anim + 28))
//     +32  boost::shared_array<FloatDataBuffer> frames
//                                       (constructed in-place with a null
//                                       pointer by the inlined ctor —
//                                       the PLT call to
//                                       boost::shared_array<FloatDataBuffer>
//                                       ::shared_array(FloatDataBuffer*) in
//                                       CreateAnimationFromFile; px at
//                                       +32, control block at +40, later
//                                       filled with one 16-byte
//                                       FloatDataBuffer per frame;
//                                       AnimKeysNode::Evaluate reads the
//                                       array base at +32 and the first
//                                       buffer's count as the clip's
//                                       float count)
//     +48  float duration              (init 0; CreateAnimationFromFile
//                                       stores (numFrames - 1) / frameRate;
//                                       AnimKeysNode::InitWithAnimation
//                                       0x325364 reads it as
//                                       *(float*)(anim + 48) - 0.001f)
//     +52  padding (to 0x38)
//
//   Destructor — implicit; its inlined body in
//   boost::checked_delete<KeyframeAnimation> (0x4E6430) releases the
//   frames control block at +40 first, then frees the name string's heap
//   buffer if its SSO flag is set, exactly the reverse-declaration-order
//   member destruction of the layout above. No vtable, no base class.
#ifndef CAVER_KEYFRAMEANIMATION_H
#define CAVER_KEYFRAMEANIMATION_H

#include <string>

#include <boost/shared_array.hpp>

#include "caver/animation/FloatDataBuffer.h"

namespace Caver {

class KeyframeAnimation {
public:
    // Inlined in the golden binary (see header note): member order and
    // initial values are those written by the inlined construction in
    // PODLoader::CreateAnimationFromFile. frames is constructed from a
    // null pointer (not default-constructed): the inlined ctor calls
    // shared_array<FloatDataBuffer>::shared_array(FloatDataBuffer*)
    // with 0, which allocates its control block.
    KeyframeAnimation()
        : numFrames(0), frameRate(24.0f), frames(0), duration(0.0f) {}

    std::string name;                             // +0
    int numFrames;                                // +24
    float frameRate;                              // +28
    boost::shared_array<FloatDataBuffer> frames;  // +32
    float duration;                               // +48
};

} // namespace Caver

#endif
