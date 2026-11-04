// Recovered Caver free functions with no home translation unit.
//
// These are out-of-line in the golden binary (triangulation helpers,
// geometry primitives, string/enum conversions, POD loading, path
// helpers, texture sort predicates) but have no class or TU of their
// own in the tree, so they live here. Every body is recovered from
// the IDA arm64 decompilation cited in the comment above it; see
// docs/unimplemented.tsv for the golden addresses.
//
// Two signatures carry deliberate notes:
//   - StringForMovementBehavior takes the golden enum
//     EntityControllerComponent::MovementBehavior, which has no
//     C++ declaration in the tree yet (only the proto enum, whose
//     values are 1-based; the golden enum is 0-based: none=0,
//     roam=1, follow=2, fight=3). The parameter is declared as int
//     with an __asm__ label carrying the exact golden mangling, so
//     the emitted symbol matches; callers must cast from the enum
//     once it lands.
//   - MovementBehaviourForString returns that same enum; with no
//     enum declaration available it returns int with the same
//     0..3 values. Return types do not affect mangling, so the
//     symbol is exact either way.
#ifndef CAVER_CAVERFREEFUNCTIONS_H
#define CAVER_CAVERFREEFUNCTIONS_H
#include "caver/core/AsmLabel.h"

#include "Vector2.h"

#include <string>

#include <boost/intrusive_ptr.hpp>

#include "../../../thirdparty/pvrtools/PVRTVertex.h"

class CPVRTModelPOD;

namespace Caver {

class Texture;
struct Polygon;

// --- Triangulation helpers (golden 0x3ABFEC-0x3AC4C4) -----------------
// VertexList_ / VertexListNode_ are the golden's own type names
// (trailing underscore included): a circular doubly-linked list of
// polygon vertices. Node: prev +0, next +8, index +16,
// position +20 (32 bytes). List: node array +0, head +8,
// count +16 (24 bytes).
struct VertexListNode_ {
    VertexListNode_* m_prev; // +0
    VertexListNode_* m_next; // +8
    int m_index;             // +16
    Vector2 m_position;      // +20
};

struct VertexList_ {
    VertexListNode_* m_nodes; // +0
    VertexListNode_* m_head;  // +8
    int m_count;              // +16
};

int PrevIndex(int index, int count);
int NextIndex(int index, int count);
bool IsConvexVertex(VertexList_* list, VertexListNode_* node);
VertexList_* BuildVertexList(Vector2* points, int count);
void DestroyVertexList(VertexList_* list);
VertexListNode_* RemoveVertex(VertexList_* list, VertexListNode_* node);
VertexList_* BuildConcaveList(Vector2* points, int count, VertexList_* list);
bool PointInsideTriangle(Vector2 a, Vector2 b, Vector2 c, Vector2 point);
bool IsAnEar(VertexList_* list, VertexListNode_* node, VertexList_* concaveList);
int TestTriangulation();

// --- Geometry primitives (golden 0x5338C4-0x539598) -------------------
float PolygonArea(const Vector2* points, int count);
bool PolygonIsCCW(const Vector2* points, int count);
bool PolygonSelfIntersects(const Vector2* points, int count);
bool LineSegmentsIntersect(const Vector2& a, const Vector2& b,
                           const Vector2& c, const Vector2& d,
                           float* outT1, float* outT2, bool infiniteLines);
bool RectIntersectsWith1DLine(float lineStart, float lineEnd,
                              float slabMin, float slabMax,
                              float* tMin, float* tMax);
bool IsCollisionNormalValidForPolygonLineSegment(const Vector2& normal,
                                                 const Polygon& polygon,
                                                 int index);
float CubicRoot(float value);

// --- String / enum conversions (golden 0x366A84, 0x366B18) ------------
std::string StringForMovementBehavior(int behavior)
    CAVER_ASM_LABEL("_ZN5Caver25StringForMovementBehaviorENS_25EntityControllerComponent16MovementBehaviorE");
int MovementBehaviourForString(const std::string& str);

// --- Small value helpers (golden 0x549DF8, 0x54F6EC, 0x54F720) ---------
bool IsWhiteSpace(char c);
unsigned int FloatToBits(float value, unsigned int bits);
float BitsToFloat(unsigned int value, unsigned int bits);

// --- File / program stubs (golden 0x53BD1C, 0x53BD2C) ------------------
// The golden bodies of both are a bare `return 0`; these return
// false, matching the decompiled bodies exactly.
bool CopyFile(const std::string& from, const std::string& to);
bool ExecuteProgram(const std::string& program, const std::string& arg1,
                    const std::string& arg2, bool flag1, bool flag2,
                    bool flag3, const std::string& arg3,
                    const std::string& arg4, const std::string& arg5);

// --- Texture sort predicates (golden 0x551804, 0x551AE0) ---------------
bool TextureLoadTimeSortPredicate(
    const boost::intrusive_ptr<Texture>& a,
    const boost::intrusive_ptr<Texture>& b);
bool TextureSizeSortPredicate(const Texture* a, const Texture* b);

// --- Texture conversion (golden 0x555DDC, 0x5555DE4) -------------------
// ConversionRequired is false on this platform in the golden, and
// ConvertToFileBuffer16 is an empty body there for the same reason.
bool ConversionRequired();
void ConvertToFileBuffer16(unsigned char* buffer, int size);

// --- File contents / POD (golden 0x513CD0, 0x4E3CC8, 0x4E5834) ---------
bool LoadContentsOfFile(const std::string& path, std::string* out);
// Returns true on success (ReadFromMemory == PVR_SUCCESS), false
// otherwise. The golden leaks the asset buffer on the failure path;
// that quirk is reproduced, not fixed, so the control flow matches.
bool ReadPODModelFromFile(CPVRTModelPOD* model, const std::string& file);
// POD data type -> GL enum (0 for types with no GL equivalent),
// from the golden's static table at 0x26DBD0.
int MeshValueTypeFromPODType(EPVRTDataType type);

} // namespace Caver

#endif // CAVER_CAVERFREEFUNCTIONS_H
