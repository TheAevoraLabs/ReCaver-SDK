// Recovered from libswordigo.so (1.4.13) evidence — IDA/Hex-Rays exports
// in ~/workspace/analysis/swordigo/functions/Caver/PauseViewController/,
// the golden vtable _ZTVN5Caver19PauseViewControllerE @0x62B968
// (address point 0x62B978, recovered from ABS64 relocations) and the
// __vmi RTTI @0x62BA00 (56 bytes, 2 bases; the +72 base's RTTI name
// string @0x26AA98 proves it is IPauseViewDelegate).
//
// PauseViewController is the pause modal GameViewController::
// SuspendView presents when the game is running (that call site's
// operator new(0x60) fixes sizeof = 96; it presents this controller
// as a modal and fades the window via GUIWindow::UpdateModalFade).
//
// Bases (RTTI order):
//   +0   GUIViewController (public)
//   +72  IPauseViewDelegate (secondary @0x62B9E8; its RTTI @0x62B9F0
//        is a plain __class_type_info — a bare vptr, no data, no
//        destructor slot)
// Primary vtable (address point 0x62B978, 12 slots):
//   0 ~PauseViewController (D1/D2 @0x442FB0)  1 ~ (D0 @0x44303C)
//   2 GUIViewController::NewLoadingView (not overridden)
//   3 LoadView (@0x442D48)
//   4 SuspendView (@0x442F94, empty)
//   5 GUIViewController::ResumeView (not overridden)
//   6 UnloadView (@0x442F98, empty)
//   7 GUIViewController::ViewDidAppear (not overridden)
//   8 GUIViewController::ViewWillDisappear (not overridden)
//   9 Update(float) (@0x442F9C, 4-byte tail call to the base)
//   10 GUIViewController::PresentModalViewController (not overridden)
//   11 PauseViewDidContinue (@0x442FA0)
// Secondary vtable (-72): exactly one slot, the _ZThn72_ thunk to
// PauseViewDidContinue (@0x442FA8).
//
// Layout (ctor @0x442D0C zero-store map; destructor @0x442FB0
// member-release: the +80 shared_ptr's block at +88 is released
// before the base destructor runs):
//   +80  shared_ptr<PauseView> — the view built by LoadView
//        (operator new(0x118) = the 280-byte PauseView); also adopted
//        as the controller's m_view. Name UNRECOVERED.
// There are no other members: the ctor zero-stores +80/+88 only, and
// GameViewController::SuspendView's operator new(0x60) caps the size
// at 96.
//
// NOTE: no user-declared destructor — the golden D0 @0x44303C is a
// full clone of the D2 body plus the operator delete tail, the shape
// clang emits for an implicit destructor (the GameOverViewController
// / PurchaseViewController pattern), NOT the 36-byte call-D1 thunk a
// user-declared destructor produces here.
#ifndef CAVER_PAUSEVIEWCONTROLLER_H
#define CAVER_PAUSEVIEWCONTROLLER_H

#include <boost/shared_ptr.hpp>

#include "GUIViewController.h"

namespace Caver {

class PauseView;

class PauseViewController;

// Delegate interface implemented at +72 (bare vptr, no destructor —
// see header note). PauseView stores a pointer to it at its +256
// (LoadView writes `this + 72` there).
class IPauseViewDelegate {
public:
    virtual void PauseViewDidContinue(PauseView* view) = 0; // slot 0
};

class PauseViewController : public GUIViewController,
                              public IPauseViewDelegate {
public:
    PauseViewController();
    // No user-declared destructor: the golden D2 @0x442FB0 / D0
    // @0x44303C pair is the implicit-destructor shape (see above).

    // GUIViewController overrides (primary slots 3, 4, 6, 9).
    virtual void LoadView();
    virtual void SuspendView();
    virtual void UnloadView();
    virtual void Update(float deltaTime);

    // IPauseViewDelegate (primary slot 11).
    virtual void PauseViewDidContinue(PauseView* view);

    boost::shared_ptr<PauseView> m_pauseView; // +80  name UNRECOVERED
};

} // namespace Caver

#endif
