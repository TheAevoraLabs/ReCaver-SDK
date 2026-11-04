// Recovered from libswordigo.so (1.4.13) evidence — IDA/Hex-Rays exports
// in ~/workspace/analysis/swordigo/functions/Caver/
// OfflineAchievementsViewController/, the golden vtable
// _ZTVN5Caver33OfflineAchievementsViewControllerE @0x62C350
// (address point 0x62C360, recovered from ABS64 relocations) and the
// operator new(0x68) at both presenting call sites
// (GameMenuViewController::SettingsViewDidShowOfflineAchievements
// @0x422910 and MainMenuViewController::
// MainMenuViewDidShowOfflineAchievements @0x4424E8).
//
// OfflineAchievementsViewController is the offline achievements modal.
// Single base GUIViewController (+0); no secondary bases (the RTTI
// @0x62C3B8 is a plain __si_class_type_info on GUIViewController).
//
// Primary vtable (address point 0x62C360, 11 slots — exactly the
// GUIViewController set, no new virtuals):
//   0 ~OfflineAchievementsViewController (D1/D2 @0x450F94)
//   1 ~OfflineAchievementsViewController (D0 @0x45106C)
//   2 GUIViewController::NewLoadingView (not overridden)
//   3 LoadView (@0x451090)
//   4 SuspendView (@0x451788, empty)
//   5 ResumeView (@0x45178C, empty)
//   6 UnloadView (@0x451790, empty)
//   7 ViewDidAppear (@0x451748)
//   8 GUIViewController::ViewWillDisappear (not overridden)
//   9 GUIViewController::Update (not overridden)
//   10 GUIViewController::PresentModalViewController (not overridden)
// ButtonPressed / GoogleSignInHandler are plain methods: both reach
// this class only through boost::bind target-actions built by
// LoadView, never through a vtable.
//
// Layout (sizeof = 104, from the presenting sites' operator
// new(0x68), the ctor @0x450F58 zero-store map +72/+88 and the
// destructor @0x450F94 member-release order — the +88 shared_ptr's
// block at +96 is released before the +72 shared_ptr's block at +80,
// exact reverse declaration order):
//   +0..71  GUIViewController base
//   +72     shared_ptr<PlayerProfile> m_profile (copied in by
//           GameMenuViewController::SettingsViewDidShowOffline-
//           Achievements; LoadView passes it to the
//           OfflineAchievementsView ctor, and when it is null
//           LoadView first asks ProfileManager to load all profiles)
//   +88     shared_ptr<OfflineAchievementsView> — the view built by
//           LoadView (operator new(0x178) = the 376-byte
//           OfflineAchievementsView); also adopted as the
//           controller's m_view. Name UNRECOVERED.
// Member names marked UNRECOVERED were not found in any evidence;
// only offsets, types and roles are proven.
//
// NOTE: the golden D0 @0x45106C (36 bytes) is the call-D1-plus-
// operator-delete shape, i.e. the destructor is user-declared and
// defined out-of-line (the PortalViewController pattern), NOT the
// full-clone implicit-destructor shape of GameOver/Purchase.
#ifndef CAVER_OFFLINEACHIEVEMENTSVIEWCONTROLLER_H
#define CAVER_OFFLINEACHIEVEMENTSVIEWCONTROLLER_H

#include <string>

#include <boost/shared_ptr.hpp>

#include "GUIViewController.h"

namespace Caver {

class GUIEvent;
class OfflineAchievementsView;
class PlayerProfile;

class OfflineAchievementsViewController : public GUIViewController {
public:
    OfflineAchievementsViewController();
    virtual ~OfflineAchievementsViewController();

    // GUIViewController overrides (primary slots 3, 4, 5, 6, 7).
    virtual void LoadView();
    virtual void SuspendView();
    virtual void ResumeView();
    virtual void UnloadView();
    virtual void ViewDidAppear();

    // Non-virtual methods (census functions; bodies in the .cpp).
    // Bound by LoadView: ButtonPressed to the view's three buttons
    // (GUIControl event 64), GoogleSignInHandler to the online
    // controller's Google-services sign-in handler list.
    void ButtonPressed(void* sender, GUIEvent* event);
    void GoogleSignInHandler(bool signedIn, const std::string& error);

    boost::shared_ptr<PlayerProfile> m_profile;                        // +72
    boost::shared_ptr<OfflineAchievementsView> m_offlineAchievementsView; // +88 name UNRECOVERED
};

static_assert(sizeof(OfflineAchievementsViewController) == 104,
              "OfflineAchievementsViewController layout: presenting "
              "sites' operator new(0x68)");

} // namespace Caver

#endif
