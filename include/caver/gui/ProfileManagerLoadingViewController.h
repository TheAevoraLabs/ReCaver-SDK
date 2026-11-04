// Recovered from libswordigo.so (1.4.13) evidence — IDA/Hex-Rays
// exports in ~/workspace/analysis/swordigo/functions/Caver/
// ProfileManagerLoadingViewController/, the golden disassembly, and
// the golden vtable / RTTI recovered from ABS64 relocations.
//
// ProfileManagerLoadingViewController — the loading step that
// precedes profile selection: ProfileManager_Android::
// ShowCloudLoadingView creates it (operator new(0x60)) and presents
// it as a modal while cloud saves download; its
// SkipLoadingButtonPressed / HideCloudLoadingView call
// DismissAfterMinimumDelay, and its Update fades the loading view
// out and dismisses the modal once the view has been alive at least
// the requested minimum delay.
//
// Bases: single inheritance only — GUIViewController at +0 (the
// __si_class_type_info RTTI @0x62DB30 proves one base; there is no
// secondary vtable and no delegate interface, unlike the sibling
// Pause / ProfileSelection controllers).
//
// Primary vtable (address point 0x62DAD8, vtable base 0x62DAC8,
// 11 slots — the GUIViewController slot count, from relocations):
//   0 ~ProfileManagerLoadingViewController (D1/D2 @0x46BDA4)
//   1 ~ProfileManagerLoadingViewController (D0 @0x46BE28)
//   2 GUIViewController::NewLoadingView (not overridden)
//   3 LoadView (@0x46BA1C)
//   4 GUIViewController::SuspendView (not overridden)
//   5 GUIViewController::ResumeView (not overridden)
//   6 GUIViewController::UnloadView (not overridden)
//   7 GUIViewController::ViewDidAppear (not overridden)
//   8 GUIViewController::ViewWillDisappear (not overridden)
//   9 Update (@0x46BC60)
//   10 GUIViewController::PresentModalViewController (not overridden)
// DismissAfterMinimumDelay (@0x46BD94) is NOT virtual — no vtable
// slot carries it; callers reach it through the import thunk
// @0x60D090.
//
// Layout (sizeof = 96 = 0x60, from the ctor @0x46B9DC store map —
// byte +72, qwords +76/+84, dword +92 — the dtor @0x46BDA4
// releasing only the +88 control block, and
// ShowCloudLoadingView's operator new(0x60); the same layout is
// already stated in ProfileManager_Android.cpp's stub of this class,
// which this header supersedes at merge):
//   +72  bool  dismiss-pending flag (set by
//        DismissAfterMinimumDelay, cleared by Update once it fires)
//   +76  float minimum delay (the DismissAfterMinimumDelay argument)
//   +80  shared_ptr<ProfileManagerLoadingView> — the view built by
//        LoadView (operator new(0x130) = the 304-byte
//        ProfileManagerLoadingView); also adopted as the
//        controller's m_view. Name UNRECOVERED for all three members.
//
// NOTE: no user-declared destructor — the golden D0 @0x46BE28 is a
// full clone of the D2 body plus the operator delete tail, the shape
// clang emits for an implicit destructor (the PauseViewController /
// ProfileSelectionViewController pattern), NOT a call-D1 thunk.
#ifndef CAVER_PROFILEMANAGERLOADINGVIEWCONTROLLER_H
#define CAVER_PROFILEMANAGERLOADINGVIEWCONTROLLER_H

#include <boost/shared_ptr.hpp>

#include "GUIViewController.h"

namespace Caver {

class ProfileManagerLoadingView;

class ProfileManagerLoadingViewController : public GUIViewController {
public:
    ProfileManagerLoadingViewController();
    // No user-declared destructor: the golden D2 @0x46BDA4 / D0
    // @0x46BE28 pair is the implicit-destructor shape (see above).

    // GUIViewController overrides (primary slots 3, 9).
    virtual void LoadView();
    virtual void Update(float deltaTime);

    // Non-virtual (see vtable note above).
    void DismissAfterMinimumDelay(float delay);

    bool m_dismissPending;  // +72 (name UNRECOVERED)
    float m_minimumDelay;   // +76 (name UNRECOVERED)
    boost::shared_ptr<ProfileManagerLoadingView>
        m_loadingView;      // +80 (name UNRECOVERED)
};

static_assert(sizeof(ProfileManagerLoadingViewController) == 96,
              "ProfileManagerLoadingViewController layout: ctor/dtor-proven, ends at +96");

} // namespace Caver

#endif
