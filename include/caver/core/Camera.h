// Partial recovery — Caver::Camera. Declared by the core slice:
// Scene holds one through a shared_ptr at +440 and TouchBegan calls
// ViewPointFromWorldPosition. The full class is recovered with the
// render/camera work and will supersede this header.
#ifndef CAVER_CAMERA_H
#define CAVER_CAMERA_H

#include "Vector2.h"
#include "Vector3.h"
#include "Rectangle.h"
#include "Ray3.h"

namespace Caver {

struct Matrix4;

class Camera {
public:
    // Proven by BackgroundComponent::Draw (golden symbol
    // _ZNK5Caver6Camera16ForwardDirectionEv): returns the camera's
    // forward vector by value. Declaration added by the modelmesh
    // slice (declaration only, no layout) — see SHARED.md.
    Vector3 ForwardDirection() const;
    // Golden _ZN5Caver6Camera13AABBForZRangeEff (non-const); declared
    // by the core slice for Scene::Update (body: Camera queue).
    Rectangle AABBForZRange(float minZ, float maxZ);
    Rectangle AABBOnZPlane(float z);
    Vector2 ViewPointFromWorldPosition(const Vector3& worldPosition, float* outDepth) const;
    // Proven from the golden symbol _ZNK5Caver6Camera28ScreenPointFromWorldPosition
    // ERKNS_7Vector3ERKNS_9RectangleEPf and its call in
    // HealthComponent::DrawHealthBar (third argument passed as null).
    Vector2 ScreenPointFromWorldPosition(const Vector3& worldPosition,
                                         const Rectangle& rect, float* outDepth) const;
    void EvaluateViewMatrix();
    float MinDistanceForVisibleAreaSize(const Vector2& size);
    // Golden _ZN5Caver6Camera25VisibleAreaSizeAtDistanceEf (@0x4EC740):
    // visible-area size at a distance along the view axis — tan(fov) *
    // distance tall, scaled by the aspect ratio wide.
    Vector2 VisibleAreaSizeAtDistance(float distance);
    void SetPerspectiveProjection(float fov, float aspect, float nearP, float farP);
    void SetOrthoProjection(float width, float height, float nearP, float farP);
    void SetAspectRatio(float aspect);
    // Golden 0x4EBC98-0x4EC904 (const-ness per the golden manglings).
    // Layout offsets follow Camera.cpp's existing conventions:
    // view-projection at +172, inverse view-projection at +384,
    // projection at +108, inverse projection at +256, ortho flag at
    // +236, aspect at +240, fov at +244, position at +16, rotation
    // quaternion at +28.
    Vector2 NormalizedScreenPointFromWorldPosition(const Vector3& worldPosition, float* outDepth) const;
    Vector3 WorldPositionFromNormalizedScreenPoint(Vector2 point, float depth) const;
    Vector3 WorldPositionFromScreenPoint(Vector2 point, const Rectangle& rect, float z) const;
    Ray3 RayFromScreenPoint(const Vector2& point) const;
    Rectangle ScreenAABBFromWorldAABB(const Rectangle& worldRect, float z, const Rectangle& screenRect) const;
    float DistanceForVisibleAreaWidth(float width);
    float DistanceForVisibleAreaHeight(float height);
    void SetProjectionMatrix(const Matrix4& matrix);
};

} // namespace Caver

#endif
