// Recovered from libswordigo.so (1.4.13) evidence — layout pinned by
// Mesh::Mesh (252B) store map, Mesh::~Mesh (280B) teardown order and
// Mesh::AllocateBuffers (564B). sizeof 368 (last member ends at +364).
//
//   +0   int m_numVertices — AllocateBuffers gates on it (>= 1) and
//        strides by (m_numVertices - 1); MeshInstance::Draw adds it to
//        the vertices-drawn counters.
//   +4   int m_numTriangles — MeshInstance::Draw adds it to the
//        triangles counter and draws 3 * m_numTriangles vertices.
//   +8   MeshData m_indexData — the index stream: type at +8, stride
//        at +16, CPU data pointer at +24 (GetVertexIndicesForFace).
//   +32/+56/+88/+112/+136 — five more embedded MeshData streams.
//        AllocateBuffers' size list uses +32, +56, the heap array, +88
//        (in that order); +112/+136 are not in that list (their consumer
//        is CreateInterleavedData — UNRECOVERED so far).
//   +80  heap MeshData array, element count at +336. Its delete[]
//        runs in member-teardown position (last in ~Mesh), and the
//        buffer members at +288/+304 are likewise deleted between the
//        vector and weak_ptr teardowns — so all three are RAII array
//        members (unique_ptr<T[]> shape), not raw pointers freed by a
//        user dtor body.
//   +160 MeshMaterial (88B) — its texture member is the pointer at
//        Mesh +216 that GroundMeshComponent::DrawMesh binds.
//   +248 boost::shared_array<unsigned short> — the ctor creates the
//        null-pointer control block (sp_counted_impl_pd with
//        checked_array_deleter<unsigned short>), i.e. the member is
//        initialised from a null unsigned short* (bug-for-bug).
//   +264 boost::weak_ptr — target type UNRECOVERED (weak_ptr<Mesh>
//        used; the dtor's release shape is type-erased).
//   +312 std::vector — element type UNRECOVERED (the golden dtor only
//        deallocates: trivial elements; int is a placeholder).
//   +340 six floats — role UNRECOVERED (bounding volume is the obvious
//        candidate; UpdateBoundingBox will arbitrate).
#ifndef CAVER_MESH_H
#define CAVER_MESH_H

#include "MeshData.h"
#include "MeshMaterial.h"
#include <boost/scoped_array.hpp>
#include <boost/shared_array.hpp>
#include <boost/weak_ptr.hpp>
#include <memory>
#include <vector>

namespace Caver {

class Matrix4;
class MeshInstance;
class GLBLoader;

namespace Proto {
class Mesh;
}

// Array holder whose codegen matches the golden Mesh teardown exactly:
// its destructor delete[]s without nulling the slot, and reset() stores
// the new buffer before deleting the old (the Alloc*Buffer shape).
// std::unique_ptr under NDK r27's libc++ nulls the slot on destroy and
// boost::scoped_array's reset deletes first — neither reproduces both
// golden shapes. (The original holder type is UNRECOVERED; this is the
// minimal shape consistent with all three graded functions.)
template <typename T>
struct MeshOwnedArray {
    T* ptr;
    MeshOwnedArray() : ptr(0) {}
    ~MeshOwnedArray() { if (ptr) delete[] ptr; }
    void reset(T* p) {
        T* old = ptr;
        ptr = p;
        if (old)
            delete[] old;
    }
    T* get() const { return ptr; }
    T& operator[](std::size_t i) const { return ptr[i]; }

private:
    MeshOwnedArray(const MeshOwnedArray&);
    MeshOwnedArray& operator=(const MeshOwnedArray&);
};

class Mesh {
    friend class MeshInstance;

public:
    Mesh();
    ~Mesh();

    void AllocVertexBuffer(int size);
    void AllocIndexBuffer(int size);
    void AllocateBuffers(int numIndices);

    void GetVertexIndicesForFace(int face, int* indices);

    void LoadFromProtobufMessage(const Proto::Mesh& message);
    void SaveToProtobufMessage(Proto::Mesh* message) const;

    void CreateInterleavedData(int numVertices, int numTriangles,
                               int numIndices);
    void TransformVertices(const Matrix4& matrix);
    void NormalizeNormals();
    void GetDataStructs(MeshData** structs, int* count);
    void SetNumTexCoordSets(int count);
    void UpdateBoundingBox();
    void AddTriangleStrip(int numVertices);
    int numIndices() const;

    Texture* GetTexture() const { return m_material.m_texture.get(); }

    friend class PODLoader;
    friend class GLBLoader;
    friend class GroundMeshComponent;
    friend class MeshBuilder;

private:
    int m_numVertices;                                  // +0
    int m_numTriangles;                                 // +4
    MeshData m_indexData;                               // +8
    MeshData m_data32;                                  // +32
    MeshData m_data56;                                  // +56
    boost::scoped_array<MeshData> m_dataArray;          // +80
    MeshData m_data88;                                  // +88
    MeshData m_data112;                                 // +112
    MeshData m_data136;                                 // +136
    MeshMaterial m_material;                            // +160
    boost::shared_array<unsigned short> m_field248;     // +248
    // +264: weak_ptr to the cached MeshInstance created with
    // flag=true (proven by MeshInstance::InstanceWithMesh session 3:
    // it locks this weak_ptr and stores the new instance back).
    boost::weak_ptr<MeshInstance> m_field264;           // +264
    int m_vertexBufferSize;                             // +280
    MeshOwnedArray<unsigned char> m_vertexBuffer;       // +288
    int m_indexBufferSize;                              // +296
    MeshOwnedArray<unsigned char> m_indexBuffer;        // +304
    std::vector<int> m_field312;                        // +312
    int m_numDataArray;                                 // +336
    float m_field340[6];                                // +340
};

} // namespace Caver

#endif
