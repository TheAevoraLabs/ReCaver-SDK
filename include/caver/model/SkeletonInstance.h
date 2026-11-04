// Recovered from libswordigo.so (1.4.13) evidence.
//
// Layout (24 bytes):
//   +0   boost::shared_ptr<const Skeleton>
//   +16  BoneInstanceData*  (array of skeleton->m_numBones elements)
//
// BoneInstanceData is 220 bytes:
//   +0    Vector3    position (from the animation buffer)
//   +12   Quaternion rotation (from the animation buffer, normalized)
//   +28   Matrix4    local matrix (rotation + position)
//   +92   Matrix4    world matrix (parent world * local)
//   +156  Matrix4    skin matrix (world * bone inverse-bind)
#ifndef CAVER_SKELETONINSTANCE_H
#define CAVER_SKELETONINSTANCE_H

#include "../core/Matrix4.h"
#include "../core/Quaternion.h"
#include "../core/Vector3.h"
#include "Skeleton.h"
#include <boost/shared_ptr.hpp>

namespace Caver {

class FloatDataBuffer;
class RenderingContext;

struct BoneInstanceData {
    Vector3 m_position;      // +0
    Quaternion m_rotation;   // +12
    Matrix4 m_localMatrix;   // +28
    Matrix4 m_worldMatrix;   // +92
    Matrix4 m_skinMatrix;    // +156
};

class SkeletonInstance {
public:
    SkeletonInstance() : m_data(nullptr) {}
    ~SkeletonInstance() {
        if (m_data) {
            delete[] m_data;
            m_data = nullptr;
        }
    }

    void InitWithSkeleton(const boost::shared_ptr<const Skeleton>& skeleton);
    void ExtractDataFromBuffer(const FloatDataBuffer* buffer);
    void EvaluateMatrices();
    void Draw(RenderingContext* context, const Matrix4& matrix) const;

    boost::shared_ptr<const Skeleton> m_skeleton;  // +0
    BoneInstanceData* m_data;                      // +16
};

} // namespace Caver

#endif
