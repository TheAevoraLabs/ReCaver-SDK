// Recovered from libswordigo.so (1.4.13) evidence — the five
// out-of-line Caver::CollisionShape functions (0x328560-0x328DA8),
// CollisionShapeComponent::UpdateCollisionShape / SetShape /
// the component ctor (which initialise this struct inside the
// component at +304), and the Shape factory functions.
//
// A CollisionShape is a Shape placed in the world: the Shape prefix
// (type + shared geometry pointer) followed by the live transform
// and motion state the collision tests consume.
//
//   +0   Shape::Type (0 = none, 1 = rectangle, 2 = circle, 3 = polygon)
//   +8   boost::shared_ptr<void> geometry (Rectangle / Circle /
//        Polygon, type-dependent; pointer at +8, control block at
//        +16). Golden ~Shape @0x326EBC is exactly this shared_ptr's
//        decref: it releases the +16 control block and destroys /
//        deallocates when the counts hit zero, which is what the
//        implicit dtor of this member does — the earlier void* pair
//        had a trivial dtor and leaked the decref.
//   +24  Vector2 position (world position of the shape's origin)
//   +32  float rotation (radians; the first of the (rotation, scale)
//        pair every geometry helper takes)
//   +36  float scale (uniform)
//   +40  bool flippedHorizontally (geometry is mirrored in x before
//        use: Rectangle::FlipHorizontally, or the circle centre's x
//        is negated, per the tests below)
//   +44  Vector2 velocity (the owning SceneObject's velocity at
//        +120, copied by UpdateCollisionShape; drives the swept
//        line-segment pre-test in CollidesWithShape and is reported
//        back through CollisionInfo)
//   +52  float minX (world AABB x range; UpdateCollisionShape
//   +56  float maxX  recomputes it from the component's range pair)
//   +60  Vector2 m_field60 — passed by const reference as the last
//        argument of RectangleIntersectsRectangle /
//        RectangleIntersectsPolygon; copied by UpdateCollisionShape
//        from the component's +492/+496 pair. Semantics UNRECOVERED.
//
// sizeof = 72 (fields proven to +68; +68 is tail padding).
// CollisionShapeComponent's ctor writes +376/+380 (its own range
// pair, -15/+15) just past this struct — those are component
// fields, NOT part of CollisionShape.
#ifndef CAVER_COLLISIONSHAPE_H
#define CAVER_COLLISIONSHAPE_H

#include "../core/Vector2.h"
#include "../core/Rectangle.h"
#include <boost/shared_ptr.hpp>
#include <vector>

namespace Caver {

// Geometry payloads of a Shape. Polygon's vector and flags are shared
// with GroundPolygonComponent's embedded contour.
struct Circle {
    Vector2 center; // +0
    float radius;   // +8
};

struct Polygon {
    int m_count;            // +0
    std::vector<Vector2> m_vertices; // +8
    bool m_field32;         // +32 — CollidesWithShape branches on it:
                            // set: RectangleIntersectsConvexPolygon,
                            // clear: RectangleIntersectsPolygon
    bool m_closed;          // +33 — the Polygon ctor's closed flag

    Polygon() : m_count(0), m_vertices(), m_field32(false), m_closed(true) {}
    // Golden 0x537750: copies count vertices and duplicates the
    // first at index count (the closed-polygon wrap entry), so
    // m_vertices holds count + 1 entries, as SetVertices does.
    Polygon(int count, const Vector2* vertices, bool closed);
    void SetVertices(int count, const Vector2* vertices);
    bool VertexAtIndexIsConvexCCW(int index);
    // Golden 0x537914: ray-crossing point-in-polygon over the count
    // contour vertices (the +1 duplicate wrap entry is not visited).
    bool PointInside(const Vector2& point) const;
    // Golden 0x5379C8: inserts evenly spaced vertices on every edge
    // longer than maxLength (edges = count when closed, count - 1
    // otherwise); m_count grows by one per inserted vertex.
    void SplitEdgesToMaxLength(float maxLength);
    Rectangle Bounds() const;
    void SpanOnAxis(const Vector2& axis, float* minOut, float* maxOut) const;
    bool IntersectsRectangle(const Rectangle& rect, float* out1, float* out2) const;
    bool IntersectsCircle(const Circle& circle, float* out1, float* out2) const;
};

struct LineSegment {
    Vector2 start; // +0
    Vector2 end;   // +8

    // Golden: _ZNK5Caver11LineSegment6BoundsEv (returns the segment's
    // bounding Rectangle by value). Declared by the core slice for
    // Scene::LineSegmentIntersectsGround (2026-10-24); the body is the
    // physics slice's to recover.
    Rectangle Bounds() const;
    void SpanOnAxis(const Vector2& axis, float* minOut, float* maxOut) const;
    bool IntersectsRectangle(const Rectangle& rect, float* out1, float* out2) const;
    bool IntersectsCircle(const Circle& circle, float* out1, float* out2) const;
    // Golden 0x53532C / 0x535418: segment/segment and segment/line
    // intersection with the shared 0.0001 denominator guard and the
    // strict (0.00001, 0.99999) open-interval test. IntersectsLine
    // range-checks only the first parameter; the second is written
    // unchecked.
    bool IntersectsLineSegment(const LineSegment& other, float* outT1, float* outT2) const;
    bool IntersectsLine(const LineSegment& other, float* outT1, float* outT2) const;
};

// The authored geometry: a type tag plus a shared pointer to a
// Rectangle (16 bytes), Circle (12 bytes) or Polygon, selected by
// the tag. ShapeWithCircle / ShapeWithRectangle / ShapeWithPolygon
// write exactly this 24-byte prefix.
struct Shape {
    enum Type {
        kNone = 0,
        kRectangle = 1,
        kCircle = 2,
        kPolygon = 3
    };
    Type m_type;    // +0
    // +8: shared_ptr<void> — pointer at +8, control block at +16.
    // The implicit dtor is golden ~Shape @0x326EBC's decref.
    boost::shared_ptr<void> m_geometry;

