// Recovered from libswordigo.so (1.4.13) evidence.
// GUIWindow: sizeof = 0x200 (512), verified from operator new(0x200) in
// CaverShell::InitView / GUIWindow::PresentModalView and the ctor stores.
//
// Bases (RTTI _ZTIN5Caver9GUIWindowE @0x6358D8, __vmi_class_type_info,
// 3 bases): GUIView @+0 (primary), IFWMouseListener @+256,
// IFWKeyboardListener @+264 (offset_flags 0x10002 / 0x10802).
//
// Vtable _ZTVN5Caver9GUIWindowE @0x6356E8, 464 bytes, recovered from
// ABS64 relocations (see the GUI slice report):
//   Primary (address point +16): 45 slots. Slots 0-10 are the GUIView
//   slots (0 TouchBegan, 1 TouchMoved, 2 TouchEnded, 3 TouchCancelled,
//   4 HandleMouseEvent, 5 HandleKeyboardEvent [NOT overridden: points to
//   GUIResponder::HandleKeyboardEvent], 6 nextResponder [GUIView],
//   7 ~D1, 8 ~D0, 9 Update, 10 DrawRect), then GUIWindow's own order:
//   11 LayoutSubviews, 12 AddSubview, 13 InsertSubviewBelowSubview,
//   14 InsertSubviewAtIndex, 15 RemoveFromSuperview, 16 DidMoveToWindow,
//   17 ContainingPopoverView, 18 DismissPopoverView() [GUIView impl],
//   19 DragAndDropItemWithTouch, 20 DraggableItemDidEnterView,
//   21 DraggableItemDidLeaveView, 22 DraggableItemDidDropIntoView,
//   23 UndoManager, 24 CanBecomeFirstResponder, 25 BecomeFirstResponder,
//   26 ResignFirstResponder, 27 ScrollViewCanCancelTouchInContentView,
//   28 PointInside, 29 HitTest, 30 Window, 31 SetBounds,
//   32 ConvertPointToSubview, 33 ConvertPointFromSubview,
//   34 ConvertRectToSubview, 35 ConvertRectFromSubview,
//   36 ConvertRectFromSubviewUsingFrame, 37 SetOrthoProjection,
//   38 MouseDown, 39 MouseMoved, 40 MouseUp, 41 MouseWheelRotated,
//   42 KeyDown, 43 KeyUp, 44 KeyChar.
//   Secondary @+392 (offset-to-top -256, IFWMouseListener): MouseDown,
//   MouseMoved, MouseUp, MouseWheelRotated (all _ZThn256_ thunks).
//   Secondary @+440 (offset-to-top -264, IFWKeyboardListener): KeyDown,
//   KeyUp, KeyChar (all _ZThn264_ thunks).
// NOTE: the golden secondary vtables contain NO destructor slots, so
// the real IFWMouseListener/IFWKeyboardListener interfaces declare
// exactly the methods above and no virtual destructor. The stubs in
// core/EngineSystems.h (virtual ~ only) do NOT match the binary and
// must be corrected by the core slice; this header keeps inheriting
// them so the tree still compiles.
//
// Member layout (byte-verified from the ctor @0x529A3C stores, the
// destructor @0x529AE0 release order, and every method's accesses):
//   +272 shared_ptr<GUIViewController> m_rootViewController
//   +288 shared_ptr<GUIView>           m_firstResponder
//   +304 std::set<unsigned int>        m_downKeys
//   +328 std::map<ulong, FWTouch>      m_activeTouches
//   +352 std::map<ulong, shared_ptr<GUIView>> m_touchViews
//   +376 std::list<GUIPopoverView*>    m_popoverViews
//   +400 std::list<shared_ptr<GUIWindow>> m_modalWindows
//   +424 GUIWindow*                    m_parentWindow (raw; set by
//                                        PresentModalView on the child)
//   +432 float m_modalFade        (current fade, starts 0)
//   +436 float m_modalFadeTarget  (starts 0.8f: ctor qword
//                                  0x3F4CCCCD00000000 at +432)
//   +440 GUIUndoManager* m_undoManager (raw; the ctor NEVER stores
//                                  +440 — UNRECOVERED initialisation,
//                                  presumably set externally)
//   +448 shared_ptr<GUIEffect> m_effect
//   +464 bool  m_dismissed        (set when the dismiss timer expires)
//   +468 float m_dismissTimer     (Dismiss() sets 0.3f = 1050253722)
//   +472 float rotation           (read by CaverShell::Update/Render)
//   +480 intrusive_ptr<Texture> m_framebufferTexture (manual counted
//                                  pointer in the binary: pointee count
//                                  at +8, release calls vtable +8)
//   +488 shared_ptr<GUIWindowFramebuffer> m_framebuffer
//                                  (concrete type UNNAMED — see the
//                                  GUIWindowFramebuffer note below;
//                                  RenderingContext::
//                                  FramebufferForDrawingToTexture
//                                  returns it BY VALUE as a shared_ptr
//                                  (16-byte sret, proven — NOT the
//                                  intrusive_ptr<Texture> the render
//                                  header currently declares); called
//                                  through its vtable +16/+24/+32 in
//                                  DrawRect)
//   +504 bool m_framebufferDirty  (ctor stores 1)
#ifndef CAVER_GUIWINDOW_H
#define CAVER_GUIWINDOW_H

