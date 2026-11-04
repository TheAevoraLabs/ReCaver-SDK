// Recovered from libswordigo.so (1.4.13) evidence.
// GUIAnimationCurve is a BezierSegment whose control points are pinned to
// (0,0) and (1,1) at the ends: the constructor writes point0 = (0,0),
// point1 = first argument, point2 = second argument, point3 = (1,1)
// (IDA 0x52EDB8). ValueAtTime treats the object as a BezierSegment
// (calls BezierSegment::IntersectsWithX / EvaluatePosition on this).
#ifndef CAVER_GUIANIMATIONCURVE_H
#define CAVER_GUIANIMATIONCURVE_H

#include "../core/Vector2.h"

namespace Caver {

class BezierSegment {
public:
    Vector2 points[4]; // +0, +8, +16, +24 — sizeof 32

    bool IntersectsWithX(float x, int* segment, float* t);
    Vector2 EvaluatePosition(float t) const;
};

class GUIAnimationCurve : public BezierSegment {
public:
    GUIAnimationCurve(const Vector2& point1, const Vector2& point2);
    float ValueAtTime(float time);
};

} // namespace Caver

#endif
