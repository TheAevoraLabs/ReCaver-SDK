// Recovered from libswordigo.so (1.4.13) evidence — see docs/CORE_HEADERS.md.
// Layout: sixteen floats, column-major (m[0..3] is the first column).
#ifndef CAVER_MATRIX4_H
#define CAVER_MATRIX4_H

#include <initializer_list>

namespace Caver {

struct Quaternion;
struct Vector3;

// No Caver::Matrix3 symbol survives in the golden image apart from
// Matrix4::Matrix4(const Matrix3&) @0x53593C, so its layout is pinned by that
// body alone: nine floats, column-major, identical to the first three columns
// of a Matrix4.
struct Matrix3 {
    float m[9];
};

static_assert(sizeof(Matrix3) == 0x24, "sizeof must be 0x24");

struct Matrix4 {
    float m[16];

    static const Matrix4 identityMatrix;

    Matrix4() = default;

    // The golden Matrix4 is not an aggregate (it owns a user-provided
    // converting constructor), so brace-filled matrices need an explicit list
    // constructor. Semantics are the aggregate ones: values fill m[0..] in
    // order and the remaining slots are zeroed.
    Matrix4(std::initializer_list<float> list) {
        int i = 0;
        for (float val : list) {
            if (i < 16) m[i++] = val;
        }
        while (i < 16) m[i++] = 0.0f;
    }

    // Golden _ZN5Caver7Matrix4C1ERKNS_7Matrix3E @0x53593C: the 3x3 lands in
    // columns 0..2, the remaining column and row become (0,0,0,1).
    Matrix4(const Matrix3& matrix3);

    // Golden _ZNK5Caver7Matrix4plERKS0_ @0x5359A8 and
    // _ZNK5Caver7Matrix4miERKS0_ @0x535AC8: elementwise, const, by value.
    Matrix4 operator+(const Matrix4& other) const;
    Matrix4 operator-(const Matrix4& other) const;

    // Golden _ZN5Caver7Matrix4pLERKS0_ @0x5359E4 and
    // _ZN5Caver7Matrix4mIERKS0_ @0x535B04: in place, the golden body leaves
    // the receiver pointer untouched in x0, so they return *this.
    Matrix4& operator+=(const Matrix4& other);
    Matrix4& operator-=(const Matrix4& other);

    // Golden _ZNK5Caver7Matrix4mlEf @0x535BE8 and
    // _ZN5Caver7Matrix4mLEf @0x535C1C: every element scaled by the scalar.
    Matrix4 operator*(float scalar) const;
    Matrix4& operator*=(float scalar);

    static Matrix4 RotationX(float angle);
    static Matrix4 RotationY(float angle);
    static Matrix4 RotationZ(float angle);
    static Matrix4 RotationAxis(float angle, float x, float y, float z);

    // Proven from the golden symbols _ZN5Caver7Matrix412PreTranslate
    // ERKNS_7Vector3E / Efff (SceneObject::WorldMatrix tail-calls the
    // Vector3 form on its result).
    Matrix4& PreTranslate(const Vector3& translation);
    Matrix4& PreTranslate(float x, float y, float z);
    Matrix4& PostTranslate(const Vector3& translation);
    Matrix4& PostTranslate(float x, float y, float z);

    // Ortho(left, right, bottom, top, near, far). Signature verified from the
    // only call site in the shell (FWShell::DrawTouches @0x50BE80: sret +
    // six float arguments).
    static Matrix4 Ortho(float left, float right, float bottom, float top,
                         float nearPlane, float farPlane);
    static Matrix4 PerspectiveFov(float fov, float aspect, float nearPlane, float farPlane);

    // Proven by Mesh::TransformVertices (golden mangled
    // _ZNK5Caver7Matrix419InverseTranspose3x3Ev): const method
    // returning the inverse-transpose 3x3 as a Matrix4 by value.
    Matrix4 InverseTranspose3x3() const;

    // Proven by ModelInstance::SkinMeshes (golden mangled
    // _ZNK5Caver7Matrix46EqualsERKS0_f).
    bool Equals(const Matrix4& other, float epsilon) const;

    // Proven by SkeletonInstance::EvaluateMatrices (golden mangled
    // _ZN5Caver7Matrix422FromRotationQuaternionERKNS_10QuaternionE).
    Matrix4 FromRotationQuaternion(const Quaternion& quaternion);

    // Proven by PODLoader::CreateSkeleton (golden mangled
    // _ZNK5Caver7Matrix47InverseEv): full inverse, returned by value.
    Matrix4 Inverse() const;
    Matrix4 InverseEx() const;

    // _ZNK5Caver7Matrix411DeterminantEv @0x535E08,
    // _ZNK5Caver7Matrix414Determinant3x3Ev @0x535F68 and
    // _ZNK5Caver7Matrix410IsSingularEv @0x535EAC: all const, all recovered
    // from the golden bodies (see Matrix4.cpp).
    float Determinant() const;
    float Determinant3x3() const;
    bool IsSingular() const;

    static Matrix4 LookAt(const Vector3& eye, const Vector3& center, const Vector3& up);
};

} // namespace Caver

// Free C-style multiplies — GLOBAL scope in the golden binary:
// _Z12C_Matrix4MulPKfS0_Pf (@0x536C58, 124 B) and
// _Z19C_Matrix4Vector4MulPKfS0_Pf (@0x536CD4, 180 B). They were previously
// declared inside namespace Caver, which made every call site mangle to a
// symbol that does not exist in the golden; the gui slice was already
// shimming around this with __asm__ labels. Moved out of the namespace by
// the shell slice (wrong stub blocking parity — see SHARED.md).
void C_Matrix4Mul(const float* a, const float* b, float* out);
void C_Matrix4Vector4Mul(const float* matrix, const float* vector,
                         float* out);

#endif
