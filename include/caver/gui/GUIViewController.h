// Recovered from libswordigo.so (1.4.13) evidence — IDA/Hex-Rays exports
// in ~/workspace/analysis/swordigo/functions/Caver/GUIViewController/
// plus the golden vtable recovered from ABS64 relocations.
//
// GUIViewController — vtable @0x635648, 104 bytes = 16-byte header +
// 11 slots (address point 0x635658):
//   slot 0 (+0)  ~GUIViewController (D1/D2, @0x529310)
//   slot 1 (+8)  ~GUIViewController (D0, @0x529414)
//   slot 2 (+16) NewLoadingView (@0x41C3CC, WEAK)
//   slot 3 (+24) LoadView (@0x529A38, WEAK, empty)
//   slot 4 (+32) SuspendView (@0x41C414, WEAK, empty)
//   slot 5 (+40) ResumeView (@0x41C418, WEAK, empty)
//   slot 6 (+48) UnloadView (@0x41C41C, WEAK, empty)
//   slot 7 (+56) ViewDidAppear (@0x41C420, WEAK, empty)
//   slot 8 (+64) ViewWillDisappear (@0x529438)
//   slot 9 (+72) Update(float) (@0x52965C)
//   slot 10 (+80) PresentModalViewController(shared_ptr const&) (@0x529758)
// The +32/+40/+48 offsets of SuspendView/ResumeView/UnloadView match
// the CaverShell call-site anchors and the GUINavigationController
// primary vtable @0x634F70 (its overrides sit at the same offsets).
// TransitionToViewController and ViewControllerViewLoaded are NOT
// GUIViewController methods — they belong to IGUINavigationDelegate
// (below) and enter GUINavigationController through that interface.
//
// NewLoadingView returns boost::shared_ptr<GUILoadingView> BY VALUE
// (hidden sret pointer): operator new(0x108) + GUILoadingView ctor +
// shared_ptr<GUILoadingView> ctor at @0x41C3CC.
//
// Layout (sizeof = 72, from ctor @0x5292EC and dtor @0x529310; the
// GUINavigationController delegate subobject sits at +72, which fixes
// the size):
//   +0  vptr
//   +8  enable_shared_from_this<GUIViewController> weak_ptr
//       (px +8, refcount block +16)
//   +24 shared_ptr<GUIView> m_view (px +24, block +32)
//   +40 GUIViewController* m_presentingViewController
//       (PresentModalViewController @0x529758 writes `this` into the
//        modal controller's +40; DismissModalViewController clears it.
//        The ctor does NOT initialise +40 — the 36-byte ctor stores
//        to +8,+16,+24,+32,+48,+56,+64 only.)
//   +48 shared_ptr<GUIViewController> m_modalViewController (+48/+56)
//   +64 IGUINavigationDelegate* m_navigationDelegate
//
// NOTE: the pre-recovery stub of GUINavigationController that used to
// live in this header has been removed — the real, vtable-verified
// declaration is in GUINavigationController.h (which currently carries
// its own copy of the GUIViewController declaration; the two headers
// cannot be included in one TU until the merge reconciles them).
#ifndef CAVER_GUIVIEWCONTROLLER_H
#define CAVER_GUIVIEWCONTROLLER_H

#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>

#include "GUIFileDialog.h"
#include "GUIView.h"

#include "GUILoadingView.h"

namespace Caver {

class GUIEvent;
class GUIViewController;

// IGUINavigationDelegate — RTTI @0x634FF8 is a plain
// __class_type_info (no bases). GUINavigationController's secondary
// vtable has exactly two slots for it, in this order:
//   slot 0 ViewControllerViewLoaded(GUIViewController*)
//   slot 1 TransitionToViewController(shared_ptr<GUIViewController>
//                                     const&, float, float, bool)
// No destructor slot exists in the secondary vtable.
class IGUINavigationDelegate {
public:
    virtual void ViewControllerViewLoaded(GUIViewController* controller) = 0; // slot 0
    virtual void TransitionToViewController(
        const boost::shared_ptr<GUIViewController>& controller,
        float duration, float delay, bool animate) = 0;                        // slot 1
};

class GUIViewController : public boost::enable_shared_from_this<GUIViewController> {
public:
    GUIViewController();
    virtual ~GUIViewController();

    virtual boost::shared_ptr<GUILoadingView> NewLoadingView();   // slot 2
    virtual void LoadView();                                      // slot 3
    virtual void SuspendView();                                   // slot 4
    virtual void ResumeView();                                    // slot 5
    virtual void UnloadView();                                    // slot 6
    virtual void ViewDidAppear();                                 // slot 7
    virtual void ViewWillDisappear();                             // slot 8
    virtual void Update(float deltaTime);                         // slot 9
    virtual void PresentModalViewController(
        const boost::shared_ptr<GUIViewController>& controller);  // slot 10

    void DismissModalViewController();

    // Not in the census as an out-of-line function: the original is a
    // header inline. Every call site in the binary shows the same
    // shape — if the view pointer (+24) is null, call LoadView()
    // (vtable slot 3), then return a copy of the member shared_ptr.
    boost::shared_ptr<GUIView> View()
    {
        if (!m_view)
            LoadView();
        return m_view;
    }

    boost::shared_ptr<GUIView> m_view;                            // +24
    GUIViewController* m_presentingViewController;                // +40
    boost::shared_ptr<GUIViewController> m_modalViewController;   // +48
    IGUINavigationDelegate* m_navigationDelegate;                 // +64
};

class MainMenuViewController;

class PlayerProfile;
class GameViewController;

// GUIFileDialog: the duplicate stub that used to live here has been
// removed in favour of the recovered declaration in GUIFileDialog.h
// (included above, which is also how CaverShell.cpp — this header's
// other consumer — still sees the class). Verified facts: the class
// has NO vtable/RTTI in the golden binary at all (pure interface);
// slots pinned from CaverShell::InitView call sites: +0 no-arg
// (Open), +16 two-string SetFilter ("Scene files", "*.scene"),
// +24 SelectedFilePath returning const std::string&; slot 1 (+8) is
// never called. GUIFileDialog::New() always returns null in this
// build, so the InitView dialog branch is dead on Android.

} // namespace Caver

#endif
