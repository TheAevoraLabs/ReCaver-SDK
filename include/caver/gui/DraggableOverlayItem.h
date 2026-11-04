// Recovered from libswordigo.so (1.4.13) evidence — IDA/Hex-Rays exports
// in ~/workspace/analysis/swordigo/functions/Caver/DraggableOverlayItem/,
// the golden vtable _ZTVN5Caver20DraggableOverlayItemE @0x62E7D0 (38 slots,
// RTTI _ZTIN5Caver20DraggableOverlayItemE @0x62E900, single base GUIView).
//
// DraggableOverlayItem: the draggable proxy for one GameOverlayView
// control on the "configure controls" overlay (ConfigureOverlayView).
// The user drags it into place; on release (TouchMoved) it notifies its
// delegate (ConfigureOverlayView) through IDraggableOverlayItemDelegate.
//
// Base: GUIView (256 bytes, primary vtable, no secondary bases).
// Overrides (vtable slots, proven by the golden vtable relocations):
//   slot 0  TouchBegan, slot 1 TouchMoved, slot 2 TouchEnded,
//   slot 3  TouchCancelled, slots 7/8 ~DraggableOverlayItem (D1/D0),
//   slot 10 DrawRect, slot 11 LayoutSubviews (a 4-byte tail-branch
//   thunk to GUIView::LayoutSubviews @0x4853E4).
// All other slots keep the GUIView functions.
//
// Layout (sizeof 656 = 0x290, proven by operator new(0x290) in
// ConfigureOverlayView::InitWithGameState and by every member access):
//   +256 IDraggableOverlayItemDelegate* m_delegate (RAW; TouchMoved
//        calls its slot 0 with this item when the touch matches)
//   +264 Rectangle m_cachedRect (member name UNRECOVERED; the rect the
//        rounded rects were last built for — DrawRect rebuilds them
//        only when the drawn rect differs from this by > 0.0001 in
//        any component; zeroed by the ctor)
//   +280 GUIRoundedRect m_roundedRect1 (168 bytes; drawn normally,
//        textured "ui_controls_button_pressed")
//   +448 GUIRoundedRect m_roundedRect2 (168 bytes; same texture,
//        drawn while the item is touched)
//   +616 std::string m_identifier (the GUIViewLayout subview id, set
//        by ConfigureOverlayView::InitWithGameState)
//   +640 unsigned long long m_touchIdentifier (FWTouch id captured
//        by TouchBegan; cleared by TouchEnded/TouchCancelled)
//   +648 Vector2 m_touchOffset (touch location in the item at
//        TouchBegan; TouchMoved drags the frame by the delta from it;
//        NOT initialised by the ctor — the golden ctor zeroes
//        +616..+647 only)
//
// MERGE NOTE: ConfigureOverlayView.h currently carries its own copies
// of IDraggableOverlayItemDelegate and a forward DraggableOverlayItem
// declaration (plus a partial DraggableOverlayItem class sketch);
// those must be removed at merge in favour of this header — the two
// definitions cannot co-exist in one TU.
#ifndef CAVER_DRAGGABLEOVERLAYITEM_H
#define CAVER_DRAGGABLEOVERLAYITEM_H

#include <string>

#include "../core/Color.h"
#include "../core/FWTouch.h"
#include "../core/Rectangle.h"
#include "../core/Vector2.h"
#include "GUIView.h"
#include "GUIRoundedRect.h"

namespace Caver {

class RenderingContext;
class DraggableOverlayItem;

// The draggable-item delegate interface (no vtable slot for a
// destructor — the golden IDraggableOverlayItemDelegate _ZTI @0x62E450
// is a plain __class_type_info; ConfigureOverlayView implements it
// through its +256 secondary vtable).
class IDraggableOverlayItemDelegate {
public:
    virtual void DraggableOverlayItemDidMove(DraggableOverlayItem* item) = 0;  // slot 0
};

class DraggableOverlayItem : public GUIView {
public:
    DraggableOverlayItem();
    ~DraggableOverlayItem() override;  // slots 7, 8

    // GUIResponder slots 0-3. Declared void here: the golden bodies
    // never materialise a return value (TouchEnded/TouchCancelled
    // are just `str xzr, [x0, #0x280]; ret`) yet have full epilogues
    // and stack protectors — a shape the NDK r27d clang (the golden's
    // own compiler) cannot produce from a bool function that falls
    // off the end (it deletes the epilogue and the trailing stores),
    // and a bool function with an explicit return would show a
    // `mov w0` that the golden lacks. The in-tree GUIResponder.h
    // currently declares these four as bool; that header must be
    // corrected to void (hand-off, see the report) for this header
    // to compile against the tree — the scratch parity build proves
    // the void form grades BYTE-OK with a patched base header.
    void TouchBegan(const FWTouch& touch) override;      // slot 0
    void TouchMoved(const FWTouch& touch) override;      // slot 1
    void TouchEnded(const FWTouch& touch) override;      // slot 2
    void TouchCancelled(const FWTouch& touch) override;  // slot 3

    void DrawRect(RenderingContext* context, const Rectangle& rect,
                  const Matrix4& transform) override;  // slot 10
    void LayoutSubviews() override;                    // slot 11

    // Non-virtual members.
    void SetColor(Color color);

    IDraggableOverlayItemDelegate* m_delegate;  // +256
    Rectangle m_cachedRect;                     // +264 UNRECOVERED name
    GUIRoundedRect m_roundedRect1;              // +280
    GUIRoundedRect m_roundedRect2;              // +448
    std::string m_identifier;                   // +616
    unsigned long long m_touchIdentifier;       // +640
    Vector2 m_touchOffset;                      // +648
};

// Golden aarch64 NDK-libc++ layout anchor; MSVC STL member sizes differ,
// so this assert is Android/Linux-only (the Windows build is self-consistent
// under MSVC STL and does not claim golden byte-layout parity).
#ifndef _MSC_VER
static_assert(sizeof(DraggableOverlayItem) == 656,
              "DraggableOverlayItem must be 656 bytes (0x290)");
#endif

} // namespace Caver

#endif
