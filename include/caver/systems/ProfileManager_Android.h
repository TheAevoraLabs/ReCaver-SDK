// Recovered from libswordigo.so (1.4.13) evidence — IDA/Hex-Rays
// exports in ~/workspace/analysis/swordigo/functions/Caver/
// ProfileManager_Android/ and the derived vtable @0x636830
// (ABS64 relocations): it is the ProfileManager interface vtable
// with slots 0 (PlayerProfileWasSaved), 2 (DownloadCloudProfiles)
// and 4 (DeleteProfileWithIdentifier) overridden by the Android
// class; slots 1 and 3 keep the base functions. RTTI @0x636878 is
// an __si_class_type_info on ProfileManager.
//
// ProfileManager_Android: the Google-Play-Games-backed profile
// manager. Cloud operations go through the JNI free functions
// Caver::AndroidLoadSnapshot / AndroidSaveSnapshot /
// AndroidDeleteSnapshot (platform/JNI slice).
//
// Layout: ProfileManager (88 bytes), then
//   +88  bool m_field88 (UNRECOVERED; zeroed by the ctor)
//   +96  boost::shared_ptr<ProfileManagerLoadingViewController>
//        m_loadingViewController — the modal "loading" controller
//        shown while cloud profiles download
//   +112 int m_field112 (UNRECOVERED; zeroed by the ctor)
// sizeof 120.
#ifndef CAVER_PROFILEMANAGER_ANDROID_H
#define CAVER_PROFILEMANAGER_ANDROID_H

#include <boost/shared_ptr.hpp>
#include <string>

#include "ProfileManager.h"
#include "ProfileStats.h"

namespace Caver {

class GUIEvent;
class ProfileManagerLoadingViewController;

class ProfileManager_Android : public ProfileManager {
public:
    ProfileManager_Android();
    ~ProfileManager_Android();

    // IPlayerProfileDelegate overrides (slots 0, 2, 4).
    void PlayerProfileWasSaved(PlayerProfile* profile, bool saved) override;
    void DownloadCloudProfiles(const std::string& identifier,
                               double delay) override;
    void DeleteProfileWithIdentifier(const std::string& identifier) override;

    void SetPreviousCloudSaveTimeToCurrentTime();
    void SaveSnapshot(PlayerProfile* profile, const std::string& data);
    void HandleSnapshotLoaded(const std::string& identifier,
                              const std::string& data);
    boost::shared_ptr<PlayerProfile> HandleDownloadedProfileData(
        const std::string& data);
    bool NewProfileIsBetter(const ProfileStats& newStats,
                            const ProfileStats& existingStats);
    void ShowCloudLoadingView();
    void SkipLoadingButtonPressed(void* sender, GUIEvent* event);
    void HideCloudLoadingView();

    bool m_field88;  // +88 UNRECOVERED
    boost::shared_ptr<ProfileManagerLoadingViewController>
        m_loadingViewController;  // +96
    int m_field112;               // +112 UNRECOVERED
};

} // namespace Caver

#endif
