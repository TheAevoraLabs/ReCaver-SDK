// Recovered from libswordigo.so (1.4.13) evidence — IDA/Hex-Rays exports
// in ~/workspace/analysis/swordigo/functions/Caver/GUIScrollView/, the
// ABS64 relocations of the golden vtable, and golden disassembly of the
// ctor / Update / touch handlers.
//
// GUIScrollView: a GUIView that pans a content area larger than itself
// with two GUIScrollLocationController springs (one per axis), rubber-
// band drag overshoot (0.4 factor), delayed forwarding of touches to a
// hit content subview, and mouse-wheel scrolling.
//
// Derivation (binary-proven): RTTI @0x635218 is a __si_class_type_info
// whose single base is Caver::GUIView (@0x635228 -> _ZTIN5Caver7GUIViewE).
// GUIScrollView derives DIRECTLY from GUIView, not GUIControl.
//
// Vtable _ZTVN5Caver13GUIScrollViewE @0x6350D0, 328 bytes = 16-byte
// header + exactly 39 function slots (ABS64 relocations, in order):
// the 38 GUIView slots plus ONE added slot:
//   0  TouchBegan (override)          20-22 GUIView drag delegates
//   1  TouchMoved (override)          23 GUIView UndoManager
//   2  TouchEnded (override)          24 CanBecomeFirstResponder (override)
//   3  TouchCancelled (override)      25/26 GUIView Become/Resign
//   4  HandleMouseEvent (override)    27 ScrollViewCanCancelTouchInContentView (override)
//   5  GUIResponder HandleKeyboardEvent
//   6  GUIView nextResponder          28 GUIView PointInside
//   7/8 ~GUIScrollView D1/D0          29 HitTest (override)
//   9  Update (override)              30 GUIView Window
//   10 GUIView DrawRect               31 SetBounds (override)
//   11 GUIView LayoutSubviews         32 ConvertPointToSubview (override, const)
//   12-18 GUIView subview/popover     33 ConvertPointFromSubview (override, const)
//   19 GUIView DragAndDropItemWithTouch
//                                     34 ConvertRectToSubview (override, const)
//                                     35 ConvertRectFromSubview (override, const)
//                                     36 ConvertRectFromSubviewUsingFrame (override, const)
//                                     37 GUIView SetOrthoProjection
//                                     38 SetContentBounds (NEW in this class)
//
// Layout: GUIView (256 bytes), then (offsets byte-verified against the
// ctor @0x523718 stores and every method body; member NAMES are
// behaviour-derived and UNRECOVERED unless a consumer proves them):
//   +252 Rectangle contentBounds — the content area rect; it starts 4
//        bytes before the GUIView size because the golden
//        SetContentBounds stores a full 16-byte Rectangle at +252
//        (stur q0, [x0, #0xfc]); the GUIView colour dword at +249 is
//        overlapped by its first byte lane in this class's objects.
//   +268 GUIScrollLocationController horizontalController (44 bytes)
//   +312 GUIScrollLocationController verticalController (44 bytes)
//   +360 unsigned long long touchIdentifier (tracked touch, 0 = none)
//   +368 bool field368 (ctor true: gates the Update pass that derives
//        the controllers' scrolling flags from the scroll-enabled
//        tests — name UNRECOVERED)
//   +369 bool centersContent (ctor true: AdjustContentOffset centres
//        fitting content instead of aligning it to the bounds origin)
//   +370 bool touchActive (a tracked touch is down)
//   +371 bool dragging (the scroll drag is in progress)
//   +372 Vector2 dragDistance (drag offset delta accumulated since the
//        last velocity sample in Update)
//   +380 float dragTime (seconds accumulated over dragDistance)
//   +384 Vector2 dragStartOffset (controller locations when the drag
//        began)
//   +392 Vector2 dragStartTouchLocation (the touch's previous location
//        in this view when the drag began)
//   +400 Vector2 previousDragOffset (drag offset at the prior sample)
//   +408 Vector2 dragOffset (current drag offset)
//   +416 float previousDragInterval (ctor 0.0; the interval the prior
//        sample spanned — name UNRECOVERED)
//   +420 float dragInterval (ctor FLT_MAX; seconds since the latest
//        drag sample — name UNRECOVERED)
//   +424 bool field424 (ctor false; no reader in this class —
//        name UNRECOVERED)
//   +432 boost::shared_ptr<GUIView> contentTouchView (the subview hit
//        by the tracked touch, held while the touch is down)
//   +448 bool touchForwarded (TouchBegan was forwarded to the content
//        view)
//   +449 bool forwardingTouch (re-entrancy guard around forwarding)
//   +450 bool field450 (ctor false: when set, TouchBegan hit-tests and
//        remembers the content view and HitTest claims any hit for the
//        scroll view itself — name UNRECOVERED)
//   +451 bool contentCanCancel (result of the content view's
//        ScrollViewCanCancelTouchInContentView; NOT initialised by
//        the golden ctor — it is only written by TouchBegan)
//   +456 FWTouch storedTouch (raw storage: the in-tree FWTouch declares
//        a user copy constructor and no default constructor — see the
//        .cpp note; the golden ctor writes only its tapCount/phase)
//   +496 float touchDelay (seconds since TouchBegan; the deferred
//        TouchBegan forward fires once it is positive or the content
//        cannot cancel)
//   +500 Vector2 touchStartLocation
// sizeof = 512 (last member ends at +508; 8-byte alignment rounds up —
// derived from the layout, no allocation site was available to prove
// it independently).
#ifndef CAVER_GUISCROLLVIEW_H
#define CAVER_GUISCROLLVIEW_H

