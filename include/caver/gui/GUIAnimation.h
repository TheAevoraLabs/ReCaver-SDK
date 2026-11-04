// Recovered from libswordigo.so (1.4.13) evidence.
//
// GUIAnimation is a plain (non-polymorphic — no vtable symbol in the
// golden) property animation: a GUITargetAction fired on completion, the
// animated property id, a GUIAnimationCurve, from/to GUIPropertyValues and
// the interpolated current value. Size 192 bytes (the factories allocate
// 0xC0), established from IDA 0x52E048 (ctor), 0x42C2A0 (dtor),
// 0x52E130/0x52E400/0x52E6D0 (factories), 0x52E9A0 (Update).
//
// Layout:
//   +0   GUITargetAction targetAction (48 bytes)
//   +48  int property (1 = scale, 2 = alpha, 3 = frame — set by the
//        Scale/Alpha/Frame factories respectively)
//   +52  GUIAnimationCurve curve ((0.5,0) / (0.5,1) control points)
//   +84  bool finished
//   +88  float delay (0 in the ctor)
//   +92  float duration (0.2 in the ctor; factories overwrite)
//   +96  float elapsed
//   +100 bool reversed (InterpolatedValue runs to->from when set)
//   +104 Vector2 field104 = (0.5, 0.5) — name UNRECOVERED
//   +112 GUIPropertyValue fromValue
//   +136 GUIPropertyValue toValue
//   +160 GUIPropertyValue currentValue
//   +184 bool field184 = true  — name UNRECOVERED
//   +185 bool field185 = false — name UNRECOVERED
//   +186 bool field186 = false — name UNRECOVERED
#ifndef CAVER_GUIANIMATION_H
#define CAVER_GUIANIMATION_H

#include <boost/shared_ptr.hpp>

#include "../core/Rectangle.h"
#include "../core/Vector2.h"
#include "GUIAnimationCurve.h"
#include "GUIPropertyValue.h"
#include "GUITargetAction.h"

namespace Caver {

class GUIAnimation {
public:
    GUIAnimation();
    // The golden D2 (0x42C2A0, 122 bytes) is exactly member teardown
    // (currentValue, toValue, fromValue, targetAction, in reverse
    // declaration order); declared here so the out-of-line destructor
    // is emitted and can be graded.
    ~GUIAnimation();

    static boost::shared_ptr<GUIAnimation> AlphaAnimation(float from,
                                                          float to,
                                                          float duration);
    static boost::shared_ptr<GUIAnimation> ScaleAnimation(float from,
                                                          float to,
                                                          float duration);
    static boost::shared_ptr<GUIAnimation> FrameAnimation(
        const Rectangle& from, const Rectangle& to, float duration);

    void Update(float deltaTime);
    void Transform() const;

    GUITargetAction targetAction;      // +0
    int property;                      // +48
    GUIAnimationCurve curve;           // +52
    bool finished;                     // +84
    float delay;                       // +88
    float duration;                    // +92
    float elapsed;                     // +96
    bool reversed;                     // +100
    Vector2 field104;                  // +104 UNRECOVERED name
    GUIPropertyValue fromValue;        // +112
    GUIPropertyValue toValue;          // +136
    GUIPropertyValue currentValue;     // +160
    bool field184;                     // +184 UNRECOVERED name
    bool field185;                     // +185 UNRECOVERED name
    bool field186;                     // +186 UNRECOVERED name
};

} // namespace Caver

#endif
