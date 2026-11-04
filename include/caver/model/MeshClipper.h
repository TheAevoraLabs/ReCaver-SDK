// Recovered from libswordigo.so (1.4.13) evidence — the four
// out-of-line Caver::MeshClipper functions at 0x4DF690-0x4DFBCC
// (SF-2, jni-small lane).
//
// Layout (sizeof 192, proven by the ctor @0x4DF690 and
// InitWithMeshInstance @0x4DF74C):
//   +0   MeshInstance* instance (stored by InitWithMeshInstance)
//   +8   Matrix4 matrix (copied from InitWithMeshInstance's argument)
//   +72  MeshBuilder builder (88B; constructed by the ctor, which
//        initialises nothing else except the two array pointers)
//   +160 int vertex count (copied from the builder by
//        InitWithMeshInstance)
//   +168 float* per-vertex plane distances (allocated by
//        InitWithMeshInstance, filled by ClipWithPlane)
//   +176 int face count (copied from the builder)
//   +184 int* per-face signs (allocated by InitWithMeshInstance,
//        filled by ClipWithPlane, consumed by ClippedMesh)
#ifndef CAVER_MESHCLIPPER_H
#define CAVER_MESHCLIPPER_H

#include "../core/Matrix4.h"
#include "../core/Vector3.h"
#include "MeshBuilder.h"
#include <boost/shared_ptr.hpp>

namespace Caver {

class Mesh;
class MeshInstance;

class MeshClipper {
public:
    MeshClipper();

    void InitWithMeshInstance(MeshInstance* instance, const Matrix4& matrix);
    void ClipWithPlane(const Vector4& plane);
    boost::shared_ptr<Mesh> ClippedMesh(int sign);

private:
    MeshInstance* m_instance;   // +0 — UNRECOVERED name
    Matrix4 m_matrix;           // +8
    MeshBuilder m_builder;      // +72
    int m_numVertices;          // +160
    float* m_distances;         // +168
    int m_numFaces;             // +176
    int* m_faceSigns;           // +184
};

} // namespace Caver

#endif
