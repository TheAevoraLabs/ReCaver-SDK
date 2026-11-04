// Recovered from libswordigo.so (1.4.13) evidence — IDA exports in
// reference/ida-arm64/functions/Caver/SceneGrid/ (11 own bodies,
// 0x4FD5C4..0x4FE1B4). The Scene keeps two of these at +728 and +792.
//
// A SceneGrid owns an ordered list of depth slabs and two scratch
// buffers used by the query entry points. Queries fill the scratch
// buffers and hand the caller a borrowed pointer to the second one,
// valid until the next query on the same grid.
//
// Layout (sizeof 0x40, proven by the Scene layout and every body):
//   +0   FastVector<boost::shared_ptr<SceneGridLayer>> m_layers
//   +16  Rectangle m_bounds
//   +32  FastVector<SceneGridEntry*> m_entries   (collection scratch)
//   +48  FastVector<void*> m_objects             (query output)
//
// There is no out-of-line constructor in the golden: both grids are
// zero-initialised in place (Scene's own constructor stores zeroes
// into the 64-byte span), which is exactly what the member default
// constructors below do.
#ifndef CAVER_SCENEGRID_H
#define CAVER_SCENEGRID_H

#include <boost/shared_ptr.hpp>

#include "../core/Rectangle.h"
#include "FastVector.h"
#include "SceneGridLayer.h"

namespace Caver {

class Camera;
class RenderingContext;
struct Matrix4;
struct SceneGridEntry;

class SceneGrid {
public:
    // Golden 0x4FD5C4: the compiler-emitted member teardown, in reverse
    // declaration order (m_objects buffer, m_entries buffer, then each
    // layer's shared_ptr release).
    ~SceneGrid();

    // Golden 0x4FD68C: a bare `ret`.
    void Reset();

    // Golden 0x4FD690: stores the bounds, then re-inits every layer for
    // them. Each layer's entries are collected first, unlinked by the
    // re-init, and added back with their own bounds.
    void SetBounds(const Rectangle& bounds);

    // Golden 0x4FD8EC: allocates a layer for the depth slab and appends
    // it to the list.
    void AddLayer(float minDepth, float maxDepth, float cellWidth,
                  float cellHeight);

    // Golden 0x4FDBA8: routes the entry to the layer that owns `depth`.
    void AddObject(SceneGridEntry* entry, const Rectangle& bounds, float depth);

    // Golden 0x4FDBE8: moves the entry within its current layer, or
    // unlinks it and adds it to the target layer when the depth crossed
    // a slab boundary.
    void MoveObject(SceneGridEntry* entry, const Rectangle& bounds, float depth);

    // Golden 0x4FDC7C: unlinks through the entry's own layer pointer.
    void RemoveObject(SceneGridEntry* entry);

    // Golden 0x4FDC84 / 0x4FDFA8: queries. Both return the object count
    // and borrow `*objects` from m_objects.
    unsigned int GetObjectsInBounds(const Rectangle& bounds, void*** objects);
    unsigned int GetObjectsNearCamera(float range, void*** objects);

    // Golden 0x4FDF08: stores each layer's camera AABB for its z range
    // in the layer's visible-bounds rectangle.
    void UpdateVisibleAreasWithCamera(Camera* camera);

    // Golden 0x4FE1B4: draws every layer in list order.
    void Draw(RenderingContext* context, const Matrix4& matrix);

private:
    // Golden 0x4FDB68: the first layer whose minDepth is below `depth`,
    // otherwise the last layer. The list is ordered as the scene adds
    // the slabs (nearest/shallowest first).
    SceneGridLayer* LayerForMinDepth(float depth);

    FastVector<boost::shared_ptr<SceneGridLayer>> m_layers; // +0
    Rectangle m_bounds;                                     // +16
    FastVector<SceneGridEntry*> m_entries;                  // +32
    FastVector<void*> m_objects;                            // +48
};

static_assert(sizeof(SceneGrid) == 0x40, "sizeof must be 0x40");

} // namespace Caver

#endif
