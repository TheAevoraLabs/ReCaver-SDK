// Recovered from libswordigo.so (1.4.13) evidence — see docs/CORE_HEADERS.md.
// Layout: two floats, x then y (verified across the engine's math code).
#ifndef CAVER_VECTOR2_H
#define CAVER_VECTOR2_H

namespace Caver {

struct Vector3;

struct Vector2 {
    float x;
    float y;
    Vector2() {}
    Vector2(float _x, float _y) : x(_x), y(_y) {}
    Vector2(const Vector2& o) : x(o.x), y(o.y) {}
    // Member-wise copy assignment: golden copies of a Vector2 (e.g.
    // FWTouch.position in MapView::TouchBegan) go through the FP
    // registers (ldr d0 / str d0), not an integer blob copy — the
    // shape a user-defined member-wise operator= produces. (Added for
    // the systems slice's MapView reconstruction, 2026-10-23.)
    Vector2& operator=(const Vector2& o) {
        x = o.x;
        y = o.y;
        return *this;
    }

    // Out-of-line in the golden binary; definitions in Vector2.cpp
    // (core slice). Const-ness and return types per the golden
    // manglings/bodies (corrected 2026-10-23, core): Angle and Rounded
    // are const (_ZNK...), Normalize returns the normalised vector,
    // Rotate and RotateWithSinCos return the NEW Y component.
    Vector2(const Vector3& v);
    float Angle() const;
    Vector2 Normalize();
    Vector2 Rounded() const;
    static Vector2 FromAngle(float angle);
    Vector2 RotatedAroundPoint(float rotation, const Vector2& point) const;

    // Also out-of-line in the golden binary; declared here for the
    // CollisionShape slice (systems/physics, 2026-10-23); definitions
    // belong to the core slice.
    //   Transformed:        offset + rotate(*this * scale, rotation)
    //   InverseTransformed: inverse of Transformed
    Vector2 Transformed(const Vector2& offset, float rotation, float scale) const;
    Vector2 InverseTransformed(const Vector2& offset, float rotation, float scale) const;
    float Rotate(float rotation);
    // Out-of-line in the golden binary (_ZN5Caver7Vector216RotateWithSinCosEff);
    // definition in Vector2.cpp (core slice). Returns the new Y
    // component per the golden body at 0x5398E4 (the earlier Vector2&
    // declaration was a guess — corrected 2026-10-23, core).
    float RotateWithSinCos(float sinValue, float cosValue);
};

} // namespace Caver

#endif
