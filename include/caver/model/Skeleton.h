// Recovered from libswordigo.so (1.4.13) evidence.
// A bone table: count at +0, array of 160-byte Bone records at +8.
// A Bone's first member is its name string (the golden Skeleton(int)
// constructor's per-element initialisation zeroes exactly the 24 bytes
// of a std::string at the head of each 160-byte element).
// Bone fields proven by SkeletonInstance::EvaluateMatrices: parent
// index at +24, inverse-bind matrix (16 floats) at +92.
// Skeleton fields after the bone table are the FloatDataBuffer channel
// descriptors the instance readers use (proven by
// SkeletonInstance::ExtractDataFromBuffer / InitWithSkeleton):
//   +20  position channel offset (floats into the buffer)
//   +24  position channel stride (floats per bone)
//   +32  rotation channel offset
//   +36  rotation channel stride
//   +40  float[3] skin offset: InitWithSkeleton seeds every element's
//        skin matrix with the identity except elements 12/13/14, which
//        take these three floats
// (+16 and +28 are not read by any recovered consumer yet.)
#ifndef CAVER_SKELETON_H
#define CAVER_SKELETON_H

#include "../core/Matrix4.h"
#include <string>

namespace Caver {

struct Bone {
    std::string name;              // +0
    int m_parentIndex;             // +24 (-1 for roots)
    unsigned char m_rest28[64];    // +28..91
    Matrix4 m_inverseBindMatrix;   // +92 (16 floats)
    unsigned char m_rest156[4];    // +156..159

    Matrix4& BindMatrix() { return *reinterpret_cast<Matrix4*>(&m_rest28[0]); }
    const Matrix4& BindMatrix() const { return *reinterpret_cast<const Matrix4*>(&m_rest28[0]); }
};

class Skeleton {
public:
    explicit Skeleton(int numBones);
    ~Skeleton();

    int BoneIndexForName(const std::string& name) const;

    int m_numBones;          // +0
    Bone* m_bones;           // +8
    int m_field16;           // +16
    int m_posChannelOffset;  // +20
    int m_posChannelStride;  // +24
    int m_field28;           // +28
    int m_rotChannelOffset;  // +32
    int m_rotChannelStride;  // +36
    float m_skinOffset[3];   // +40..51
    unsigned char m_rest52[12]; // +52..63
};

} // namespace Caver

#endif
