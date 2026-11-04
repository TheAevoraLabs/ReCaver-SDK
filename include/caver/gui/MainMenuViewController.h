// Recovered from libswordigo.so (1.4.13) evidence — IDA/Hex-Rays exports
// in ~/workspace/analysis/swordigo/functions/Caver/MainMenuViewController/
// plus the golden vtable group @0x62B668 (recovered from ABS64
// relocations) and the __vmi RTTI @0x62B7F0 (6 bases).
//
// MainMenuViewController — the title-screen controller. Bases (RTTI
// order, offset_flags in parentheses):
//   +0   GUIViewController (0x2)
//   +72  IGUIAlertViewDelegate (0x4800)
//   +80  IMainMenuViewDelegate (0x5000)
//   +88  IProfileManagerDelegate (0x5800)
//   +96  IProfileSelectionViewControllerDelegate (0x6000)
//   +104 IChooseAgeViewDelegate (0x6800)
// Each delegate interface is a bare vptr (no data, no destructor slot):
// the matching secondary vtables contain exactly the interface's
// methods as _ZThn<adj>_ thunks and nothing else.
//
// Primary vtable (address point 0x62B678, 21 slots):
//   0/1 ~MainMenuViewController D1/D0 (@0x43F3EC / @0x43F5B8)
//   2   GUIViewController::NewLoadingView (not overridden)
//   3   LoadView (@0x43F5DC)        4 SuspendView (@0x440128, empty)
//   5   ResumeView (@0x44012C)      6 UnloadView (@0x4401D8, empty)
//   7   ViewDidAppear (@0x43FE24)   8 GUIViewController::ViewWillDisappear
//   9   Update (@0x4401DC)          10 GUIViewController::PresentModalViewController
//   11  ChooseAgeViewAgeChanged (@0x44171C)
//   12  ProfileManagerDidDownloadProfile (@0x441984)
//   13  ProfileSelectionViewControllerDidStart (@0x4419AC)
//   14  MainMenuViewDidStart (@0x441FBC)
//   15  MainMenuViewDidOpenShop (@0x442154)
//   16  MainMenuViewDidGoToCredits (@0x442328)
//   17  MainMenuViewDidShowOfflineAchievements (@0x4424E8)
//   18  MainMenuViewDidOpenPrivacyPolicy (@0x441554)
//   19  AlertViewClickedButtonAtIndex (@0x4417B4, returns bool)
//   20  AlertViewWasDismissed (@0x441880)
// Secondary vtables:
//   -72  IGUIAlertViewDelegate: AlertClicked (Thn72), AlertWasDismissed (Thn72)
//   -80  IMainMenuViewDelegate: DidStart, DidOpenShop, DidGoToCredits,
//        DidShowOfflineAchievements, DidOpenPrivacyPolicy (all Thn80)
//   -88  IProfileManagerDelegate: ProfileManagerDidDownloadProfile (Thn88)
//   -96  IProfileSelectionViewControllerDelegate: DidStart (Thn96)
//   -104 IChooseAgeViewDelegate: ChooseAgeViewAgeChanged (Thn104)
//
// Layout (sizeof = 328 = 0x148, from the ctor @0x43F154 store map, the
// dtor @0x43F3EC member-release order — string, +264, +248, +232,
// GameOptions, +112, base — and the operator new(0x148) in
// GameMenuViewController::GameMenuViewControllerDidQuitToMenu):
//   +112 shared_ptr<MainMenuView> — built by LoadView; its delegate
//        (+256) is set to this+80 and its SceneView (+264) initialised
//        with the "menu" scene.
//   +128 GameOptions (by value, 104 bytes — the member span to +232;
//        the stand-alone GameOptions recovery belongs to the core
//        slice; only the size is proven here)
//   +232 shared_ptr<ProfileSelectionViewController> — created lazily
//        by MainMenuViewDidStart; its delegate (+80) is set to this+96
//        and its ProfileSelectionView (+88) is what
//        ProfileManagerDidDownloadProfile reloads.
//   +248 shared_ptr<GUIAlertView> — the neutral-age dialog built by
//        ShowNeutralAgeScreenDialog (ChooseAgeViewAgeChanged reads its
//        first button; AlertViewWasDismissed calls
//        OnlineController::EnteredAge(m_age) for it).
//   +264 shared_ptr<GUIAlertView> — the privacy-consent dialog built by
//        ShowPrivacyPolicyConsentDialog; AlertViewClickedButtonAtIndex
//        dispatches its link indices (below).
//   +280 int = -1 — AddLink index of "Show Data ID"
//        (AlertViewClickedButtonAtIndex -> ShowAnalyticsIdPopup)
//   +284 int = -1 — AddLink index of "Change Consent Options"
//        (-> ShowPlatformConsentOptions)
//   +288 int = -1 — AddLink index of "Open Privacy Policy"
//        (-> OpenPrivacyPolicy)
//   +292 float = 0 — Update() accumulator; the main view's
//        userInteractionEnabled follows (accumulator > 0.5f)
//   +296 bool = false — set by GoogleSignInCompletion(bool, ...)
//   +297 bool = false — set by CompleteInitialPrivacy(); gates the
//        privacy flow in Update / DidOpenPrivacyPolicy
//   +300 int = 0 — the age from ChooseAgeViewAgeChanged, passed to
//        OnlineController::EnteredAge when the age dialog is dismissed
//   +304 std::string — OnlineController::GetAnalyticsId(), assigned in
//        ShowPrivacyPolicyConsentDialog; its emptiness gates the
//        "Show Data ID" link.
// Member names are UNRECOVERED; the roles above are proven by the
// bodies cited.
#ifndef CAVER_MAINMENUVIEWCONTROLLER_H
#define CAVER_MAINMENUVIEWCONTROLLER_H

