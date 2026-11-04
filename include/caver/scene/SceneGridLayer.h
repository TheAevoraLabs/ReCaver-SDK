// Recovered from libswordigo.so (1.4.13) evidence — IDA exports in
// reference/ida-arm64/functions/Caver/SceneGridLayer/ (12 own bodies,
// 0x4FE6AC..0x4FFA58) plus the inlined constructor shown by
// SceneGrid::AddLayer @0x4FD8EC.
//
// SceneGridLayer is one depth slab of a SceneGrid: a uniform grid of
// SceneGridCell buckets covering the scene bounds, with every cell
// pointing at the SceneGridEntry records whose AABB overlaps it. The
// layer also owns the line-list vertex data and the per-cell entry
// count overlay text used by the debug draw.
//
// Layout (sizeof 0x158 = 344, proven by the operator new(0x158) in
// SceneGrid::AddLayer, by every body's member accesses, and by the
// FontText subobject at +104 with its fixed 0xF0 size):
//   +0    float m_minDepth
//   +4    float m_maxDepth
//   +8    Rectangle m_bounds          (x, y, width, height)
//   +24   float m_cellWidth
//   +28   float m_cellHeight
//   +32   float m_invCellWidth        (1 / m_cellWidth)
//   +36   float m_invCellHeight       (1 / m_cellHeight)
//   +40   int m_cellCountX            (ceil(width  / m_cellWidth))
//   +44   int m_cellCountY            (ceil(height / m_cellHeight))
//   +48   SceneGridCell* m_cells      (column-major: x + m_cellCountX*y)
//   +56   Rectangle m_visibleBounds   (set from Camera::AABBForZRange)
//   +72   bool m_prebuiltVertexData   (see the note below)
//   +80   std::vector<Vector3> m_vertices
//   +104  FontText m_text
//
// The +72 flag is only ever READ: Init calls CreateVertexData when it
// is set, Draw calls it when it is clear, and the constructor zeroes
// it. No golden body writes it, so in practice it stays false and
// Draw rebuilds the line list on every call. The name is descriptive,
// not recovered.
//
// NON-POLYMORPHIC: no _ZTV/_ZTI/_ZTS for SceneGridLayer exists.
#ifndef CAVER_SCENEGRIDLAYER_H
#define CAVER_SCENEGRIDLAYER_H

#include <vector>

#include "../core/Rectangle.h"
#include "../core/Vector3.h"
#include "../gui/FontText.h"
#include "FastVector.h"
#include "SceneGridCell.h"

namespace Caver {

class RenderingContext;
struct Matrix4;
struct SceneGridEntry;

class SceneGridLayer {
public:
    // Header-inline in the golden (no constructor symbol exists):
    // SceneGrid::AddLayer @0x4FD8EC inlines the whole initialisation,
    // zeroing every scalar and both rectangles before Init runs. The
    // FontText member's own constructor supplies the +19/+44 alpha
    // bytes and the +232 = -1 glyph index the golden writes there.
    SceneGridLayer()
        : m_minDepth(0.0f),
          m_maxDepth(0.0f),
          m_bounds(),
          m_cellWidth(0.0f),
          m_cellHeight(0.0f),
          m_invCellWidth(0.0f),
          m_invCellHeight(0.0f),
          m_cellCountX(0),
          m_cellCountY(0),
          m_cells(nullptr),
          m_visibleBounds(),
          m_prebuiltVertexData(false),
          m_vertices(),
          m_text() {
        for (int i = 0; i < 7; ++i)
            _pad73[i] = 0;
    }

    // Golden: dtor body is `Reset()`; the compiler-emitted member
    // teardown then destroys m_text, frees m_vertices' buffer and
    // calls delete[] on m_cells (already nulled by Reset, hence the
    // guard in the golden's tail).
    ~SceneGridLayer();

    // Drops every cell bucket after clearing each entry's layer
    // pointer, frees the cell array, and zeroes the two cell counts.
    void Reset();

    // Rebuilds the grid for a new bounds/cell size. minDepth/maxDepth
    // are the layer slab's z range. The cell array is reallocated and
    // zeroed; the vertex list is rebuilt only when +72 is set.
    void Init(const Rectangle& bounds, float cellWidth, float cellHeight,
              float minDepth, float maxDepth);

    // Rebuilds m_vertices as a GL_LINES list: (m_cellCountY+1)
    // horizontal rules followed by (m_cellCountX+1) vertical rules, all
    // at z = (minDepth + maxDepth) / 2.
    void CreateVertexData();

    // Registers an entry whose AABB overlaps this layer, filling every
    // cell the AABB covers. No-op when the entry already has a layer.
    void AddObject(SceneGridEntry* entry, const Rectangle& bounds);

    // Packs the covered cell rectangle of an AABB into one u64:
    // [0:16) minX, [16:32) maxX, [32:48) minY, [48:64) maxY, each
    // clamped to [0, cellCount-1] and with max >= min. Degenerate
    // AABBs (< 0.0001 in either dimension) return 0xFFFF0000FFFF0000.
    unsigned long long GridRectForAABB(const Rectangle& bounds) const;

    // Moves an entry from its old cell rectangle to the new one:
    // unlink from the old cells outside the new rectangle, relink into
    // the new cells outside the old rectangle, then store the new
    // rectangle and bounds on the entry.
    void MoveObject(SceneGridEntry* entry, const Rectangle& bounds);

    // Unlinks the entry from every cell of its packed rectangle and
    // clears its layer pointer. No-op when the entry has no layer.
    void RemoveObject(SceneGridEntry* entry);

    // Appends every entry whose own bounds intersect the query to
    // `entries`, skipping the ones already collected (flag at +16) and
    // marking the rest as collected.
    void GetEntriesInBounds(const Rectangle& bounds,
                            FastVector<SceneGridEntry*>* entries);

    // Appends every entry in the layer to `entries`, skipping the ones
    // already collected and marking the rest. Iterates the buckets in
    // row-major order (y outer, x inner).
    void GetAllEntries(FastVector<SceneGridEntry*>* entries);

    // Cell (x, y) in world space: origin from the layer bounds, size
    // from the cell size.
    Rectangle BoundsForCellAt(int x, int y);

    // Debug draw: the grid line list, then the entry count of every
    // visible cell as text at the cell's lower-left corner, offset by
    // (+10, height-20) and pushed to the slab's mid-z.
    void Draw(RenderingContext* context, const Matrix4& matrix);

    float m_minDepth;                // +0
    float m_maxDepth;                // +4
    Rectangle m_bounds;              // +8
    float m_cellWidth;               // +24
    float m_cellHeight;              // +28
    float m_invCellWidth;            // +32
    float m_invCellHeight;           // +36
    int m_cellCountX;                // +40
    int m_cellCountY;                // +44
    SceneGridCell* m_cells;          // +48
    Rectangle m_visibleBounds;       // +56
    bool m_prebuiltVertexData;       // +72
    char _pad73[7];                  // +73
    std::vector<Vector3> m_vertices; // +80
    FontText m_text;                 // +104
};

static_assert(sizeof(SceneGridLayer) == 0x158, "sizeof must be 0x158");

} // namespace Caver

#endif
