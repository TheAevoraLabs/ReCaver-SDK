// Recovered from libswordigo.so (1.4.13) evidence — IDA exports in
// ~/workspace/analysis/swordigo/functions/Caver/GroundMeshGenerator/
// and its caller GroundMeshGeneratorComponent::GenerateMesh
// @0x3A845C, checked against the raw arm64 disassembly.
//
// GroundMeshGenerator is a small value struct (sizeof 136) built on
// the stack by GroundMeshGeneratorComponent::GenerateMesh: four raw
// input pointers, an embedded MeshBuilder, and three floats. It has
// NO out-of-line constructor (the component value-initialises the
// four pointers and runs MeshBuilder's ctor at +32); only the
// destructor @0x3A9824 is out-of-line, and it destroys exactly the
// embedded MeshBuilder (in reverse member order: the indices vector
// buffer at +88, the vertices vector buffer at +56, the mesh
// shared_ptr release at +40) — the four pointers are non-owning.
//
//   +0   Polygon* m_polygon — points at the Polygon embedded in a
//        SceneObject at +104 (Polygon layout proven by Polygon's own
//        bodies @0x537750: int count +0, std::vector<Vector2>
//        vertices +8 — count+1 entries, the last duplicating the
//        first — bool +32 UNRECOVERED, bool closed +33)
//   +8   const float* m_field8 — per-vertex depths, first array
//        (name UNRECOVERED; GenerateMesh passes two fresh random
//        depth arrays here and at +16)
//   +16  const float* m_field16 — per-vertex depths, second array
//   +24  TextureMapping* m_textureMapping
//   +32  MeshBuilder m_meshBuilder
//   +120 float m_field120 — round-hat parameter set by
//        GenerateSurfaceMeshWithRoundHat from its 2nd argument
//        (used by InsertRoundHatVertices as the hat height scale:
//        offsets of -0.4 * it and -it appear in the vertex math)
//   +124 float m_field124 — set from the 3rd argument (base height)
//   +128 float m_field128 — set from the 4th argument (added into
//        the third hat vertex's height)
// The two Generate*Mesh overloads with explicit (count, points, ...)
// arguments are STATIC (their bodies use x0 as the count and never
// touch a generator object; the component calls the instance
// overloads below). Their mangled names are identical either way.
#ifndef CAVER_GROUNDMESHGENERATOR_H
#define CAVER_GROUNDMESHGENERATOR_H

#include <boost/shared_ptr.hpp>
#include <vector>

#include "../core/Vector2.h"
#include "../core/Vector3.h"
#include "MeshBuilder.h"
#include "../physics/CollisionShape.h"

namespace Caver {

class Mesh;
class TextureMapping;

class GroundMeshGenerator {
public:
    ~GroundMeshGenerator();

    static boost::shared_ptr<Mesh> GenerateFrontMesh(
        int count, const Vector2* points, const float* depths,
        const TextureMapping* mapping);
    static boost::shared_ptr<Mesh> GenerateSurfaceMesh(
        int count, const Vector2* points, const float* depthsA,
        const float* depthsB, const TextureMapping* mapping);

    void InitializeMeshBuilder(bool withIndices);
    void InsertPlainSurfaceVertices(const Vector2& position, float depthA,
                                    float depthB, Vector2 normal,
                                    float distance);
    void InsertRoundHatVertices(const Vector2& position, float depthA,
                                float depthB, const Vector3& normalA,
                                const Vector3& normalB, float distance);
    void InsertCapForRoundHat(bool flip, const Vector2& position, float depthA,
                              float depthB, const Vector3& normalA,
                              const Vector3& normalB, float distance);
    boost::shared_ptr<Mesh> GeneratePlainSurfaceWithHatGaps(float minNormalY);
    boost::shared_ptr<Mesh> GenerateSurfaceMeshWithRoundHat(
        float minNormalY, float hatA, float hatB, float hatC);
    boost::shared_ptr<Mesh> GenerateFrontMesh();

    Polygon* m_polygon;               // +0
    const float* m_field8;            // +8 UNRECOVERED name
    const float* m_field16;           // +16 UNRECOVERED name
    TextureMapping* m_textureMapping; // +24
    MeshBuilder m_meshBuilder;        // +32
    float m_field120;                 // +120 UNRECOVERED name
    float m_field124;                 // +124 UNRECOVERED name
    float m_field128;                 // +128 UNRECOVERED name
};

} // namespace Caver

#endif
