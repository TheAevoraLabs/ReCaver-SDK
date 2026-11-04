// Recovered from libswordigo.so (1.4.13) evidence.
// Loads a POD scene (CPVRTModelPOD, owned via shared_ptr at +0) and builds
// the engine's Model/Mesh/Skeleton objects from it. CreateSkeleton also
// fills the scratch arrays and the name/index maps that
// CreateAnimationFromFile later consumes.
//
// Layout (byte offsets verified against the golden binary):
//   +0   boost::shared_ptr<CPVRTModelPOD> (px +0, control block +8)
//   +16  std::unique_ptr<Vector3[]>    per-bone bind positions
//   +24  std::unique_ptr<Quaternion[]> per-bone bind rotations
//   +32  int — bone count produced by the last CreateSkeleton
//   +40  std::map<int, int>            POD node index -> bone index
//   +64  std::vector<int>              bone index -> POD node index
//   +88  std::map<std::string, int>    node name -> bone index
//   +112 float[4] — "CenterPoint" position (xyz) + uniform scale (w),
//        initialised to {0, 0, 0, 1}
//   +128 bool initialised to true — gates texture loading in CreateMesh
#ifndef CAVER_PODLOADER_H
#define CAVER_PODLOADER_H

#include "../core/Quaternion.h"
#include "../core/Vector3.h"
#include "pvrtools/PVRTModelPOD.h"
#include <boost/shared_ptr.hpp>
#include <map>
#include <memory>
#include <string>
#include <vector>

namespace Caver {

class KeyframeAnimation;
class Mesh;
class Model;
class Skeleton;
class GLBLoader;

class PODLoader {
public:
    PODLoader();
    ~PODLoader();

    bool ReadModelFromFile(const std::string& file);
    void DestroyModel();
    bool HasSkeleton();
    bool IsGLB() const;

    boost::shared_ptr<Model> CreateModel();
    boost::shared_ptr<Mesh> CreateMesh(int meshIndex);
    boost::shared_ptr<Skeleton> CreateSkeleton();
    boost::shared_ptr<KeyframeAnimation> CreateAnimationFromFile(
        const std::string& file);

private:
    boost::shared_ptr<CPVRTModelPOD> m_pod;        // +0
    std::unique_ptr<Vector3[]> m_positions;        // +16
    std::unique_ptr<Quaternion[]> m_rotations;     // +24
    int m_boneCount;                               // +32
    std::map<int, int> m_nodeToBone;               // +40
    std::vector<int> m_boneToNode;                 // +64
    std::map<std::string, int> m_nameToBone;       // +88
    float m_centerAndScale[4];                     // +112 = {0, 0, 0, 1}
    bool m_loadTextures;                           // +128 = true
    boost::shared_ptr<GLBLoader> m_glbLoader;
};

} // namespace Caver

#endif
