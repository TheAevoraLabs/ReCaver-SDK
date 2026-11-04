// Recovered from libswordigo.so (1.4.13) evidence.
//
// Layout (byte offsets verified against the golden binary):
//   +0   boost::shared_ptr<const Model>
//   +16  bool — set once skeleton data has been extracted
//   +24  boost::shared_ptr<SkeletonInstance>
//   +40  boost::shared_ptr<MeshInstance>* — cookie-counted array,
//        one instance per mesh of the model, owned via delete[]
//   +48  float — colour scale (read as float bits by Draw)
//   +52  float[4] colour tint, initialised to {1, 1, 1, 1}
//   +68  bool — bind the mesh texture in Draw, initialised to 1
//   +69  bool — skin matrices dirty, initialised to 0
//   +72  Matrix4 — last matrix passed to SkinMeshes
// sizeof = 136. The destructor's body is exactly the member teardown
// (instance array delete[], skeleton instance, model).
#ifndef CAVER_MODELINSTANCE_H
#define CAVER_MODELINSTANCE_H

#include "../core/Matrix4.h"
#include <boost/shared_ptr.hpp>

namespace Caver {

class FloatDataBuffer;
class Model;
class RenderingContext;
class MeshInstance;
class SkeletonInstance;

class ModelInstance {
public:
    ModelInstance();
    ~ModelInstance();

    void InitWithModel(const boost::shared_ptr<const Model>& model);
    void Draw(RenderingContext* context);
    void SkinMeshes(const Matrix4& matrix);
    void ExtractSkeletonDataFromBuffer(const FloatDataBuffer* buffer);
    // Golden BoneControlledCollisionShapeComponent::Update (0x326B44)
    // reads this flag at model-instance +16 and returns early when
    // it is false, in both the existing-instance and post-Prepare
    // paths: it is the "skeleton data extracted and matrices
    // evaluated" flag, set only in ExtractSkeletonDataFromBuffer
    // after EvaluateMatrices().
    bool SkeletonEvaluated() const { return m_field16; }

private:
    boost::shared_ptr<const Model> m_model;            // +0
    bool m_field16;                                    // +16
    unsigned char m_pad17[7];
    boost::shared_ptr<SkeletonInstance> m_skeletonInstance; // +24
    boost::shared_ptr<MeshInstance>* m_meshInstances;  // +40
    float m_field48;                                   // +48
    float m_color[4];                                  // +52 = {1, 1, 1, 1}
    bool m_bindTexture;                                // +68 = 1
    bool m_skinDirty;                                  // +69 = 0
    unsigned char m_pad70[2];
    Matrix4 m_lastSkinMatrix;                          // +72
};

} // namespace Caver

#endif
