// Recovered from libswordigo.so (1.4.13) evidence — IDA/Hex-Rays exports
// in ~/workspace/analysis/swordigo/functions/Caver/GUIView/, the ABS64
// relocations of the golden vtable, and the ctor/dtor bodies.
//
// GUIView: the base UI node (frame/bounds, subview tree, layout,
// hit-testing, animation driving). sizeof = 256 (0x100): GUILoadingView
// (0x108) adds its duration/delay floats at +256/+260, and derived
// classes (GUIFrameView, GUIButton) write their first own field at +256.
//
// Derivation (binary-proven): GUIView's RTTI @0x635630 is a
// __si_class_type_info whose single base is Caver::GUIResponder
// (@0x635640 -> _ZTI GUIResponder). The golden GUIView vtable reuses
// GUIResponder's functions for slots 0-5, so GUIView IS-A GUIResponder
// (the earlier stub header, which did not derive, was wrong).
//
// Vtable _ZTVN5Caver7GUIViewE @0x6354F0, 320 bytes = 16-byte header +
// exactly 38 function slots (ABS64 relocations, in order):
//   0  GUIResponder::TouchBegan            20 DraggableItemDidEnterView
//   1  GUIResponder::TouchMoved            21 DraggableItemDidLeaveView
//   2  GUIResponder::TouchEnded            22 DraggableItemDidDropIntoView
//   3  GUIResponder::TouchCancelled        23 UndoManager
//   4  GUIResponder::HandleMouseEvent      24 CanBecomeFirstResponder
//   5  GUIResponder::HandleKeyboardEvent   25 BecomeFirstResponder
//   6  GUIView::nextResponder (override)   26 ResignFirstResponder
//   7  ~GUIView (D1)                       27 ScrollViewCanCancelTouchInContentView
//   8  ~GUIView (D0)                       28 PointInside (const)
//   9  Update(float)                       29 HitTest
//   10 DrawRect                            30 Window
//   11 LayoutSubviews                      31 SetBounds
//   12 AddSubview                          32 ConvertPointToSubview (const)
//   13 InsertSubviewBelowSubview           33 ConvertPointFromSubview (const)
//   14 InsertSubviewAtIndex                34 ConvertRectToSubview (const)
//   15 RemoveFromSuperview                 35 ConvertRectFromSubview (const)
//   16 DidMoveToWindow                     36 ConvertRectFromSubviewUsingFrame (const)
//   17 ContainingPopoverView               37 SetOrthoProjection
//   18 DismissPopoverView
//   19 DragAndDropItemWithTouch
// Slots 9/10 are Update/DrawRect — the two slots CaverShell::Update/
// Render call through the vtable (confirms the stub header's guess).
//
// Layout (byte-verified from the ctor @0x526120 stores, the dtor
// @0x526184 teardown order, and every method's accesses):
//   +0   vptr
//   +8   GUIResponder base: enable_shared_from_this weak_ptr (+8/+16)
//   +24  GUIResponder::m_nextResponder
//   +32  std::list<shared_ptr<GUIView>> subviews (24 bytes)
//   +56  Rectangle clip rect for safeBounds() (used only while its
//        width at +64 > 0) — original member name UNRECOVERED
//   +72  Rectangle clip rect for controlBounds() (gate: width at +80)
//        — stub name kept (GUIWindow.cpp assigns it); original name
//        UNRECOVERED
//   +88  bool removed flag: set by RemoveFromSuperview, cleared when
//        the view is re-added or purged (name UNRECOVERED)
//   +96  GUIView* superview (raw, non-owning)
//   +104 std::list<shared_ptr<GUIAnimation>> animations (24 bytes)
//   +128 int layout flags: LayoutSubviews tests bits 0x2 and 0x10
//        (autoresizing-style anchors; name UNRECOVERED)
//   +132 Rectangle frame
//   +148 Rectangle bounds — the OPERATIVE rect of the view: golden
//        bodies across the view classes consistently read/write its
//        origin (+148) and size (+156/+160) as the rect that layout,
//        conversion and drawing maths operate on (midviews slice,
//        binary-verified across 8 view classes)
//   +164 Rectangle touchBounds (hit-test rect; used only while its
//        width at +172 > 0.001, else bounds is used)
//   +180 Matrix4 ortho projection, rebuilt by SetBounds (name
//        UNRECOVERED)
//   +244 bool hidden (Update/DrawRect skip the view, HitTest bails)
//   +245 bool userInteractionEnabled (ctor sets it: the ctor's dword
//        store of 0x100 at +244; HitTest requires it; GUIWindow::
//        Dismiss clears it)
//   +246 bool (HitTest: when set, a hit view returns an empty
//        shared_ptr instead of itself — name UNRECOVERED)
//   +247 bool (touch/key guard read by the window slice — name
//        UNRECOVERED)
//   +248 bool clipsSubviews (DrawRect wraps the child in a GL scissor
//        test, GL_SCISSOR_TEST=3089)
//   +249 colour dword, UNALIGNED (midviews slice, binary-verified):
//        GUILoadingView's ctor writes 0xFF000000 there and its
//        DrawRect reads it; GUIFrameView's ctor writes -1 and its
//        SetFrameColor/SetFrameTexture use it. Byte +252 is the
//        dword's top byte (the colour's alpha for the values above).
//        GUIView's own ctor never writes +249..+255.
#ifndef CAVER_GUIVIEW_H
#define CAVER_GUIVIEW_H

