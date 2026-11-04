// CaverEngine Raymath Integration
// Credit: Quantum Creeper / Aevora Labs
// Wraps Raylib's raymath v2.0 routines into native Caver types.

#ifndef CAVER_RAYMATH_H
#define CAVER_RAYMATH_H

#include "Vector2.h"
#include "Vector3.h"
#include "Quaternion.h"
#include "Matrix4.h"

#include <cstring>
#include <cmath>

#define RAYMATH_STATIC_INLINE
#define RAYMATH_DISABLE_CPP_OPERATORS

namespace Caver {
namespace RaymathInternal {
#include "raymath_raw.h"
} // namespace RaymathInternal

namespace Math {

// --- Converters between Caver and Raymath Internal ---
inline RaymathInternal::Vector2 ToRM(const Vector2& v) {
    RaymathInternal::Vector2 r = { v.x, v.y };
    return r;
}
inline Vector2 FromRM(const RaymathInternal::Vector2& v) {
    return Vector2(v.x, v.y);
}

inline RaymathInternal::Vector3 ToRM(const Vector3& v) {
    RaymathInternal::Vector3 r = { v.x, v.y, v.z };
    return r;
}
inline Vector3 FromRM(const RaymathInternal::Vector3& v) {
    return Vector3(v.x, v.y, v.z);
}

inline RaymathInternal::Quaternion ToRM(const Quaternion& q) {
    RaymathInternal::Quaternion r = { q.x, q.y, q.z, q.w };
    return r;
}
inline Quaternion FromRM(const RaymathInternal::Quaternion& q) {
    Quaternion r;
    r.x = q.x; r.y = q.y; r.z = q.z; r.w = q.w;
    return r;
}

inline RaymathInternal::Matrix ToRM(const Matrix4& m) {
    RaymathInternal::Matrix r;
    std::memcpy(&r, m.m, sizeof(r));
    return r;
}
inline Matrix4 FromRM(const RaymathInternal::Matrix& m) {
    Matrix4 r;
    std::memcpy(r.m, &m, sizeof(r.m));
    return r;
}

// --- Float utilities ---
inline float Clamp(float value, float min, float max) {
    return RaymathInternal::Clamp(value, min, max);
}
inline float Lerp(float start, float end, float amount) {
    return RaymathInternal::Lerp(start, end, amount);
}
inline float Normalize(float value, float start, float end) {
    return RaymathInternal::Normalize(value, start, end);
}
inline float Remap(float value, float inputStart, float inputEnd, float outputStart, float outputEnd) {
    return RaymathInternal::Remap(value, inputStart, inputEnd, outputStart, outputEnd);
}
inline float Wrap(float value, float min, float max) {
    return RaymathInternal::Wrap(value, min, max);
}
inline bool FloatEquals(float x, float y) {
    return RaymathInternal::FloatEquals(x, y) != 0;
}

// --- Vector2 routines ---
inline Vector2 Vector2Zero() { return Vector2(0.0f, 0.0f); }
inline Vector2 Vector2One() { return Vector2(1.0f, 1.0f); }
inline Vector2 Vector2Add(const Vector2& v1, const Vector2& v2) {
    return FromRM(RaymathInternal::Vector2Add(ToRM(v1), ToRM(v2)));
}
inline Vector2 Vector2Subtract(const Vector2& v1, const Vector2& v2) {
    return FromRM(RaymathInternal::Vector2Subtract(ToRM(v1), ToRM(v2)));
}
inline float Vector2Length(const Vector2& v) {
    return RaymathInternal::Vector2Length(ToRM(v));
}
inline float Vector2LengthSqr(const Vector2& v) {
    return RaymathInternal::Vector2LengthSqr(ToRM(v));
}
inline float Vector2Dot(const Vector2& v1, const Vector2& v2) {
    return RaymathInternal::Vector2DotProduct(ToRM(v1), ToRM(v2));
}
inline float Vector2Distance(const Vector2& v1, const Vector2& v2) {
    return RaymathInternal::Vector2Distance(ToRM(v1), ToRM(v2));
}
inline float Vector2Angle(const Vector2& v1, const Vector2& v2) {
    return RaymathInternal::Vector2Angle(ToRM(v1), ToRM(v2));
}
inline Vector2 Vector2Scale(const Vector2& v, float scale) {
    return FromRM(RaymathInternal::Vector2Scale(ToRM(v), scale));
}
inline Vector2 Vector2Normalize(const Vector2& v) {
    return FromRM(RaymathInternal::Vector2Normalize(ToRM(v)));
}
inline Vector2 Vector2Rotate(const Vector2& v, float angle) {
    return FromRM(RaymathInternal::Vector2Rotate(ToRM(v), angle));
}
inline Vector2 Vector2Lerp(const Vector2& v1, const Vector2& v2, float amount) {
    return FromRM(RaymathInternal::Vector2Lerp(ToRM(v1), ToRM(v2), amount));
}
inline Vector2 Vector2Reflect(const Vector2& v, const Vector2& normal) {
    return FromRM(RaymathInternal::Vector2Reflect(ToRM(v), ToRM(normal)));
}

// --- Vector3 routines ---
inline Vector3 Vector3Zero() { return Vector3(0.0f, 0.0f, 0.0f); }
inline Vector3 Vector3One() { return Vector3(1.0f, 1.0f, 1.0f); }
inline Vector3 Vector3Add(const Vector3& v1, const Vector3& v2) {
    return FromRM(RaymathInternal::Vector3Add(ToRM(v1), ToRM(v2)));
}
inline Vector3 Vector3Subtract(const Vector3& v1, const Vector3& v2) {
    return FromRM(RaymathInternal::Vector3Subtract(ToRM(v1), ToRM(v2)));
}
inline Vector3 Vector3Scale(const Vector3& v, float scale) {
    return FromRM(RaymathInternal::Vector3Scale(ToRM(v), scale));
}
inline Vector3 Vector3Cross(const Vector3& v1, const Vector3& v2) {
    return FromRM(RaymathInternal::Vector3CrossProduct(ToRM(v1), ToRM(v2)));
}
inline float Vector3Dot(const Vector3& v1, const Vector3& v2) {
    return RaymathInternal::Vector3DotProduct(ToRM(v1), ToRM(v2));
}
inline float Vector3Length(const Vector3& v) {
    return RaymathInternal::Vector3Length(ToRM(v));
}
inline float Vector3LengthSqr(const Vector3& v) {
    return RaymathInternal::Vector3LengthSqr(ToRM(v));
}
inline float Vector3Distance(const Vector3& v1, const Vector3& v2) {
    return RaymathInternal::Vector3Distance(ToRM(v1), ToRM(v2));
}
inline float Vector3Angle(const Vector3& v1, const Vector3& v2) {
    return RaymathInternal::Vector3Angle(ToRM(v1), ToRM(v2));
}
inline Vector3 Vector3Normalize(const Vector3& v) {
    return FromRM(RaymathInternal::Vector3Normalize(ToRM(v)));
}
inline Vector3 Vector3Lerp(const Vector3& v1, const Vector3& v2, float amount) {
    return FromRM(RaymathInternal::Vector3Lerp(ToRM(v1), ToRM(v2), amount));
}
inline Vector3 Vector3Reflect(const Vector3& v, const Vector3& normal) {
    return FromRM(RaymathInternal::Vector3Reflect(ToRM(v), ToRM(normal)));
}
inline Vector3 Vector3RotateByQuaternion(const Vector3& v, const Quaternion& q) {
    return FromRM(RaymathInternal::Vector3RotateByQuaternion(ToRM(v), ToRM(q)));
}
inline Vector3 Vector3RotateByAxisAngle(const Vector3& v, const Vector3& axis, float angle) {
    return FromRM(RaymathInternal::Vector3RotateByAxisAngle(ToRM(v), ToRM(axis), angle));
}

// --- Quaternion routines ---
inline Quaternion QuaternionIdentity() {
    return FromRM(RaymathInternal::QuaternionIdentity());
}
inline float QuaternionLength(const Quaternion& q) {
    return RaymathInternal::QuaternionLength(ToRM(q));
}
inline Quaternion QuaternionNormalize(const Quaternion& q) {
    return FromRM(RaymathInternal::QuaternionNormalize(ToRM(q)));
}
inline Quaternion QuaternionInvert(const Quaternion& q) {
    return FromRM(RaymathInternal::QuaternionInvert(ToRM(q)));
}
inline Quaternion QuaternionMultiply(const Quaternion& q1, const Quaternion& q2) {
    return FromRM(RaymathInternal::QuaternionMultiply(ToRM(q1), ToRM(q2)));
}
inline Quaternion QuaternionSlerp(const Quaternion& q1, const Quaternion& q2, float amount) {
    return FromRM(RaymathInternal::QuaternionSlerp(ToRM(q1), ToRM(q2), amount));
}
inline Quaternion QuaternionNlerp(const Quaternion& q1, const Quaternion& q2, float amount) {
    return FromRM(RaymathInternal::QuaternionNlerp(ToRM(q1), ToRM(q2), amount));
}
inline Quaternion QuaternionFromEuler(float pitch, float yaw, float roll) {
    return FromRM(RaymathInternal::QuaternionFromEuler(pitch, yaw, roll));
}
inline Vector3 QuaternionToEuler(const Quaternion& q) {
    return FromRM(RaymathInternal::QuaternionToEuler(ToRM(q)));
}

// --- Matrix routines ---
inline Matrix4 MatrixIdentity() {
    return FromRM(RaymathInternal::MatrixIdentity());
}
inline Matrix4 MatrixMultiply(const Matrix4& left, const Matrix4& right) {
    return FromRM(RaymathInternal::MatrixMultiply(ToRM(left), ToRM(right)));
}
inline Matrix4 MatrixTranspose(const Matrix4& mat) {
    return FromRM(RaymathInternal::MatrixTranspose(ToRM(mat)));
}
inline Matrix4 MatrixInvert(const Matrix4& mat) {
    return FromRM(RaymathInternal::MatrixInvert(ToRM(mat)));
}
inline Matrix4 MatrixTranslate(float x, float y, float z) {
    return FromRM(RaymathInternal::MatrixTranslate(x, y, z));
}
inline Matrix4 MatrixRotate(const Vector3& axis, float angle) {
    return FromRM(RaymathInternal::MatrixRotate(ToRM(axis), angle));
}
inline Matrix4 MatrixScale(float x, float y, float z) {
    return FromRM(RaymathInternal::MatrixScale(x, y, z));
}
inline Matrix4 MatrixLookAt(const Vector3& eye, const Vector3& target, const Vector3& up) {
    return FromRM(RaymathInternal::MatrixLookAt(ToRM(eye), ToRM(target), ToRM(up)));
}
inline Matrix4 MatrixPerspective(float fovY, float aspect, float nearPlane, float farPlane) {
    return FromRM(RaymathInternal::MatrixPerspective(fovY, aspect, nearPlane, farPlane));
}
inline Matrix4 MatrixOrtho(float left, float right, float bottom, float top, float nearPlane, float farPlane) {
    return FromRM(RaymathInternal::MatrixOrtho(left, right, bottom, top, nearPlane, farPlane));
}

} // namespace Math
} // namespace Caver

#endif // CAVER_RAYMATH_H
