// Recovered from libswordigo.so (1.4.13) evidence.
//
// GUIDraggableItem: a GUIView that follows a touch and reports drop
// targets. Vtable 320 bytes @0x6349F0 — same 38 slots as GUIView, with
// these overrides (from the ABS64 relocation dump):
//   slot 1  TouchMoved, slot 2 TouchEnded, slot 3 TouchCancelled,
//   slot 7/8 ~GUIDraggableItem (D2 @0x4AA8C4, D0 @0x51FD40),
//   slot 29 HitTest (returns an empty shared_ptr in this class)
// All other slots are GUIView's own functions.
//
// Layout: GUIView (256 bytes), then
//   +256 boost::shared_ptr<GUIView> dropTargetView — the view currently
//        hovered during a drag (released first in the golden D2).
// No out-of-line constructor/destructor was written in the original:
// both are implicit (the D2 body is exactly the shared_ptr teardown plus
// ~GUIView).
#ifndef CAVER_GUIDRAGGABLEITEM_H
#define CAVER_GUIDRAGGABLEITEM_H

#include <boost/shared_ptr.hpp>

#include "../core/Vector2.h"
#include "GUIView.h"

namespace Caver {

class GUIDraggableItem : public GUIView {
public:
    void TouchMoved(const FWTouch& touch) override;      // slot 1
    void TouchEnded(const FWTouch& touch) override;      // slot 2
    void TouchCancelled(const FWTouch& touch) override;  // slot 3

    boost::shared_ptr<GUIView> HitTest(const Vector2& point) override;

    void SetLocationInSuperview(const Vector2& location);
    Vector2 LocationInView(GUIView* view);

    boost::shared_ptr<GUIView> dropTargetView;  // +256
};

} // namespace Caver

#endif
