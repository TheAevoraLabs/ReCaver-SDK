// Recovered from libswordigo.so (1.4.13) evidence — IDA/Hex-Rays exports
// in ~/workspace/analysis/swordigo/functions/Caver/
// ConfigureOverlayViewController/, the golden vtable
// _ZTVN5Caver30ConfigureOverlayViewControllerE @0x629B38
// (address point 0x629B48, recovered from ABS64 relocations) and the
// operator new(0x70) at the presenting call site
// (GameMenuViewController::SettingsViewWantsToConfigureControls
// @0x4222B8, which also copies its own shared_ptr<PlayerProfile> into
// this controller's +80 member).
//
// ConfigureOverlayViewController is the "configure controls" modal:
// it builds a ConfigureOverlayView, feeds it the profile's game state
// and saves the edited controls layout back to GameOptions whenever
// the view reports a layout change.
//
// Bases (vtable/RTTI order):
//   +0   GUIViewController (public)
//   +72  IConfigureOverlayViewDelegate (secondary @0x629BB0, address
//        point 0x629BC0; its RTTI @0x629BD0 is a plain
//        __class_type_info — a bare vptr, no data, no destructor
//        slot; the interface is declared in ConfigureOverlayView.h)
// Primary vtable (address point 0x629B48, 13 slots):
//   0 ~ConfigureOverlayViewController (D1/D2 @0x41C204)
//   1 ~ConfigureOverlayViewController (D0 @0x41C2E4)
//   2 GUIViewController::NewLoadingView (not overridden)
//   3 LoadView (@0x41BBB0)
//   4 GUIViewController::SuspendView (not overridden)
//   5 GUIViewController::ResumeView (not overridden)
//   6 GUIViewController::UnloadView (not overridden)
//   7 GUIViewController::ViewDidAppear (not overridden)
//   8 ViewWillDisappear (@0x41BEF4)
//   9 GUIViewController::Update (not overridden)
//   10 GUIViewController::PresentModalViewController (not overridden)
//   11 ConfigureOverlayViewLayoutDidChange (@0x41BF00)
//   12 ConfigureOverlayViewDismissButtonPressed (@0x41C1F4)
// Secondary vtable (-72): exactly two slots, the _ZThn72_ thunks to
// ConfigureOverlayViewLayoutDidChange (@0x41C1F0) and
// ConfigureOverlayViewDismissButtonPressed (@0x41C1FC), in the
// IConfigureOverlayViewDelegate declaration order.
//
// Layout (sizeof = 112, from the presenting site's operator
// new(0x70), the ctor @0x41BB70 32-byte zero store at +80 and the
// destructor @0x41C204 member-release order — the +96 shared_ptr's
// block at +104 is released before the +80 shared_ptr's block at
// +88, exact reverse declaration order):
//   +0..71  GUIViewController base
//   +72     IConfigureOverlayViewDelegate vptr
//   +80     shared_ptr<PlayerProfile> m_profile (copied in by
//           GameMenuViewController::SettingsViewWantsToConfigure-
//           Controls; LoadView reads the profile's shared_ptr<
//           GameState> at PlayerProfile+240 through it)
//   +96     shared_ptr<ConfigureOverlayView> — the view built by
//           LoadView (operator new(0x180) = the 384-byte
//           ConfigureOverlayView); also adopted as the controller's
//           m_view. Name UNRECOVERED.
// Member names marked UNRECOVERED were not found in any evidence;
// only offsets, types and roles are proven.
//
// NOTE: no user-declared destructor — the golden D2 @0x41C204
// (224 bytes) / D0 @0x41C2E4 (232 bytes) pair is a full clone of the
// member-release body plus the operator delete tail, the shape clang
// emits for an implicit destructor (the GameOverViewController /
// PauseViewController pattern), NOT the 36-byte call-D1 thunk a
// user-declared destructor produces here.
#ifndef CAVER_CONFIGUREOVERLAYVIEWCONTROLLER_H
#define CAVER_CONFIGUREOVERLAYVIEWCONTROLLER_H

#include <boost/shared_ptr.hpp>

#include "ConfigureOverlayView.h" // IConfigureOverlayViewDelegate
#include "GUIViewController.h"
#include "../systems/PlayerProfile.h"

namespace Caver {

class ConfigureOverlayViewController : public GUIViewController,
                                       public IConfigureOverlayViewDelegate {
public:
    ConfigureOverlayViewController();
    // No user-declared destructor: the golden D2 @0x41C204 / D0
    // @0x41C2E4 pair is the implicit-destructor shape (see above).

    // GUIViewController overrides (primary slots 3, 8).
    virtual void LoadView();
    virtual void ViewWillDisappear();

    // IConfigureOverlayViewDelegate (primary slots 11, 12, in the
    // interface's declaration order; also reached through the +72
    // secondary vtable via the _ZThn72_ thunks).
    virtual void ConfigureOverlayViewLayoutDidChange(ConfigureOverlayView* view);
    virtual void ConfigureOverlayViewDismissButtonPressed(ConfigureOverlayView* view);

    boost::shared_ptr<PlayerProfile> m_profile;               // +80
    boost::shared_ptr<ConfigureOverlayView> m_configureView;  // +96  name UNRECOVERED
};

static_assert(sizeof(ConfigureOverlayViewController) == 112,
              "ConfigureOverlayViewController layout: presenting "
              "site's operator new(0x70)");

} // namespace Caver

#endif