    Rectangle Bounds() const;

    static Shape ShapeWithRectangle(const Rectangle& rect);
    static Shape ShapeWithCircle(const Circle& circle);
    static Shape ShapeWithPolygonNoCopy(Polygon* polygon);
    // Copying polygon factory (golden GroundPolygonComponent::Process
    // 0x3AA97C builds each ground shape with ShapeWithPolygon, i.e.
    // a copy of the contour, not an aliased polygon).
    static Shape ShapeWithPolygon(const Polygon& polygon);
};

static_assert(sizeof(Shape) == 24, "Shape must stay 24 bytes (type + shared_ptr)");

// Contact report filled by CollisionShape::CollidesWithShape when
// the info pointer is non-null and the shapes collide (37 bytes
// written; the struct is 40 with tail padding). The layout is the
// one PhysicsObjectState::HandleGroundCollision consumes through
// CollisionMessageData (see PhysicsObjectState.h).
struct CollisionInfo {
    Vector2 m_position;      // +0 — (possibly sweep-adjusted) position
    Vector2 m_velocity;      // +8 — this shape's velocity (+44)
    Vector2 m_otherVelocity; // +16 — the other shape's velocity (+44)
    Vector2 m_normal;        // +24
    float m_depth;           // +32
    bool m_valid;            // +36 — set to true on a filled report
    char _pad37[3];          // +37
};

// Free collision primitives (out-of-line in the golden binary in the
// 0x533980-0x535100 cluster). Declared here because CollisionShape
// is their dispatcher; the bodies belong to the physics/geometry
// queue.
bool CircleIntersectsCircle(const Circle& a, const Vector2& positionA, float rotationA, float scaleA,
                            const Circle& b, const Vector2& positionB, float rotationB, float scaleB,
                            Vector2* normal, float* depth);
bool CircleIntersectsRectangle(const Circle& a, const Vector2& positionA, float rotationA, float scaleA,
                               const Rectangle& b, const Vector2& positionB, float rotationB, float scaleB,
                               Vector2* normal, float* depth);
bool CircleIntersectsPolygon(const Circle& a, const Vector2& positionA, float rotationA, float scaleA,
                             const Polygon& b, const Vector2& positionB, float rotationB, float scaleB,
                             Vector2* normal, float* depth);
bool RectangleIntersectsRectangle(const Rectangle& a, const Vector2& positionA, float rotationA, float scaleA,
                                  const Rectangle& b, const Vector2& positionB, float rotationB, float scaleB,
                                  Vector2* normal, float* depth, const Vector2& field60);
bool RectangleIntersectsPolygon(const Rectangle& a, const Vector2& positionA, float rotationA, float scaleA,
                                const Polygon& b, const Vector2& positionB, float rotationB, float scaleB,
                                Vector2* normal, float* depth, const Vector2& field60);
bool RectangleIntersectsConvexPolygon(const Rectangle& a, const Vector2& positionA, float rotationA, float scaleA,
                                      const Polygon& b, const Vector2& positionB, float rotationB, float scaleB,
                                      Vector2* normal, float* depth);
bool LineSegmentIntersectsRectangle(const LineSegment& segment, const Rectangle& rectangle,
                                    const Vector2& position, float rotation, float scale,
                                    float* outT, Vector2* outPoint);
bool LineSegmentIntersectsCircle(const LineSegment& segment, const Circle& circle,
                                 const Vector2& position, float rotation, float scale,
                                 float* outT, Vector2* outPoint);
bool LineSegmentIntersectsPolygon(const LineSegment& segment, const Polygon& polygon,
                                  const Vector2& position, float rotation, float scale,
                                  float* outT, Vector2* outPoint, Vector2* outPoint2, int* outIndex);

class CollisionShape {
public:
    bool CollidesWithShape(const CollisionShape& other, float scale, CollisionInfo* info) const;
    bool IntersectsWithLineSegment(const LineSegment& segment, Vector2* outPoint, Vector2* outPoint2, int* outIndex) const;
    Vector2 NearestPointInShape(const Vector2& point) const;
    LineSegment TransformedPolygonSegmentAtIndex(int index) const;
    Vector2 TransformedPolygonVertexAtIndex(int index) const;

    Shape m_shape;              // +0
    Vector2 m_position;         // +24
    float m_rotation;           // +32
    float m_scale;              // +36
    bool m_flippedHorizontally; // +40
    char _pad41[3];             // +41
    Vector2 m_velocity;         // +44
    float m_minX;               // +52
    float m_maxX;               // +56
    Vector2 m_field60;          // +60 — UNRECOVERED semantics (see above)
    char _pad68[4];             // +68

    const Rectangle* rectangleData() const { return static_cast<const Rectangle*>(m_shape.m_geometry.get()); }
    const Circle* circleData() const { return static_cast<const Circle*>(m_shape.m_geometry.get()); }
    const Polygon* polygonData() const { return static_cast<const Polygon*>(m_shape.m_geometry.get()); }
};

} // namespace Caver

#endif