#include "GUIView.h"
#include "../../platform/FWKeyboard.h"
#include "../../platform/FWMouse.h"
#include "../core/FWTouch.h"
#include "../core/Rectangle.h"
#include "../core/Vector2.h"
#include <boost/intrusive_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <list>
#include <map>
#include <set>
#include <string>

namespace Caver {

class GUIViewController;
class GUIEffect;
class Texture;
// Manual counted-pointer hooks for the +480 framebuffer texture member
// (the pointee carries its count at +8; definitions live in the render
// slice — the golden GUIWindow code performs the same add/release
// protocol inline).
void intrusive_ptr_add_ref(Texture* texture);
void intrusive_ptr_release(Texture* texture);
class GUIPopoverView;
class GUIDraggableItem;
class GUIUndoManager;
class RenderingContext;
struct Matrix4;

// Concrete type behind the shared_ptr at +488: STILL UNNAMED — no
// RTTI, no sp_counted_impl_p instantiation and no type-name string for
// it exists anywhere in the golden binary, and its factory
// (NewFramebufferImplForDrawingToTexture) returns a null shared_ptr in
// this build, so the concrete class is not linked into this .so.
// What IS proven (GUIWindow::DrawRect @0x52A544 disassembly + Ghidra):
// it is held by boost::shared_ptr (16-byte sret return from
// RenderingContext::FramebufferForDrawingToTexture), and its vtable
// carries a destructor pair at +0/+8 plus exactly the three methods
// DrawRect calls: +16 (two int args, 1 and 0; return ignored),
// +24 (RenderingContext*) and +32 (RenderingContext*). The method
// NAMES are UNRECOVERED; the slot-shaped declarations below exist so
// DrawRect's call sites emit the golden vtable offsets.
class GUIWindowFramebuffer {
public:
    virtual ~GUIWindowFramebuffer();
    virtual void Slot16(int, int);
    virtual void Slot24(RenderingContext*);
    virtual void Slot32(RenderingContext*);
};

class GUIWindow : public GUIView, public IFWMouseListener, public IFWKeyboardListener {
public:
    GUIWindow();
    virtual ~GUIWindow();

    static GUIWindow* mainWindow();
    static void SetMainWindow(const boost::shared_ptr<GUIWindow>& window);
    static boost::shared_ptr<GUIWindow> s_mainWindow;  // .bss @0x6518A8

    void SetTitle(const std::string& title);
    void SetRootViewController(const boost::shared_ptr<GUIViewController>& controller);
    void SetFirstResponder(const boost::shared_ptr<GUIView>& view);

