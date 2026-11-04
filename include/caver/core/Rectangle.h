// Recovered from libswordigo.so (1.4.13) evidence — see docs/CORE_HEADERS.md.
// Layout: origin (x, y) then size (width, height), four floats.
#ifndef CAVER_RECTANGLE_H
#define CAVER_RECTANGLE_H

#include "Vector2.h"

namespace Caver {

struct Matrix4;

struct Rectangle {
    float x;
    float y;
    float width;
    float height;

    Rectangle() : x(0), y(0), width(0), height(0) {}
    Rectangle(float _x, float _y, float _w, float _h) : x(_x), y(_y), width(_w), height(_h) {}

    // Proven non-trivial: Component::localAABB() returns Rectangle by
    // value through the sret pointer (golden: `stp xzr, xzr, [x8]; ret`),
    // which under AAPCS64 only happens for a type with a user-defined
    // destructor. A bare 4-float struct would be an HFA returned in
    // s0-s3. (Found by the components slice — see SHARED board.)
    ~Rectangle() {}

    // Out-of-line methods in the golden binary (declared by the core
    // slice for its Scene call sites; bodies are recovered with the
    // Rectangle function queue):
    void FlipHorizontally();
    void Transform(const Vector2& position, float rotation, const Vector2& scale);
    bool IntersectsWithRect(const Rectangle& other) const;
    // Golden _ZNK5Caver9Rectangle21RectangleByOutsettingEf; declared
    // by the core slice for Scene::Update (body: Rectangle queue).
    Rectangle RectangleByOutsetting(float outset) const;
    // Declared for the CollisionShape slice (systems/physics); body
    // belongs to the core slice's Rectangle queue.
    Vector2 NearestPointInRectangle(const Vector2& point) const;

    // Also out-of-line in the golden; declared here for the systems
    // slice's MapView reconstruction (2026-10-23).
    void Translate(const Vector2& offset);
    void ExpandToincludeRectangle(const Rectangle& rect);
    // Proven from the golden symbols _ZNK5Caver9Rectangle11PointInside
    // ERKNS_7Vector2E / _ZNK5Caver9Rectangle15RectangleInsideERKS0_
    // (SceneObject::ContainsPoint / ExpandBoundsToIncludeLocalAABB).
    bool PointInside(const Vector2& point) const;
    bool RectangleInside(const Rectangle& rect) const;
    // Proven from the golden symbols _ZNK5Caver9Rectangle19Translated
    // RectangleERKNS_7Vector2E / _ZN5Caver9Rectangle6RotateEf
    // (GroundMeshComponent::Draw). Declarations added by the
    // modelmesh slice; bodies belong to the core Rectangle queue.
    Rectangle TranslatedRectangle(const Vector2& offset) const;
    void Rotate(float angle);

    // --- Rectangle remainder queue (march 2, engine-debts) -----------
    // Const-ness / signatures proven from the golden mangled names
    // (docs/census/FUNCTION_CENSUS.tsv + Ghidra symbol list).
    float DistanceFromPoint(const Vector2& point) const;
    float DistanceFromPointSquared(const Vector2& point) const;
    bool RectInside(const Rectangle& rect) const;
    bool OverlapsRectHorizontally(const Rectangle& rect, float amount) const;
    bool OverlapsRectVertically(const Rectangle& rect, float amount) const;
    static bool TransformedRectsIntersect(const Rectangle& rect1,
                                          Vector2 position1, float rotation1,
                                          float scale1, const Rectangle& rect2,
                                          Vector2 position2, float rotation2,
                                          float scale2);
    void Inset(float amount);
    void Outset(float amount);
    void ExpandHorizontally(float amount);
    void ExpandVertically(float amount);
    Rectangle RectangleByInsetting(float amount) const;
    void FlipVertically();
    void Scale(const Vector2& scale);
    void Transform(const Vector2& position, float rotation);
    Rectangle RotatedRectangle(float angle) const;
    Rectangle ScaledRectangle(const Vector2& scale) const;
    Rectangle TransformedRectangle(const Vector2& position, float rotation) const;
    Rectangle TransformedRectangle(const Vector2& position, float rotation,
                                   const Vector2& scale) const;
    void Transform(const Matrix4& matrix);
    Rectangle TransformedRectangle(const Matrix4& matrix) const;
    void ExpandToIncludePoint(const Vector2& point);
    Rectangle Intersection(const Rectangle& rect) const;
};

} // namespace Caver

#endif
