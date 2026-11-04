// Recovered from libswordigo.so (1.4.13) evidence — see docs/CORE_HEADERS.md.
// Layout: three floats, x then y then z (verified across the engine's math code).
#ifndef CAVER_VECTOR3_H
#define CAVER_VECTOR3_H

namespace Caver {

struct Vector4 {
    float x;
    float y;
    float z;
    float w;

    // Golden symbol _ZN5Caver7Vector49NormalizeEv (used by
    // SkeletonInstance::Draw); definition is the core lane's.
    void Normalize();
};

struct Vector3 {
    float x;
    float y;
    float z;
    Vector3() : x(0.0f), y(0.0f), z(0.0f) {}
    Vector3(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {}
    Vector3(const Vector3& o) : x(o.x), y(o.y), z(o.z) {}
    // Out-of-line in the golden binary (_ZN5Caver7Vector3C2ERKNS_7Vector4E,
    // _ZN5Caver7Vector39NormalizeEv); definitions belong to the core slice.
    // Declared here for the ParticleSystem reconstruction (systems slice,
    // 2026-10-23).
    Vector3(const Vector4& v);
    void Normalize();
    bool IsClose(const Vector3& other, float epsilon) const;
};

extern const Vector3 Vector3Zero;

} // namespace Caver

#endif
