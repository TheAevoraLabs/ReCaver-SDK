// Recovered from libswordigo.so (1.4.13) evidence — layout pinned by
// the ctor @0x4445D4 and Draw @0x4446B8 (jni-small SF-1):
//   +0  int m_vertexCount (= 6)
//   +8  Vertex* m_vertices (new[](0x60): 6 vertices of 16 bytes —
//       Vector3 position + 4 colour bytes)
//   +16 int m_triangleCount (= 4)
//   +24 unsigned short* m_indices (new[](0x18): 12 indices)
// sizeof 32. Member names UNRECOVERED beyond these roles.
#ifndef CAVER_SCENEEDGEFADE_H
#define CAVER_SCENEEDGEFADE_H

namespace Caver {

class RenderingContext;
struct Matrix4;

class SceneEdgeFade {
public:
    struct Vertex {
        float x, y, z;
        unsigned char r, g, b, a;
    };

    SceneEdgeFade();
    void Draw(RenderingContext* context, const Matrix4& matrix);

    int m_vertexCount;              // +0
    Vertex* m_vertices;             // +8
    int m_triangleCount;            // +16
    unsigned short* m_indices;      // +24
};

} // namespace Caver

#endif