#include <boost/shared_ptr.hpp>
#include <list>

#include "GUIResponder.h"
#include "../core/Matrix4.h"
#include "../core/Rectangle.h"
#include "../core/Vector2.h"

namespace Caver {

class GUIAnimation;
class GUIDraggableItem;
class GUIPopoverView;
class GUIUndoManager;
class GUIWindow;
class RenderingContext;

class GUIView : public GUIResponder {
public:
    GUIView();
    virtual ~GUIView();  // slots 7, 8

    // Slot 6 override: m_nextResponder when set, else the superview.
    virtual GUIView* nextResponder() const;

    virtual void Update(float deltaTime);  // slot 9
    virtual void DrawRect(RenderingContext* context, const Rectangle& rect,
                          const Matrix4& transform);  // slot 10
    virtual void LayoutSubviews();                    // slot 11
    virtual void AddSubview(
        const boost::shared_ptr<GUIView>& subview);  // slot 12
    virtual void InsertSubviewBelowSubview(
        const boost::shared_ptr<GUIView>& subview,
        GUIView* belowSubview);  // slot 13
    virtual void InsertSubviewAtIndex(
        int index, const boost::shared_ptr<GUIView>& subview);  // slot 14
    virtual void RemoveFromSuperview();                          // slot 15
    virtual void DidMoveToWindow();                              // slot 16
    virtual GUIPopoverView* ContainingPopoverView();             // slot 17
    virtual void DismissPopoverView();                           // slot 18
    virtual void DragAndDropItemWithTouch(
        const FWTouch& touch,
        const boost::shared_ptr<GUIDraggableItem>& item);        // slot 19
    virtual bool DraggableItemDidEnterView(GUIDraggableItem* item);   // 20
    virtual bool DraggableItemDidLeaveView(GUIDraggableItem* item);   // 21
    virtual bool DraggableItemDidDropIntoView(GUIDraggableItem* item);  // 22
    virtual GUIUndoManager* UndoManager();                            // 23
    virtual bool CanBecomeFirstResponder();                           // 24
    virtual bool BecomeFirstResponder();                              // 25
    virtual bool ResignFirstResponder();                              // 26
    virtual bool ScrollViewCanCancelTouchInContentView(GUIView* view);  // 27
    virtual bool PointInside(const Vector2& point) const;             // 28
    virtual boost::shared_ptr<GUIView> HitTest(const Vector2& point);  // 29
    virtual GUIWindow* Window();                                       // 30
    virtual void SetBounds(const Rectangle& bounds);                  // 31
    virtual Vector2 ConvertPointToSubview(const Vector2& point,
                                          const GUIView* subview) const;  // 32
    virtual Vector2 ConvertPointFromSubview(const Vector2& point,
                                            const GUIView* subview) const;  // 33
    virtual Rectangle ConvertRectToSubview(const Rectangle& rect,
                                           const GUIView* subview) const;  // 34
    virtual Rectangle ConvertRectFromSubview(const Rectangle& rect,
                                             const GUIView* subview) const;  // 35
    virtual Rectangle ConvertRectFromSubviewUsingFrame(
        const Rectangle& rect, const GUIView* subview,
        const Rectangle& frame) const;  // slot 36
    virtual void SetOrthoProjection(RenderingContext* context,
                                    const Matrix4& transform);  // slot 37

