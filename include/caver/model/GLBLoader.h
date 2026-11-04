// GLB (glTF 2.0 Binary) Ingest Engine for CaverEngine / ReCaver.
// Loads .glb assets, skeletons, and animation tracks into native Caver types.
// Credit: Quantum Creeper / Aevora Labs.
// Research & architecture inspired by sw-glb by raijin.
#ifndef CAVER_GLBLOADER_H
#define CAVER_GLBLOADER_H

#include <boost/shared_ptr.hpp>
#include <string>
#include <vector>
#include <cstdint>
#include <memory>

struct cgltf_data;
struct cgltf_skin;
struct cgltf_primitive;
struct cgltf_node;
struct cgltf_animation;

namespace Caver {

class KeyframeAnimation;
class Mesh;
class Model;
class Skeleton;

struct GLBFlattenedPrimitive {
    cgltf_primitive* prim;
    cgltf_node* node;
    float worldMatrix[16];
    std::string name;
};

class GLBLoader {
public:
    GLBLoader();
    ~GLBLoader();

    bool ReadModelFromFile(const std::string& file);
    bool ReadModelFromMemory(const void* data, size_t size, const std::string& name = "");
    void DestroyModel();

    bool HasSkeleton() const;
    int BoneCount() const;
    int PrimitiveCount() const;
    const std::string& Name() const { return m_name; }

    boost::shared_ptr<Model> CreateModel();
    boost::shared_ptr<Mesh> CreateMesh(int meshIndex);
    boost::shared_ptr<Skeleton> CreateSkeleton();

    // Animation baking
    boost::shared_ptr<KeyframeAnimation> CreateAnimationFromFile(const std::string& file);
    boost::shared_ptr<KeyframeAnimation> CreateAnimationByName(const std::string& clipName);
    boost::shared_ptr<KeyframeAnimation> CreateAnimationByIndex(int animIndex);
    boost::shared_ptr<KeyframeAnimation> CreateRestPose();

    std::vector<std::string> AnimationNames() const;

private:
    void CollectNodes(cgltf_node* node, const float* parentMatrix);
    void SetupPrimitives();
    void SampleNodeTRS(cgltf_animation* anim, cgltf_node* node, float time, float* posOut, float* rotOut);
    boost::shared_ptr<KeyframeAnimation> BakeExternalAnimation(cgltf_animation* anim);

    std::string m_name;
    std::string m_textureBaseName;
    std::vector<uint8_t> m_buffer;
    cgltf_data* m_data;
    cgltf_skin* m_skin;
    std::vector<GLBFlattenedPrimitive> m_primitives;
};

} // namespace Caver

#endif // CAVER_GLBLOADER_H
