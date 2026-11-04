// Recovered from libswordigo.so (1.4.13) evidence — layout pinned by
// the ctor store map (0x3A4CA4), the dtor teardown order (0x4DF6BC),
// AddVertex/AddIndex (0x3A6768/0x3A7B08) and the InitWithMesh /
// InitWithMeshInstance / GenerateMesh bodies. sizeof 0x58.
//
//   +0   boost::shared_ptr<Mesh> — the builder's own mesh, created
//        fresh by the ctor (operator new(0x170) + Mesh::Mesh +
//        shared_ptr::reset). Every Initialize* function writes the
//        stream descriptors of THIS mesh, not of any source mesh.
//   +16  int — vertex count: set from the source mesh's numVertices
//        by InitWithMesh/InitWithMeshInstance, incremented by
//        AddVertex, read by TransformVertices (element count) and
//        GenerateMesh (CreateInterleavedData's numVertices).
//   +24  std::vector<Vertex> — 68-byte vertices (AddVertex inlines
//        the push_back fast path; element copies are 16+16+4+16+16).
//   +48  int — index count: set by the Init* functions, incremented
//        by AddIndex, GenerateMesh's CreateInterleavedData numIndices.
//   +56  std::vector<int> — the index list (AddIndex inlines the
//        vector<int> growth code in the golden).
//   +80  int m_field80 — face count (name kept: GroundMeshComponent::
//        Process reads builder->m_field80 and batches faces in
//        groups of 20). Maintained three ways, bug-for-bug: copied
//        from the source mesh's numTriangles by the Init* functions,
//        incremented by AddVertex when the mesh index stream is
//        empty and the vertex count crosses a multiple of 3, and by
//        AddIndex on every third index.
//
// Vertex (68 bytes, proven by the AddVertex copy shape, the
// TransformVertices strides — position at +0, normal at +16 — and the
// InitWithMeshBuilderFaceSubset stack copies):
//   +0  float[4] position slot (stream count floats used)
//   +16 float[4] normal slot
//   +32 unsigned char[4] colour slot
//   +36 float[4] texcoord set 0 slot
//   +52 float[4] texcoord set 1 slot
#ifndef CAVER_MESHBUILDER_H
#define CAVER_MESHBUILDER_H

#include "Mesh.h"
#include "MeshData.h"
#include <boost/shared_ptr.hpp>
#include <vector>

namespace Caver {

class Matrix4;
class MeshInstance;

class MeshBuilder {
public:
    struct Vertex {
        float position[4];        // +0
        float normal[4];          // +16
        unsigned char color[4];   // +32
        float texCoord0[4];       // +36
        float texCoord1[4];       // +52
    };

    MeshBuilder();
    ~MeshBuilder();

    void AddVertex(const Vertex& vertex);
    void AddIndex(int index);

    // _ZN5Caver11MeshBuilder12InitWithMeshERKNS_4MeshEb
    void InitWithMesh(const Mesh& mesh, bool flag);
    // _ZN5Caver11MeshBuilder20InitWithMeshInstanceEPNS_12MeshInstanceEb
    void InitWithMeshInstance(MeshInstance* instance, bool flag);
    // _ZN5Caver11MeshBuilder29InitWithMeshBuilderFaceSubsetERKS0_Pii
    void InitWithMeshBuilderFaceSubset(const MeshBuilder& other,
                                       int* faces, int count);
    // _ZN5Caver11MeshBuilder12GenerateMeshEv
    boost::shared_ptr<Mesh> GenerateMesh();

    void InitializePosition(DataType type, int count);
    void InitializeNormal(DataType type, int count);
    void InitializeVertexColor(DataType type, int count);
    void SetNumTexCoordSets(int count);
    void InitializeTexCoordSet(int set, DataType type, int count);
    void InitializeIndices(DataType type);

    void CopyDataFromMesh(float* dst, int dstStride,
                          const MeshData* src, int numVertices);
    void CopyDataFromMesh(unsigned char* dst, int dstStride,
                          const MeshData* src, int numVertices);
    void CopyDataFromMesh(int* dst, int dstStride,
                          const MeshData* src, int numVertices);
    void CopyDataToMesh(MeshData* dst, const float* src, int srcStride,
                        int numVertices);
    void CopyDataToMesh(MeshData* dst, const unsigned char* src,
                        int srcStride, int numVertices);
    void CopyDataToMesh(MeshData* dst, const int* src, int srcStride,
                        int numVertices);

    void TransformVertices(const Matrix4& matrix);

    boost::shared_ptr<Mesh> m_mesh;   // +0
    int m_vertexCount;                // +16
    std::vector<Vertex> m_vertices;   // +24
    int m_indexCount;                 // +48
    std::vector<int> m_indices;       // +56
    int m_field80;                    // +80 face count (see header note)
};

} // namespace Caver

#endif
