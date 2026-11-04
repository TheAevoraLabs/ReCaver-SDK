// Recovered from libswordigo.so (1.4.13) evidence — the seven
// out-of-line Caver::MeshOptimizer functions at 0x4E0AE8-0x4E17A4
// (SF-2, jni-small lane). No ctor/dtor symbols exist in the golden;
// the layout below is proven by those seven bodies.
//
//   +0   boost::shared_ptr<Mesh> — InitWithMesh assigns it
//   +16  int vertex count (= Mesh+0)
//   +24  Vertex* vertex array (new[]; element 40B: Vector3 position
//        +0, int group +12 (-1 until grouped), std::vector of 8-byte
//        edge entries +16 — pass 1 of BuildEdgeAdjacencyInfo appends
//        (edgeSlot | otherGroup << 32) entries)
//   +32  int face count (= Mesh+4)
//   +40  int* face indices (3 per face; converted from the mesh index
//        stream by InitWithMesh when the mesh has one, else untouched)
//   +48  Box bounds (24B) — rebuilt by UpdateBounds
//   +72  float epsilon — BuildVertexGroups sets it to 0.001f before
//        use; name UNRECOVERED
//   +76  float cell sizes x/y/z — bounds size / 10000, clamped to
//        >= 0.003f by BuildVertexGroups
//   +88  std::map<unsigned char, int> — hash-byte -> representative
//        vertex index (tree node proven: key byte at node +28, value
//        int at node +32, node size 0x28 in BuildVertexGroups)
#ifndef CAVER_MESHOPTIMIZER_H
#define CAVER_MESHOPTIMIZER_H

#include "../core/Vector3.h"
#include <boost/shared_ptr.hpp>
#include <map>
#include <vector>

namespace Caver {

class Mesh;

// Caver::Box (also embedded at Mesh +340): Vector3 origin then
// Vector3 size (24B; the Vector3-pair form is proven by
// UpdateBounds' register-temp construction in the golden).
// ExpandToIncludePoint is out-of-line in the golden
// (_ZN5Caver3Box20ExpandToIncludePointERKNS_7Vector3E, 0x5336A8);
// its definition belongs to the core queue — declared here only.
// (Merge hand-off: dedup against the core slice's Box when it lands.)
struct Box {
    Vector3 origin;
    Vector3 size;

    void ExpandToIncludePoint(const Vector3& point);
};

class MeshOptimizer {
public:
    // Cell coordinates proven by HashForCell (0x4E0EF4): three
    // unsigned components, each hashed with its own multiplier.
    struct Cell {
        unsigned int x;
        unsigned int y;
        unsigned int z;
    };

    void InitWithMesh(const boost::shared_ptr<Mesh>& mesh);
    void UpdateBounds();
    Cell CellForPosition(const Vector3& position);
    unsigned int HashForCell(const Cell& cell);
    void BuildVertexGroups();
    void GetVertexGroupIndicesForFace(int face, int* indices);
    void BuildEdgeAdjacencyInfo();

private:
    struct Vertex {
        Vector3 position;                            // +0
        int group;                                   // +12
        std::vector<unsigned long long> m_edges;     // +16 — member
                                                     // name UNRECOVERED
    };

    boost::shared_ptr<Mesh> m_mesh;                  // +0
    int m_numVertices;                               // +16
    Vertex* m_vertices;                              // +24
    int m_numFaces;                                  // +32
    int* m_faceIndices;                              // +40
    Box m_bounds;                                    // +48
    float m_field72;                                 // +72 — UNRECOVERED
    float m_cellSizeX;                               // +76
    float m_cellSizeY;                               // +80
    float m_cellSizeZ;                               // +84
    std::map<unsigned char, int> m_groupMap;         // +88
};

} // namespace Caver

#endif
