// Recovered from libswordigo.so (1.4.13) evidence — the five out-of-line
// Caver::Circle functions at 0x53503C-0x5351E4 (SF-2, jni-small lane).
//
// Layout (proven by those bodies and by CollisionShape's geometry
// access): center +0 (Vector2), radius +8. sizeof 12.
//
// NOTE (merge hand-off): src/caver/physics/CollisionShape.h carries a
// plain-field `struct Circle` with this same layout but no methods.
// The two definitions cannot co-exist in one TU; at merge the
// CollisionShape.h stub should be replaced by this header.
#ifndef CAVER_CIRCLE_H
#define CAVER_CIRCLE_H

#include "../core/Vector2.h"
#include "../core/Rectangle.h"

namespace Caver {

// Layout proven by CollisionShape.h (systems/physics): two Vector2
// endpoints. SpanOnAxis is out-of-line in the golden
// (_ZNK5Caver11LineSegment10SpanOnAxisERKNS_7Vector2EPfS4_,
// 0x5352F4); declared here for Circle's use — the definition belongs
// to the physics queue.
struct LineSegment {
    Vector2 start; // +0
    Vector2 end;   // +8

    void SpanOnAxis(const Vector2& axis, float* minOut, float* maxOut) const;
};

struct Circle {
    Vector2 center; // +0
    float radius;   // +8

    bool PointInside(const Vector2& point) const;
    bool IntersectsWithCircle(const Circle& other) const;
    float SpanOnAxis(const Vector2& axis, float* minOut, float* maxOut) const;
    bool IntersectsLineSegmentOnAxis(const LineSegment& segment,
                                     const Vector2& axis, float* minOut,
                                     float* maxOut) const;
    Rectangle Bounds() const;
};

} // namespace Caver

#endif