    // Non-virtual members.
    void SetFrame(const Rectangle& frame);
    void InsertSubview(std::list<boost::shared_ptr<GUIView> >::iterator pos,
                       const boost::shared_ptr<GUIView>& subview);
    void RemoveAllSubviews();
    void PurgeRemovedSubviews();
    bool IsSubviewRemoved(const boost::shared_ptr<GUIView>& subview);
    void BringSubviewToFront(GUIView* subview);
    bool IsDescendantOf(GUIView* view);
    bool FindAndSetSubviewFirstResponder();
    bool FindAndSetSuperviewFirstResponder();
    bool IsFirstResponder();
    void DrawSubviewRect(GUIView* subview, RenderingContext* context,
                         const Rectangle& rect, const Matrix4& transform);
    void AddAnimation(const boost::shared_ptr<GUIAnimation>& animation);
    void RemoveAllAnimations();
    void AddPopupAnimation();
    void PresentModalView(const boost::shared_ptr<GUIView>& view, bool center);
    void DismissModalView(GUIView* view);
    void PresentPopoverViewFromRectangle(
        const boost::shared_ptr<GUIView>& view, const Rectangle& rect,
        unsigned int permittedArrowDirections);
    boost::shared_ptr<GUIView> SharedViewPtr();
    Rectangle safeBounds() const;
    Rectangle controlBounds() const;
    void SetTouchBoundsToIncludeSubviewTouchBounds();
    Vector2 ConvertPointToView(const Vector2& point, GUIView* view) const;
    Vector2 ConvertPointFromView(const Vector2& point, GUIView* view) const;
    Rectangle ConvertRectToView(const Rectangle& rect, GUIView* view) const;
    Rectangle ConvertRectFromView(const Rectangle& rect, GUIView* view) const;

    std::list<boost::shared_ptr<GUIView> > subviews;  // +32
    Rectangle field56;       // +56  safeBounds() clip rect (name UNRECOVERED)
    Rectangle contentFrame;  // +72  controlBounds() clip rect (see header note)
    bool field88;            // +88  removed flag (name UNRECOVERED)
    char _pad89[7];          // +89
    GUIView* superview;      // +96
    std::list<boost::shared_ptr<GUIAnimation> > animations;  // +104
    int field128;            // +128 layout flags (name UNRECOVERED;
                             // derived ctors also force the value 18
                             // here when it differs — purpose still
                             // UNRECOVERED, midviews slice)
    Rectangle frame;         // +132
    Rectangle bounds;        // +148 the operative rect (see header note)
    Rectangle touchBounds;   // +164
    Matrix4 field180;        // +180 ortho projection (name UNRECOVERED)
    bool hidden;                       // +244
    bool userInteractionEnabled;       // +245
    bool field246;                     // +246 (name UNRECOVERED)
    bool field247;                     // +247 (name UNRECOVERED)
    bool clipsSubviews;                // +248
    unsigned char field249[4];         // +249 unaligned colour dword
                                       // (derived-class state, see note)
    char _pad253[3];                   // +253 -> 256
};

} // namespace Caver

#endif
