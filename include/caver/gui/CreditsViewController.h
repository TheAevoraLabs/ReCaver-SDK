// Recovered from libswordigo.so (1.4.13) evidence — IDA/Hex-Rays
// exports in ~/workspace/analysis/swordigo/functions/Caver/
// CreditsViewController/, the golden vtable
// _ZTVN5Caver21CreditsViewControllerE @0x62CC38 (104 bytes,
// recovered from ABS64 relocations) and the __si RTTI @0x62CCA0
// (24 bytes, single base).
//
// CreditsViewController is the credits-screen controller the main
// menu transitions to (MainMenuViewController::
// MainMenuViewDidGoToCredits @0x442328 and GameViewController::
// GotoCredits @0x4306BC): both call sites' operator new(0x58)
// fixes sizeof = 88, and both transition to it with (0.4f, 0.4f) —
// animated=true from the main menu, false from the game.
//
// Bases:
//   +0   GUIViewController (public, the ONLY base — the RTTI is a
//        __si_class_type_info, unlike the delegate-base siblings)
// Primary vtable (address point 0x62CC48, 11 slots):
//   0 ~CreditsViewController (D1/D2 @0x456EC0)  1 ~ (D0 @0x456F44)
//   2 GUIViewController::NewLoadingView (not overridden)
//   3 LoadView (@0x456A7C)
//   4 GUIViewController::SuspendView (not overridden)
//   5 GUIViewController::ResumeView (not overridden)
//   6 GUIViewController::UnloadView (not overridden)
//   7 ViewDidAppear (@0x456EB8)
//   8 GUIViewController::ViewWillDisappear (not overridden)
//   9 GUIViewController::Update (not overridden)
//   10 GUIViewController::PresentModalViewController (not overridden)
//
// Layout (creation-site ctor @0x442328 zero-store map; destructor
// @0x456EC0 member-release: the +72 shared_ptr's block at +80 is
// released before the base destructor runs):
//   +72  shared_ptr<CreditsView> — the view built by LoadView
//        (operator new(0x120) = the 288-byte CreditsView); also
//        adopted as the controller's m_view. Name UNRECOVERED.
// There are no other members.
//
// NOTE: no user-declared constructor or destructor — there is NO
// out-of-line C1/C2 symbol for this class in the golden at all: the
// creation sites carry the whole construction inlined (zero
// stores, the GUIViewController base ctor call, the +72/+80 zero
// store, the vptr store), the shape of an implicit default
// constructor. The golden D0 @0x456F44 is likewise a full clone of
// the D2 body plus the operator delete tail, the implicit-
// destructor shape (the PauseViewController /
// OnlineMenuViewController pattern), NOT a call-D1 thunk.
#ifndef CAVER_CREDITSVIEWCONTROLLER_H
#define CAVER_CREDITSVIEWCONTROLLER_H

#include <boost/shared_ptr.hpp>

// The GUIViewController base declaration is taken from
// GUINavigationController.h (the MainMenuViewController pattern):
// GUIViewController.h additionally defines a stub
// MainMenuViewController whose name the .cpp needs for its own
// proven-shape declaration of the real class. The two base
// declarations are ABI-identical; reconcile at merge.
#include "GUINavigationController.h"

namespace Caver {

class CreditsView;
class GUIEvent;

class CreditsViewController : public GUIViewController {
public:
    // No user-declared constructor or destructor: both are implicit
    // in the golden (see header note).

    // GUIViewController overrides (primary slots 3, 7).
    virtual void LoadView();
    virtual void ViewDidAppear();

    // Non-virtual: the boost::bind target LoadView registers on the
    // CreditsView's back button (+272) for control event 64.
    void ButtonPressed(void* sender, GUIEvent* event);

    boost::shared_ptr<CreditsView> m_creditsView; // +72  name UNRECOVERED
};

} // namespace Caver

#endif
