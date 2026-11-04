// Recovered from libswordigo.so (1.4.13) evidence.
// One interleaved vertex-data stream descriptor, 24 bytes:
//   +0  DataType m_type
//   +4  int m_count   (components per vertex; AllocateBuffers multiplies
//                      by SizeOfDataType(m_type))
//   +8  int m_stride  (byte stride between vertices; read directly by
//                      Mesh::GetVertexIndicesForFace for the index stream)
//   +12 int           UNRECOVERED — Mesh's constructor provably never
//                      writes this field in any of the six embedded
//                      MeshData members (store-map gaps at Mesh
//                      +20/+44/+68/+100/+124/+148), and the inline ctor
//                      below reproduces that: it initialises the other
//                      four fields only (bug-for-bug).
//   +16 unsigned char* m_data — holds a byte OFFSET into the vertex
//                      buffer until Mesh::AllocateBuffers rebases it by
//                      adding the buffer base (bug-for-bug).
#ifndef CAVER_MESHDATA_H
#define CAVER_MESHDATA_H

#include "../core/Vector3.h" // Matrix4.h (via RenderingContext.h) names
                               // Vector3 without declaring it — include-order
                               // workaround until core fixes its header
#include "../render/RenderingContext.h" // for Caver::DataType (its real home
                                          // is unconfirmed; see the board)

namespace Caver {

namespace Proto {
class MeshData;
}

// Free helper (40-byte golden function; implemented outside this slice).
unsigned int SizeOfDataType(DataType type);

struct MeshData {
    DataType m_type;       // +0
    int m_count;           // +4
    int m_stride;          // +8
    int m_field12;         // +12 — UNRECOVERED (see header comment)
    unsigned char* m_data; // +16

    MeshData()
        : m_type(DataType(0)), m_count(0), m_stride(0), m_data(0) {}

    unsigned int valueSize() const;

    void LoadFromProtobufMessage(const Proto::MeshData& message,
                                 unsigned char* data);
    void SaveToProtobufMessage(Proto::MeshData* message,
                               unsigned char* data) const;
};

} // namespace Caver

#endif
