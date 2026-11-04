// Recovered from libswordigo.so (1.4.13) evidence — the six out-of-line
// Caver::OrientedRect functions at 0x536F14-0x5373F4 (SF-2, jni-small
// lane).
//
// Layout (32 bytes, proven by OBFromRectangle @0x536F14, which writes
// every field): center +0, first axis +8, second axis +16, half
// extents +24 — OBFromRectangle stores the constant axes (1,0),(0,1)
// at +8 and the half size at +24. Original member NAMES are
// UNRECOVERED; roles are as proven.
#ifndef CAVER_ORIENTEDRECT_H
#define CAVER_ORIENTEDRECT_H

#include "../core/Vector2.h"
#include "../core/Rectangle.h"
#include "CollisionShape.h" // for Caver::Polygon / Caver::LineSegment

namespace Caver {

struct OrientedRect {
    Vector2 m_center;      // +0
    Vector2 m_axisX;       // +8
    Vector2 m_axisY;       // +16
    Vector2 m_halfExtents; // +24

    static OrientedRect OBFromRectangle(const Rectangle& rectangle);
    static OrientedRect OBFromTransformedRectangle(
        const Rectangle& rectangle, const Vector2& offset, float rotation,
        float scale, const Vector2& offset2, float rotation2, float scale2);

    float SpanOnAxis(const Vector2& axis, float* minOut, float* maxOut) const;
    bool IntersectsLineSegmentOnAxis(const LineSegment& segment,
                                     const Vector2& axis, float* minOut,
                                     float* maxOut) const;
    float IntersectsOrientedRectOnAxis(const OrientedRect& other,
                                       const Vector2& axis, float* minOut,
                                       float* maxOut) const;
    bool IntersectsPolygonOnAxis(const Polygon& polygon, const Vector2& axis,
                                 float* minOut, float* maxOut) const;
};

} // namespace Caver

#endif
