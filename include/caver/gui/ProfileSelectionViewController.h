// Recovered from libswordigo.so (1.4.13) evidence — IDA/Hex-Rays
// exports in ~/workspace/analysis/swordigo/functions/Caver/
// ProfileSelectionViewController/, the golden disassembly, the
// golden vtable _ZTVN5Caver30ProfileSelectionViewControllerE
// @0x62D628 (address point 0x62D638, secondary address point
// 0x62D6B0) and the __vmi RTTI @0x62D6D0 (2 bases).
//
// ProfileSelectionViewController — the modal controller behind the
// "Saved Games" screen. MainMenuViewController presents it when the
// player asks for profiles (its DidStart creates it with operator
// new(0x68) and writes its own +96 delegate subobject into this
// controller's +80); the controller builds a ProfileSelectionView,
// forwards a started profile to its delegate, and dismisses itself.
//
// Bases (RTTI order; offset_flags in parentheses):
//   +0   GUIViewController (0x2)
//   +72  IProfileSelectionViewDelegate (0x4800) — the interface
//        declared in ProfileSelectionView.h; its RTTI
//        (_ZTIN5Caver29IProfileSelectionViewDelegateE @0x62D6C0) is
//        a plain __class_type_info, a bare vptr with no destructor
//        slot. The view stores this subobject (this + 72) as its
//        m_delegate at +264.
//
// Primary vtable (address point 0x62D638, 13 slots):
//   0 ~ProfileSelectionViewController (D1/D2 @0x468B6C)
//   1 ~ProfileSelectionViewController (D0 @0x468BF8)
//   2 GUIViewController::NewLoadingView (not overridden)
//   3 LoadView (@0x468850)
//   4 GUIViewController::SuspendView (not overridden)
//   5 GUIViewController::ResumeView (not overridden)
//   6 GUIViewController::UnloadView (not overridden)
//   7 ViewDidAppear (@0x468A9C)
//   8 GUIViewController::ViewWillDisappear (not overridden)
//   9 GUIViewController::Update (not overridden)
//   10 GUIViewController::PresentModalViewController (not overridden)
//   11 ProfileSelectionViewPlayerDidStart (@0x468AA4)
//   12 ProfileSelectionViewCloseButtonPressed (@0x468B1C)
// Secondary vtable (address point 0x62D6B0, offset-to-top -72):
// exactly the two _ZThn72_ thunks to slots 11/12, in interface order.
//
// Layout (sizeof = 104 = 0x68, from the ctor @0x468814 store map —
// secondary vptr + a zero qword at +72/+80 in one store pair, the
// shared_ptr zeroed at +88/+96 — the dtor @0x468B6C releasing only
// the +96 control block, and MainMenuViewController's operator
// new(0x68) / its stub of this class in MainMenuViewController.cpp):
//   +80  IProfileSelectionViewControllerDelegate* m_delegate (raw;
//        name UNRECOVERED)
//   +88  shared_ptr<ProfileSelectionView> — the view built by
//        LoadView (operator new(0x140) = the 320-byte
//        ProfileSelectionView); also adopted as the controller's
//        m_view. Name UNRECOVERED.
//
// NOTE: no user-declared destructor — the golden D0 @0x468BF8
// (148 bytes) is a full clone of the D2 body (140 bytes) plus the
// operator delete tail, the shape clang emits for an implicit
// destructor (the PauseViewController / GameOverViewController
// pattern), NOT a call-D1 thunk.
#ifndef CAVER_PROFILESELECTIONVIEWCONTROLLER_H
#define CAVER_PROFILESELECTIONVIEWCONTROLLER_H

#include <boost/shared_ptr.hpp>

// The GUIViewController base declaration is the real one in
// GUIViewController.h (vtable @0x635648): the
// GUINavigationController.h copy of the base does not declare the
// non-virtual DismissModalViewController that the delegate bodies
// call through m_presentingViewController.
#include "GUIViewController.h"

// ProfileSelectionView.h declares IProfileSelectionViewDelegate
// (this class's +72 base) and the complete ProfileSelectionView
// (sizeof 320) whose shared_ptr is the +88 member.
#include "ProfileSelectionView.h"

namespace Caver {

class PlayerProfile;
class ProfileSelectionViewController;

// Delegate notified when a profile is started (raw pointer at +80;
// the implementor is MainMenuViewController, through its +96
// subobject). HAND-OFF: this declaration is token-identical to the
// one in MainMenuViewController.h; the two must be unified at merge.
#ifndef CAVER_IPROFILESELECTIONVIEWCONTROLLERDELEGATE_DEFINED
#define CAVER_IPROFILESELECTIONVIEWCONTROLLERDELEGATE_DEFINED
class IProfileSelectionViewControllerDelegate {
public:
    virtual void ProfileSelectionViewControllerDidStart(
        ProfileSelectionView* view,
        const boost::shared_ptr<PlayerProfile>& profile) = 0;  // slot 0
};
#endif

class ProfileSelectionViewController : public GUIViewController,
                                       public IProfileSelectionViewDelegate {
public:
    ProfileSelectionViewController();
    // No user-declared destructor: the golden D2 @0x468B6C / D0
    // @0x468BF8 pair is the implicit-destructor shape (see above).

    // GUIViewController overrides (primary slots 3, 7).
    virtual void LoadView();
    virtual void ViewDidAppear();

    // IProfileSelectionViewDelegate (primary slots 11, 12).
    virtual void ProfileSelectionViewPlayerDidStart(
        ProfileSelectionView* view,
        const boost::shared_ptr<PlayerProfile>& profile);
    virtual void ProfileSelectionViewCloseButtonPressed(
        ProfileSelectionView* view);

    IProfileSelectionViewControllerDelegate* m_delegate;  // +80 (name UNRECOVERED)
    boost::shared_ptr<ProfileSelectionView>
        m_profileSelectionView;                           // +88 (name UNRECOVERED)
};

static_assert(sizeof(ProfileSelectionViewController) == 104,
              "ProfileSelectionViewController layout: ctor/dtor-proven, ends at +104");

} // namespace Caver

#endif
