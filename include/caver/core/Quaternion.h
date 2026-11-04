// Recovered from libswordigo.so (1.4.13) evidence.
// Quaternion: four floats. The golden binary stores the SCALAR FIRST - offset
// 0 is cos(theta/2) and offsets 4/8/12 are sin(theta/2) * axis (proven by
// RotationAxisAngle @0x53856C, RotationAngle @0x538550, RotationAxis
// @0x5384EC and FromRotationMatrix @0x5385CC).
//
// The four members below are named in declaration order by the core slice, so
// for the golden layout `x` holds the scalar and `w` holds the golden z. The
// out-of-line bodies in Quaternion.cpp spell the offsets out at every use.
//
// UNRESOLVED: Matrix4::FromRotationQuaternion (@0x536BB8) and
// Camera::EvaluateViewMatrix (@0x4ECAD0) were reconstructed by the core/boot
// slices against an (x, y, z, w) reading of this struct and are internally
// consistent with each other. Renaming/renumbering the members here would
// silently change both, so the naming is left alone until that pairing is
// re-derived from the golden in one pass.
#ifndef CAVER_QUATERNION_H
#define CAVER_QUATERNION_H

namespace Caver {

struct Matrix4;
struct Vector3;

struct Quaternion {
    float x, y, z, w;

    void Normalize();
    float Length() const;
    static Quaternion FromRotationMatrix(const Matrix4& matrix);

    // _ZNK5Caver10Quaternion12RotationAxisEv @0x5384EC and
    // _ZNK5Caver10Quaternion13RotationAngleEv @0x538550.
    Vector3 RotationAxis() const;
    float RotationAngle() const;

    // _ZN5Caver10Quaternion17RotationAxisAngleEfRKNS_7Vector3E @0x53856C.
    static Quaternion RotationAxisAngle(float angle, const Vector3& axis);

    // _ZN5Caver10Quaternion5NlerpERKS0_S2_f @0x538740 and
    // _ZN5Caver10Quaternion5SlerpERKS0_S2_f @0x5387BC (empty in the golden).
    static Quaternion Nlerp(const Quaternion& start, const Quaternion& end,
                            float amount);
    static void Slerp(const Quaternion& start, const Quaternion& end,
                      float amount);
};

} // namespace Caver

#endif