#include <string>

#include <boost/shared_ptr.hpp>

#include "GUINavigationController.h"

#include "../core/GameOptions.h"

namespace Caver {

class ChooseAgeView;
class GUIAlertView;
class MainMenuView;
class PlayerProfile;
class ProfileManager;
class ProfileSelectionView;
class ProfileSelectionViewController;

// Delegate interfaces (bare vptrs, no destructor — see header note).
// IGUIAlertViewDelegate is token-identical to the declaration in
// GuidePurchaseViewController.h; the two must be unified at merge.
#ifndef CAVER_IGUIALERTVIEWDELEGATE_DEFINED
#define CAVER_IGUIALERTVIEWDELEGATE_DEFINED
class IGUIAlertViewDelegate {
public:
    // Returns bool: the golden bodies return 1 on the handled paths
    // (GuidePurchaseViewController @0x43C934 is `mov w0,#1; ret`, and
    // MainMenuViewController's body @0x4417B4 returns 1/0 by path).
    virtual bool AlertViewClickedButtonAtIndex(GUIAlertView* alertView, int index) = 0; // slot 0
    virtual void AlertViewWasDismissed(GUIAlertView* alertView) = 0;                    // slot 1
};
#endif

#ifndef IMAINMENUVIEWDELEGATE_DEFINED
#define IMAINMENUVIEWDELEGATE_DEFINED
class IMainMenuViewDelegate {
public:
    virtual void MainMenuViewDidStart(MainMenuView* view) = 0;                        // slot 0
    virtual void MainMenuViewDidOpenShop(MainMenuView* view) = 0;                     // slot 1
    virtual void MainMenuViewDidGoToCredits(MainMenuView* view) = 0;                  // slot 2
    virtual void MainMenuViewDidShowOfflineAchievements(MainMenuView* view) = 0;      // slot 3
    virtual void MainMenuViewDidOpenPrivacyPolicy(MainMenuView* view) = 0;            // slot 4
};
#endif

// IProfileManagerDelegate is token-identical to the declaration in
// GameViewController.h; the two must be unified at merge.
#ifndef CAVER_IPROFILEMANAGERDELEGATE_DEFINED
#define CAVER_IPROFILEMANAGERDELEGATE_DEFINED
class IProfileManagerDelegate {
public:
    virtual void ProfileManagerDidDownloadProfile(ProfileManager* manager, const std::string& profileName) = 0;
};
#endif

#ifndef CAVER_IPROFILESELECTIONVIEWCONTROLLERDELEGATE_DEFINED
#define CAVER_IPROFILESELECTIONVIEWCONTROLLERDELEGATE_DEFINED
class IProfileSelectionViewControllerDelegate {
public:
    virtual void ProfileSelectionViewControllerDidStart(
        ProfileSelectionView* view,
        const boost::shared_ptr<PlayerProfile>& profile) = 0;                         // slot 0
};
#endif

class IChooseAgeViewDelegate {
public:
    virtual void ChooseAgeViewAgeChanged(ChooseAgeView* view) = 0;                    // slot 0
};

class MainMenuViewController : public GUIViewController,
                               public IGUIAlertViewDelegate,
                               public IMainMenuViewDelegate,
                               public IProfileManagerDelegate,
                               public IProfileSelectionViewControllerDelegate,
                               public IChooseAgeViewDelegate {
public:
    MainMenuViewController();
    virtual ~MainMenuViewController();

    // GUIViewController overrides (primary slots 3-7, 9).
    virtual void LoadView();                                      // slot 3
    virtual void SuspendView();                                   // slot 4 (empty)
    virtual void ResumeView();                                    // slot 5
    virtual void UnloadView();                                    // slot 6 (empty)
    virtual void ViewDidAppear();                                 // slot 7
    virtual void Update(float deltaTime);                         // slot 9

    // Delegate methods, in golden primary-vtable order (slots 11-20).
    virtual void ChooseAgeViewAgeChanged(ChooseAgeView* view);    // slot 11
    virtual void ProfileManagerDidDownloadProfile(ProfileManager* manager, const std::string& profileName); // slot 12
    virtual void ProfileSelectionViewControllerDidStart(
        ProfileSelectionView* view,
        const boost::shared_ptr<PlayerProfile>& profile);         // slot 13
    virtual void MainMenuViewDidStart(MainMenuView* view);        // slot 14
    virtual void MainMenuViewDidOpenShop(MainMenuView* view);     // slot 15
    virtual void MainMenuViewDidGoToCredits(MainMenuView* view);  // slot 16
    virtual void MainMenuViewDidShowOfflineAchievements(MainMenuView* view); // slot 17
    virtual void MainMenuViewDidOpenPrivacyPolicy(MainMenuView* view);       // slot 18
    virtual bool AlertViewClickedButtonAtIndex(GUIAlertView* alertView, int index); // slot 19
    virtual void AlertViewWasDismissed(GUIAlertView* alertView);  // slot 20

    // Non-virtual methods (census functions; GoogleSignInCompletion is
    // the boost::bind target registered with the OnlineController in
    // LoadView).
    void GoogleSignInCompletion(bool success, const std::string& error);
    void CompleteInitialPrivacy();
    void ShowNeutralAgeScreenDialog();
    void ShowPrivacyPolicyConsentDialog();

    boost::shared_ptr<MainMenuView> m_mainMenuView;               // +112 (name UNRECOVERED)
    GameOptions m_options;                                        // +128 (name UNRECOVERED)
    boost::shared_ptr<ProfileSelectionViewController>
        m_profileSelectionViewController;                         // +232 (name UNRECOVERED)
    boost::shared_ptr<GUIAlertView> m_ageAlertView;               // +248 (name UNRECOVERED)
    boost::shared_ptr<GUIAlertView> m_consentAlertView;           // +264 (name UNRECOVERED)
    int m_showDataIdLinkIndex = -1;                               // +280 (name UNRECOVERED)
    int m_changeConsentOptionsLinkIndex = -1;                     // +284 (name UNRECOVERED)
    int m_openPrivacyPolicyLinkIndex = -1;                        // +288 (name UNRECOVERED)
    float m_updateTimer = 0.0f;                                   // +292 (name UNRECOVERED)
    bool m_googleSignedIn = false;                                // +296 (name UNRECOVERED)
    bool m_privacyCompleted = false;                              // +297 (name UNRECOVERED)
    int m_age = 0;                                                // +300 (name UNRECOVERED)
    std::string m_analyticsId;                                    // +304 (name UNRECOVERED)
};

// Golden aarch64 NDK-libc++ layout anchor; MSVC STL member sizes differ,
// so this assert is Android/Linux-only (the Windows build is self-consistent
// under MSVC STL and does not claim golden byte-layout parity).
#ifndef _MSC_VER
static_assert(sizeof(MainMenuViewController) == 328,
              "MainMenuViewController layout: ctor/dtor-proven, ends at +328");
#endif

} // namespace Caver

#endif
