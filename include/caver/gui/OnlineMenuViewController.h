// Recovered from libswordigo.so (1.4.13) evidence — IDA/Hex-Rays
// exports in ~/workspace/analysis/swordigo/functions/Caver/
// OnlineMenuViewController/, the golden vtable
// _ZTVN5Caver24OnlineMenuViewControllerE @0x62D088 (104 bytes,
// recovered from ABS64 relocations) and the __si RTTI @0x62D0F0
// (24 bytes, single base).
//
// OnlineMenuViewController is the modal controller
// GameMenuViewController::SettingsViewWantsToPresentOnlineView
// (@0x422644) presents for the online (Achievements / Leaderboards)
// panel: that call site's operator new(0x58) fixes sizeof = 88, and
// it presents this controller through the presenting controller's
// vtable slot 10 (PresentModalViewController).
//
// Bases:
//   +0   GUIViewController (public, the ONLY base — the RTTI is a
//        __si_class_type_info, unlike the delegate-base siblings)
// Primary vtable (address point 0x62D098, 11 slots):
//   0 ~OnlineMenuViewController (D1/D2 @0x45E3A0)  1 ~ (D0 @0x45E424)
//   2 GUIViewController::NewLoadingView (not overridden)
//   3 LoadView (@0x45E010)
//   4 GUIViewController::SuspendView (not overridden)
//   5 GUIViewController::ResumeView (not overridden)
//   6 GUIViewController::UnloadView (not overridden)
//   7 ViewDidAppear (@0x45E398)
//   8 GUIViewController::ViewWillDisappear (not overridden)
//   9 GUIViewController::Update (not overridden)
//   10 GUIViewController::PresentModalViewController (not overridden)
//
// Layout (creation-site ctor @0x422644 zero-store map; destructor
// @0x45E3A0 member-release: the +72 shared_ptr's block at +80 is
// released before the base destructor runs):
//   +72  shared_ptr<OnlineMenuView> — the view built by LoadView
//        (operator new(0x158) = the 344-byte OnlineMenuView); also
//        adopted as the controller's m_view. Name UNRECOVERED.
// There are no other members.
//
// NOTE: no user-declared constructor or destructor — there is NO
// out-of-line C1/C2 symbol for this class in the golden at all: the
// creation site @0x422644 carries the whole construction inlined
// (zero stores, the GUIViewController base ctor call, the +72/+80
// zero store, the vptr store), the shape of an implicit default
// constructor. The golden D0 @0x45E424 is likewise a full clone of
// the D2 body plus the operator delete tail, the implicit-
// destructor shape (the PauseViewController / GameOverViewController
// pattern), NOT a call-D1 thunk.
#ifndef CAVER_ONLINEMENUVIEWCONTROLLER_H
#define CAVER_ONLINEMENUVIEWCONTROLLER_H

#include <boost/shared_ptr.hpp>

#include "GUIViewController.h"

namespace Caver {

class GUIEvent;
class OnlineMenuView;

class OnlineMenuViewController : public GUIViewController {
public:
    // No user-declared constructor or destructor: both are implicit
    // in the golden (see header note).

    // GUIViewController overrides (primary slots 3, 7).
    virtual void LoadView();
    virtual void ViewDidAppear();

    // Non-virtual: the boost::bind target LoadView registers on all
    // three OnlineMenuView buttons (close / achievements /
    // leaderboards) for control event 64.
    void ButtonPressed(void* sender, GUIEvent* event);

    boost::shared_ptr<OnlineMenuView> m_onlineMenuView; // +72  name UNRECOVERED
};

} // namespace Caver

#endif
