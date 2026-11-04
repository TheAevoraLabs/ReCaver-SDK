// Recovered from libswordigo.so (1.4.13) evidence — the six out-of-line
// Caver::BezierSegment functions at 0x5331CC-0x5336A8 (SF-2, jni-small
// lane). A cubic Bezier segment: four Vector2 control points, 32 bytes.
// Member roles are proven by the Bernstein weights in
// EvaluatePosition (0x533280); the original member NAMES are
// UNRECOVERED.
#ifndef CAVER_BEZIERSEGMENT_H
#define CAVER_BEZIERSEGMENT_H

#include "../core/Vector2.h"

namespace Caver {

struct BezierSegment {
    Vector2 m_start;    // +0  — Bernstein weight (1-t)^3
    Vector2 m_control1; // +8  — Bernstein weight 3(1-t)^2 t
    Vector2 m_control2; // +16 — Bernstein weight 3(1-t) t^2
    Vector2 m_end;      // +24 — Bernstein weight t^3

    float ControlPolygonLength() const;
    float ApproximatedLength() const;
    Vector2 EvaluatePosition(float t) const;
    Vector2 EvaluateTangent(float t) const;
    float MovePathLocation(float location, int direction, float distance) const;
    bool IntersectsWithX(float x, int* count, float* roots);
};

} // namespace Caver

#endif
