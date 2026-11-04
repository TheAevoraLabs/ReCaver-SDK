// Recovered from libswordigo.so (1.4.13) evidence.
// GUINavigationController — and, because the class cannot be declared
// without them, its two bases GUIViewController and
// IGUINavigationDelegate, recovered to the same standard.
//
// NOTE for whoever owns GUIViewController.h: that header still carries
// the pre-recovery stub of this class (and declares
// GUIViewController::NewLoadingView as returning a raw pointer). The
// declarations below are the vtable/RTTI/ctor-verified ones; the stub
// in GUIViewController.h should be deleted in favour of this header.
//
// ---------------------------------------------------------------------
// IGUINavigationDelegate — RTTI @0x634FF8 is a plain
// __class_type_info (no bases). GUINavigationController's secondary
// vtable (below) has exactly two slots for it, in this order:
//   slot 0 ViewControllerViewLoaded(GUIViewController*)
//   slot 1 TransitionToViewController(shared_ptr<GUIViewController> const&,
//                                     float, float, bool)
// No destructor slot exists in the secondary vtable.
//
// GUIViewController — vtable @0x635648, 104 bytes = header + 11 slots:
//   0 ~D2, 1 ~D0, 2 NewLoadingView, 3 LoadView, 4 SuspendView,
//   5 ResumeView, 6 UnloadView, 7 ViewDidAppear, 8 ViewWillDisappear,
//   9 Update(float), 10 PresentModalViewController(shared_ptr const&)
// TransitionToViewController / ViewControllerViewLoaded are NOT
// GUIViewController methods — they enter GUINavigationController
// through IGUINavigationDelegate (primary slots 11/12 below).
// NewLoadingView returns boost::shared_ptr<GUILoadingView> BY VALUE
// (IDA @0x41C3CC: operator new(0x108) + GUILoadingView ctor +
// shared_ptr<GUILoadingView> ctor into the hidden sret pointer).
//
// GUIViewController layout (sizeof = 72, from ctor @0x5292EC and
// dtor @0x529310; the GUINavigationController delegate subobject sits
// at +72, which fixes the size):
//   +0  vptr
//   +8  enable_shared_from_this<GUIViewController> weak_ptr
//       (px +8, refcount block +16 — dtor releases +16 on the weak path)
//   +24 shared_ptr<GUIView> m_view (px +24, block +32)
//   +40 GUIViewController* m_presentingViewController
//       (PresentModalViewController @0x529758 writes `this` into the
//        modal controller's +40; DismissModalViewController clears it.
//        The ctor does NOT initialise +40 — verified: the 36-byte ctor
//        stores to +8,+16,+24,+32,+48,+56,+64 only.)
//   +48 shared_ptr<GUIViewController> m_modalViewController (px +48, block +56)
//   +64 IGUINavigationDelegate* m_navigationDelegate
//       (SetCurrentViewController clears the outgoing controller's +64
//        and stores `this`+72 — the delegate subobject — into the
//        incoming controller's +64; LoadView calls slot 0 through it.)
//
// GUINavigationController — vtable @0x634F60, 152 bytes:
//   primary (GUIViewController subobject, address point 0x634F70):
//     0 ~D2 (@0x522CF0), 1 ~D0 (@0x522DD0),
//     2 GUIViewController::NewLoadingView (not overridden),
//     3 LoadView, 4 SuspendView (+32), 5 ResumeView (+40),
//     6 UnloadView (+48), 7 GUIViewController::ViewDidAppear,
//     8 GUIViewController::ViewWillDisappear, 9 Update,
//     10 PresentModalViewController (4-byte tail-thunk @0x5217D8 to the
//        base implementation — an explicit override that only calls
//        the base), 11 TransitionToViewController,
//     12 ViewControllerViewLoaded
//   secondary (IGUINavigationDelegate subobject at +72,
//   address point 0x634FE8, offset-to-top -72):
//     0 _ZThn72_ ViewControllerViewLoaded, 1 _ZThn72_ TransitionToViewController
// RTTI @0x635008 (__vmi_class_type_info): bases GUIViewController (+0)
// and IGUINavigationDelegate (+72), both public/non-virtual.
// The Suspend/Resume/Unload byte offsets (+32/+40/+48 from the
// address point) match the CaverShell call-site anchor.
//
// Layout (sizeof = 112, ctor @0x521544 zeroes +80..+111 after the
// base ctor; dtor @0x522CF0 releases +96's block before +80's):
//   +0..71   GUIViewController base
//   +72      IGUINavigationDelegate vptr
//   +80      shared_ptr<GUIViewController> m_currentViewController
//   +96      shared_ptr<GUIView> m_transitionView (the loading view
//            produced by NewLoadingView during a transition; treated
//            as a GUIView by RemoveFromSuperview/LayoutSubviews calls)
#ifndef CAVER_GUINAVIGATIONCONTROLLER_H
#define CAVER_GUINAVIGATIONCONTROLLER_H

#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>

#include "GUIView.h"
#include "GUIViewController.h"

namespace Caver {

class GUIAnimation;
class GUIEvent;

class GUINavigationController : public GUIViewController,
                                public IGUINavigationDelegate {
public:
    GUINavigationController();
    virtual ~GUINavigationController();

    // GUIViewController overrides.
    virtual void LoadView();                                      // primary slot 3
    virtual void SuspendView();                                   // primary slot 4
    virtual void ResumeView();                                    // primary slot 5
    virtual void UnloadView();                                    // primary slot 6
    virtual void Update(float deltaTime);                         // primary slot 9
    virtual void PresentModalViewController(
        const boost::shared_ptr<GUIViewController>& controller);  // primary slot 10

    // IGUINavigationDelegate. Declaration order here is load-bearing:
    // the primary vtable appends TransitionToViewController (slot 11)
    // before ViewControllerViewLoaded (slot 12), while the delegate
    // secondary vtable keeps the interface order (ViewLoaded first).
    virtual void TransitionToViewController(
        const boost::shared_ptr<GUIViewController>& controller,
        float duration, float delay, bool animate);               // primary slot 11
    virtual void ViewControllerViewLoaded(
        GUIViewController* controller);                           // primary slot 12

    void SetCurrentViewController(
        const boost::shared_ptr<GUIViewController>& controller);
    // Non-virtual callback target for boost::bind in
    // TransitionToViewController; the golden body @0x5226A0 is a
    // 4-byte tail call to SetCurrentViewController.
    void FinishTransitionToViewController(
        const boost::shared_ptr<GUIViewController>& controller,
        void* sender, GUIEvent* event);

    boost::shared_ptr<GUIViewController> m_currentViewController; // +80
    boost::shared_ptr<GUIView> m_transitionView;                  // +96
};

} // namespace Caver

#endif