    // GUIView overrides (primary vtable slots in parentheses).
    virtual void TouchBegan(const FWTouch& touch);       // 0 (empty body)
    virtual void TouchMoved(const FWTouch& touch);       // 1 (empty body)
    virtual void TouchEnded(const FWTouch& touch);       // 2 (empty body)
    virtual void TouchCancelled(const FWTouch& touch);   // 3 (empty body)
    virtual void HandleMouseEvent(const GUIMouseEvent& event);  // 4 (empty)
    virtual void Update(float deltaTime);                       // 9
    virtual void DrawRect(RenderingContext* context, const Rectangle& rect,
                          const Matrix4& transform);            // 10
    virtual void LayoutSubviews();                              // 11
    virtual void DragAndDropItemWithTouch(
        const FWTouch& touch,
        const boost::shared_ptr<GUIDraggableItem>& item);              // 19
    virtual GUIUndoManager* UndoManager();                             // 23
    virtual GUIWindow* Window();                                       // 30

    // IFWMouseListener / IFWKeyboardListener implementations (primary
    // slots 38-44; _ZThn256_/_ZThn264_ thunks in the secondary vtables).
    virtual void MouseDown(const Vector2& position, unsigned int buttons,
                           double timestamp);                          // 38 (empty)
    virtual void MouseMoved(const Vector2& previousPosition, const Vector2& position,
                            unsigned int buttons, double timestamp);   // 39
    virtual void MouseUp(const Vector2& position, unsigned int buttons,
                         double timestamp);                            // 40 (empty)
    virtual void MouseWheelRotated(float delta, const Vector2& position,
                                   unsigned int buttons, double timestamp); // 41
    virtual void KeyDown(unsigned int keyCode, unsigned int modifiers,
                         double timestamp);                            // 42
    virtual void KeyUp(unsigned int keyCode, unsigned int modifiers,
                       double timestamp);                              // 43
    virtual void KeyChar(unsigned int character, double timestamp);    // 44

    // Non-virtual window operations.
    GUIEffect* Effect();
    void UpdateModalFade(bool visible, float deltaTime);
    void PresentModalView(const boost::shared_ptr<GUIView>& view, bool center);
    void DismissModalView(GUIView* view);
    void DismissAllModalViews();
    void Dismiss();
    void PresentPopoverViewFromRectangle(const boost::shared_ptr<GUIView>& view,
                                         const Rectangle& rect,
                                         unsigned int permittedArrowDirections);
    void DismissPopoverView(GUIPopoverView* popover);
    void DismissAllPopoverViews();
    void CancelAllTouches();
    void CancelDownKeys();

    void BeginTouch(const FWTouch& touch);
    void MoveTouch(const FWTouch& touch);
    void EndTouch(const FWTouch& touch);
    void CancelTouch(const FWTouch& touch);

    boost::shared_ptr<GUIViewController> m_rootViewController;   // +272
    boost::shared_ptr<GUIView> m_firstResponder;                   // +288
    std::set<unsigned int> m_downKeys;                             // +304
    std::map<unsigned long, FWTouch> m_activeTouches;              // +328
    std::map<unsigned long, boost::shared_ptr<GUIView> > m_touchViews; // +352
    std::list<GUIPopoverView*> m_popoverViews;                     // +376
    std::list<boost::shared_ptr<GUIWindow> > m_modalWindows;       // +400
    GUIWindow* m_parentWindow;                                     // +424
    float m_modalFade;                                             // +432
    float m_modalFadeTarget;                                       // +436
    GUIUndoManager* m_undoManager;                                 // +440
    boost::shared_ptr<GUIEffect> m_effect;                         // +448
    bool m_dismissed;                                              // +464
    float m_dismissTimer;                                          // +468
    float rotation;                                                // +472
    char _pad0[4];                                                 // +476
    boost::intrusive_ptr<Texture> m_framebufferTexture;            // +480
    boost::shared_ptr<GUIWindowFramebuffer> m_framebuffer;         // +488
    bool m_framebufferDirty;                                       // +504
    char _pad2[7];                                                 // +505 -> 512
};

} // namespace Caver

#endif
