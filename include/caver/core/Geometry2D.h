// Geometry2D - 2D geometric algorithms, intersections, and math helpers for CaverEngine.
// Adapted from Godot Engine 4.7.2 (core/math/geometry_2d.h, math_funcs.h, easing_equations.h).
// Original Godot code: Copyright (c) 2014-present Godot Engine contributors, MIT License.
// Integration for CaverEngine / ReCaver: Quantum Creeper / Aevora Labs.

#ifndef CAVER_CORE_GEOMETRY2D_H
#define CAVER_CORE_GEOMETRY2D_H

#include "Vector2.h"
#include "Rectangle.h"
#include <vector>
#include <string>

namespace Caver {

class Geometry2D {
public:
    // --- Segment & Line Intersections ---

    // Segment vs Segment intersection. Returns true if segments intersect, with intersection point in outIntersection.
    static bool SegmentIntersectsSegment(const Vector2& fromA, const Vector2& toA,
                                         const Vector2& fromB, const Vector2& toB,
                                         Vector2* outIntersection = nullptr);

    // Segment vs Axis-Aligned Rectangle intersection test. Returns true if segment touches or penetrates the rect.
    static bool SegmentIntersectsRect(const Vector2& from, const Vector2& to,
                                      const Rectangle& rect);

    // Segment vs Circle intersection test. Returns number of intersections (0, 1, or 2).
    // Parametric fractions t1 and t2 along the segment [0..1] are written to outT1 and outT2 if provided.
    static int SegmentIntersectsCircle(const Vector2& from, const Vector2& to,
                                       const Vector2& circleCenter, float circleRadius,
                                       float* outT1 = nullptr, float* outT2 = nullptr);

    // Infinite Line vs Infinite Line intersection. Returns true if lines are not parallel, writing point to outIntersection.
    static bool LineIntersectsLine(const Vector2& fromA, const Vector2& dirA,
                                   const Vector2& fromB, const Vector2& dirB,
                                   Vector2* outIntersection = nullptr);

    // --- Distance & Proximity ---

    // Returns the closest point on segment AB to point P.
    static Vector2 GetClosestPointToSegment(const Vector2& point, const Vector2& segA, const Vector2& segB);

    // Returns the Euclidean distance from point P to segment AB.
    static float GetDistanceToSegment(const Vector2& point, const Vector2& segA, const Vector2& segB);

    // Returns closest points between two line segments (c1 on segment 1, c2 on segment 2).
    static float GetClosestPointsBetweenSegments(const Vector2& p1, const Vector2& q1,
                                                 const Vector2& p2, const Vector2& q2,
                                                 Vector2* outC1, Vector2* outC2);

    // --- Shape Containment Queries ---

    // Tests if point is inside a 2D polygon using ray-crossing algorithm with AABB acceleration.
    static bool IsPointInPolygon(const Vector2& point, const std::vector<Vector2>& polygon);

    // Tests if point is inside a 2D circle.
    static bool IsPointInCircle(const Vector2& point, const Vector2& center, float radius);

    // Tests if point is inside triangle ABC using cross-product orientation test.
    static bool IsPointInTriangle(const Vector2& point, const Vector2& a, const Vector2& b, const Vector2& c);

    // --- Math, Angles & Interpolation ---

    // Angular linear interpolation handling 360-degree wrapping cleanly.
    static float LerpAngle(float from, float to, float weight);

    // Shortest angular difference between two angles in radians [-PI, PI].
    static float AngleDifference(float from, float to);

    // Moves float towards target by delta without overshooting.
    static float MoveToward(float from, float to, float delta);

    // Rotates angle towards target angle by delta radians without overshooting or wrapping issues.
    static float RotateToward(float from, float to, float delta);

    // Smooth Hermite polynomial interpolation between from and to.
    static float Smoothstep(float from, float to, float s);

    // Cubic Bezier curve evaluation at t [0..1].
    static float BezierInterpolate(float start, float control1, float control2, float end, float t);

    // Derivative of cubic Bezier curve at t [0..1].
    static float BezierDerivative(float start, float control1, float control2, float end, float t);

    // Evaluates Robert Penner easing equations (Sine, Quad, Cubic, Quart, Quint, Expo, Circ, Back, Elastic, Bounce).
    // name examples: "InQuad", "OutBounce", "InOutSine", "Linear".
    static float Ease(const std::string& name, float t);

    // --- HUD Offscreen Target Indicator (perfect-arrows style) ---

    // Given a target world coordinate, camera position, and viewport world bounds, calculates
    // the screen edge position (px, py) and rotation angle in degrees for a HUD pointing arrow.
    // If target is inside the viewport, outIsOffscreen is false.
    static bool GetOffscreenIndicator(const Vector2& targetWorld,
                                      const Vector2& camWorld,
                                      const Rectangle& viewportWorld,
                                      float margin,
                                      Vector2* outScreenPos,
                                      float* outAngleDeg,
                                      bool* outIsOffscreen);
};

} // namespace Caver

#endif // CAVER_CORE_GEOMETRY2D_H
