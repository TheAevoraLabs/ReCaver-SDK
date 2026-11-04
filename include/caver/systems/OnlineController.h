// Recovered from libswordigo.so (1.4.13) evidence.
// Layout established from the golden binary's own disassembly:
//   - operator new(0x10) in SharedController (0x453690) pins the object
//     size at 16 bytes: vtable pointer at +0 and one int field at +8.
//   - The int at +8 is the entered age: EnteredAge (0x4538B8) stores it,
//     IsAgeKnown (0x453898) tests it != 0, IsAgeOfConsent (0x4538A8)
//     tests it > 15. There are no other data members: the Android
//     subclass (OnlineController_Android, a different slice) starts its
//     own fields at +16.
//   - The vtable (_ZTVN5Caver16OnlineControllerE, 312 bytes at
//     0x62C5E8) has 37 slots: the two destructor entries plus the 35
//     virtuals below, in exactly this order (verified from the
//     .rela.dyn entries for 0x62C5F8..0x62C718). SharedController /
//     SetSharedController are static and not in the vtable.
//   - There is no out-of-line constructor symbol in the golden binary:
//     the constructor was implicit / inlined at every construction
//     site, so none is declared here either.
// Return types that the mangled names do not encode are pinned by the
// golden bodies and by the Android subclass's JNI helpers:
// GetAnalyticsId returns std::string (sret in x8; the Android version
// builds one from the Java string), GetPlatformConsentState returns
// int (JNI ()I), StartReviewFlow returns bool (JNI ()Z),
// UnreadNewsCount returns int, the remaining value-returning stubs
// return bool (mov w0,#1 / mov w0,wzr bodies).
#ifndef CAVER_ONLINECONTROLLER_H
#define CAVER_ONLINECONTROLLER_H

#include <string>

#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

namespace Caver {

class Achievement;

// Target/action pair used for the Google-services sign-in handler.
// PLACEHOLDER layout: only the signature is needed by this slice (the
// base-class handler is an empty stub). The member shape follows the
// OnlineController_Android::AddGoogleServicesSignInHandler body
// (0x540A10), which copies a weak_ptr (refcount bump at counted+12)
// followed by a boost::function out of the incoming value; the full
// recovery of this template belongs to the slice that owns it.
template <typename Signature>
class TargetAction {
public:
    boost::weak_ptr<void> target;
    boost::function<Signature> action;
};

class OnlineController {
public:
    // Returns the shared controller as a raw pointer (the golden body
    // returns only the object pointer in x0, with no refcount bump;
    // ownership lives in a TU-local static shared_ptr in the .cpp).
    static OnlineController* SharedController();
    static void SetSharedController(const boost::shared_ptr<OnlineController>& controller);

    // Defaulted, not user-defined: the golden deleting destructor
    // (D0, 0x453864) is a bare tail-branch to operator delete, which
    // clang only emits for a defaulted destructor — a user-defined
    // empty body instead produced a 36-byte D0 that calls D1.
    virtual ~OnlineController() = default;

    virtual bool ShowOfflineAchievements();
    virtual bool AchievementsEnabled();
    virtual bool LeaderboardsEnabled();
    virtual void LaunchAchievements();
    virtual void LaunchLeaderboards();
    virtual void LaunchFacebook();
    virtual void LaunchTwitter();
    virtual bool IsKnownFreeVersion();
    virtual bool IsAgeKnown();
    virtual bool IsAgeOfConsent();
    virtual void EnteredAge(int age);
    virtual void OpenPrivacyPolicy();
    virtual bool ShouldShowInitialPrivacyConsentPopup();
    virtual bool HasPrivacyConsent();
    virtual void ReceivedPrivacyConsent(bool consent);
    virtual void StartAdsAndAnalytics();
    virtual std::string GetAnalyticsId();
    virtual void ShowAnalyticsIdPopup();
    virtual int GetPlatformConsentState();
    virtual void ShowPlatformConsentOptions();
    virtual void PrepareReviewFlow();
    virtual bool StartReviewFlow();
    virtual int UnreadNewsCount();
    virtual void LaunchNews();
    virtual void LaunchFeedbackView();
    virtual void ReportAchievementProgress(const boost::shared_ptr<Achievement>& achievement, double progress);
    virtual void ReportScore(const std::string& leaderboard, long long score, const std::string& category);
    virtual bool GoogleServicesAvailable();
    virtual bool SignedInToGoogleServices();
    virtual void AddGoogleServicesSignInHandler(TargetAction<void (bool, const std::string&)> handler);
    virtual void SignInToGoogleServices();
    virtual void ShowInterstitialAd(const std::string& name, int interval, float delay);
    virtual bool IsAdsEnabled();
    virtual void SetUseTestAds(bool useTestAds);
    virtual void AddProgressionEventSceneStarted(const std::string& sceneName);

    int m_age;  // +8 (entered age; 0 = unknown)
};

} // namespace Caver

#endif
