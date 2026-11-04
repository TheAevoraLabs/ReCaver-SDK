// Recovered from libswordigo.so (1.4.13) evidence.
//
// Layout (byte offsets verified against the golden binary):
//   +0   boost::shared_ptr<Mesh>
//   +16  bool — client-data flag: VAO buffers are created with
//        (m_field16 == 0) as the double-buffered argument, and when set
//        the client-side buffers are deleted after the first upload
//        (MeshInstance::Draw).
//   +24  MeshData — position stream (copied from the mesh by
//        InitWithMesh; field roles proven by Draw's AddArray calls:
//        32884 = GL_VERTEX_ARRAY).
//   +48  MeshData — normal stream (32885 = GL_NORMAL_ARRAY).
//   +72  MeshData* — texcoord stream, pointing into the mesh's data
//        array (32888 = GL_TEXTURE_COORD_ARRAY); only used when the
//        mesh has at least one array entry.
//   +80  MeshData — colour stream (32886 = GL_COLOR_ARRAY).
//   +104 VertexArrayObject (72 bytes, constructed in place)
//   +176 unsigned char — "buffer objects created" latch (byte-sized in
//        Draw; set to 1 on first draw).
//   +177 bool — dirty flag: swap + re-upload buffer data, then clear.
//   +178 bool — skinned-this-frame flag: Draw charges the vertex count
//        to the skinned counter and clears it.
//   +184 pointer field (role UNRECOVERED)
#ifndef CAVER_MESHINSTANCE_H
#define CAVER_MESHINSTANCE_H

#include "MeshData.h"
#include "../render/VertexArrayObject.h"
#include <boost/shared_ptr.hpp>

namespace Caver {

class Matrix4;
class Mesh;
class RenderingContext;

class MeshInstance {
public:
    MeshInstance();

    static boost::shared_ptr<MeshInstance> InstanceWithMesh(
        const boost::shared_ptr<Mesh>& mesh, bool flag);
    void InitWithMesh(const boost::shared_ptr<Mesh>& mesh, bool flag);

    void Draw(RenderingContext* context);

    void CopyAllData();
    void CopyData(const MeshData& dst, const MeshData& src);
    void SkinWithBoneMatrices(const Matrix4* matrices);
    void UpdateFaceNormals();

    // Draw counters, read by DebugInfoOverlay. Declared static: the
    // golden bodies (MeshInstance::NumStaticVerticesDrawn @0x4DFBCC,
    // NumSkinnedVerticesDrawn @0x4DFBD8, NumTrianglesDrawn @0x4DFBE4,
    // ResetDrawCounters @0x4DFBF0) only read/write the file-scope
    // counters in MeshInstance.cpp and never touch `this` — golden
    // DebugInfoOverlay::Update calls them with a stale x0. Static
    // vs non-static does not change the Itanium mangled name, so
    // this is symbol-safe; it only fixes the call shape so the
    // DebugInfoOverlay TU-local static stub can be retired by the
    // gui slice (this header's declarations now match it exactly).
    static int NumTrianglesDrawn();
    static int NumStaticVerticesDrawn();
    static int NumSkinnedVerticesDrawn();
    static void ResetDrawCounters();

    Mesh* GetMesh() const { return m_mesh.get(); }

private:
    boost::shared_ptr<Mesh> m_mesh;   // +0
    bool m_field16;                   // +16
    unsigned char m_pad17[7];
    MeshData m_positionData;          // +24
    MeshData m_normalData;            // +48
    MeshData* m_texcoordData;         // +72
    MeshData m_colorData;             // +80
    VertexArrayObject m_vao;          // +104
    unsigned char m_field176;         // +176
    bool m_field177;                  // +177
    bool m_field178;                  // +178
    unsigned char m_pad179[5];
    float* m_field184;                // +184 — face-normal array
                                      // (3 floats per triangle, lazily
                                      // allocated by UpdateFaceNormals)
};

} // namespace Caver

#endif