#include <boost/shared_ptr.hpp>

#include "../core/Vector3.h"  // Matrix4.h (via GUIView.h) names Vector3
#include "GUIView.h"
#include "GUIScrollLocationController.h"
#include "../core/FWTouch.h"

namespace Caver {

class GUIScrollView : public GUIView {
public:
    GUIScrollView();
    ~GUIScrollView() override;  // slots 7, 8

    void TouchBegan(const FWTouch& touch) override;       // slot 0
    void TouchMoved(const FWTouch& touch) override;       // slot 1
    void TouchEnded(const FWTouch& touch) override;       // slot 2
    void TouchCancelled(const FWTouch& touch) override;   // slot 3
    void HandleMouseEvent(const GUIMouseEvent& event) override;  // slot 4

    void Update(float deltaTime) override;                // slot 9
    bool CanBecomeFirstResponder() override;              // slot 24
    bool ScrollViewCanCancelTouchInContentView(GUIView* view) override;  // slot 27
    boost::shared_ptr<GUIView> HitTest(const Vector2& point) override;   // slot 29
    void SetBounds(const Rectangle& bounds) override;     // slot 31
    Vector2 ConvertPointToSubview(const Vector2& point,
                                  const GUIView* subview) const override;  // slot 32
    Vector2 ConvertPointFromSubview(const Vector2& point,
                                    const GUIView* subview) const override;  // slot 33
    Rectangle ConvertRectToSubview(const Rectangle& rect,
                                   const GUIView* subview) const override;  // slot 34
    Rectangle ConvertRectFromSubview(const Rectangle& rect,
                                     const GUIView* subview) const override;  // slot 35
    Rectangle ConvertRectFromSubviewUsingFrame(
        const Rectangle& rect, const GUIView* subview,
        const Rectangle& frame) const override;           // slot 36
    virtual void SetContentBounds(const Rectangle& bounds);  // slot 38 (new)

    // Non-virtual members.
    void ScretchContentViewToFill(GUIView* view);  // (sic) golden spelling
    bool HorizontalScrollEnabled() const;
    bool VerticalScrollEnabled() const;
    void AdjustContentOffset();
    Rectangle VisibleRect() const;
    void ScrollRectToVisible(const Rectangle& rect, bool animated);
    Vector2 AdjustedDragOffset(const Vector2& offset);
    void DragWithTouch(const FWTouch& touch);
    void EndDragWithTouch(const FWTouch& touch);

    Rectangle contentBounds;                             // +252
    GUIScrollLocationController horizontalController;    // +268
    GUIScrollLocationController verticalController;      // +312
    unsigned long long touchIdentifier;                  // +360
    bool field368;                                       // +368
    bool centersContent;                                 // +369
    bool touchActive;                                    // +370
    bool dragging;                                       // +371
    Vector2 dragDistance;                                // +372
    float dragTime;                                      // +380
    Vector2 dragStartOffset;                             // +384
    Vector2 dragStartTouchLocation;                      // +392
    Vector2 previousDragOffset;                          // +400
    Vector2 dragOffset;                                  // +408
    float previousDragInterval;                          // +416
    float dragInterval;                                  // +420
    bool field424;                                       // +424
    char _pad428[4];                                     // +428
    boost::shared_ptr<GUIView> contentTouchView;         // +432
    bool touchForwarded;                                 // +448
    bool forwardingTouch;                                // +449
    bool field450;                                       // +450
    bool contentCanCancel;                               // +451
    char _pad452[4];                                     // +452
    alignas(8) unsigned char storedTouch[40];            // +456 FWTouch storage
    float touchDelay;                                    // +496
    Vector2 touchStartLocation;                          // +500
};

} // namespace Caver

#endif
