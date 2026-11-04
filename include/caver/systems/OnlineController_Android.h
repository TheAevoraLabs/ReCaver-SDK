// Recovered from libswordigo.so (1.4.13) evidence.
// OnlineController_Android — the JNI-backed OnlineController used on
// Android. Every interface method either forwards to a Caver::Android*
// bridge helper (which calls back into com.touchfoo.swordigo.Native) or
// is a constant / empty body.
//
// Layout (sizeof 0x38) proven from the constructor @0x54063C
// (_ZN5Caver24OnlineController_AndroidC2Ev):
//   +0   vptr
//   +8   int m_age (base OnlineController member; NOT touched by this
//        constructor — the golden body never stores to +8)
//   +16  std::list<TargetAction<void (bool, const std::string&)>>
//        (libc++ __list_imp: __prev_ @+16, __next_ @+24, __size_ @+32;
//        the ctor stores the self-links with stp x9, xzr, [x0, #0x18]
//        and str x9, [x0, #0x10], i.e. an empty list)
//   +40  bool m_signedInToGoogleServices (ctor: strb wzr, [x0, #0x28])
//   +48  double m_lastInterstitialAdTime (ctor: str of 0xC0C3880000000000
//        = -10000.0; read/written by ShowInterstitialAd at [x19, #0x30])
//
// The vtable is _ZTVN5Caver24OnlineController_AndroidE @0x6366E0
// (312 bytes, 37 slots) and its relocations prove exactly which slots
// this class overrides; the four it does not override are
// ShowOfflineAchievements, IsKnownFreeVersion, SetUseTestAds and
// AddProgressionEventSceneStarted. The slot order is the base class's
// declaration order (OnlineController.h).
//
// HandleGoogleSignInCompleted is NOT virtual: no vtable slot points at
// _ZN5Caver24OnlineController_Android27HandleGoogleSignInCompletedEb.
#ifndef CAVER_ONLINECONTROLLER_ANDROID_H
#define CAVER_ONLINECONTROLLER_ANDROID_H

#include <list>
#include <string>

#include "OnlineController.h"

namespace Caver {

class OnlineController_Android : public OnlineController {
public:
    OnlineController_Android();

    // Broadcast to every registered sign-in handler, then drop the ones
    // whose weak_ptr target has died. Bug-for-bug: the signed-in flag is
    // only stored inside the loop, so an empty handler list leaves it
    // unchanged; and nodes whose target is still alive are kept (the
    // golden advances with `ldr x?, [x?, #8]` and only calls the erase
    // helper on the expired path).
    void HandleGoogleSignInCompleted(bool completed);

    bool AchievementsEnabled() override;
    bool LeaderboardsEnabled() override;
    void LaunchAchievements() override;
    void LaunchLeaderboards() override;
    void LaunchFacebook() override;
    void LaunchTwitter() override;

    bool IsAgeKnown() override;
    bool IsAgeOfConsent() override;
    void EnteredAge(int age) override;
    void OpenPrivacyPolicy() override;

    bool ShouldShowInitialPrivacyConsentPopup() override;
    bool HasPrivacyConsent() override;
    void ReceivedPrivacyConsent(bool consent) override;

    void StartAdsAndAnalytics() override;
    std::string GetAnalyticsId() override;
    void ShowAnalyticsIdPopup() override;
    int GetPlatformConsentState() override;
    void ShowPlatformConsentOptions() override;
    void PrepareReviewFlow() override;
    bool StartReviewFlow() override;

    int UnreadNewsCount() override;
    void LaunchNews() override;
    void LaunchFeedbackView() override;

    void ReportAchievementProgress(const boost::shared_ptr<Achievement>& achievement, double progress) override;
    void ReportScore(const std::string& leaderboard, long long score, const std::string& category) override;

    bool GoogleServicesAvailable() override;
    bool SignedInToGoogleServices() override;
    void AddGoogleServicesSignInHandler(TargetAction<void (bool, const std::string&)> handler) override;
    void SignInToGoogleServices() override;

    void ShowInterstitialAd(const std::string& placement, int interval, float delay) override;
    bool IsAdsEnabled() override;

    // Member order is the golden store order, so the list precedes the
    // flag byte and the double. Name of the list member is UNRECOVERED.
    std::list<TargetAction<void (bool, const std::string&)> > m_signInHandlers; // +16
    bool m_signedInToGoogleServices;  // +40
    double m_lastInterstitialAdTime;  // +48
};

// Golden aarch64 NDK-libc++ layout anchor; MSVC STL member sizes differ,
// so this assert is Android/Linux-only (the Windows build is self-consistent
// under MSVC STL and does not claim golden byte-layout parity).
#ifndef _MSC_VER
static_assert(sizeof(OnlineController_Android) == 0x38,
              "OnlineController_Android size must be 0x38");
#endif

} // namespace Caver

#endif // CAVER_ONLINECONTROLLER_ANDROID_H
