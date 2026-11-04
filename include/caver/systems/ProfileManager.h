// Recovered from libswordigo.so (1.4.13) evidence — the base class
// declaration reconstructed by the jni-small SF-4 worker from
// ProfileManager_Android's vtable/RTTI and the base bodies in
// ~/workspace/analysis/swordigo/functions/Caver/ProfileManager/
// (the base class's own function queue belongs to another worker;
// only the declaration is pinned here).
//
// ProfileManager: the save-slot registry. It implements
// IPlayerProfileDelegate (PlayerProfile.h) — its vtable @0x62D8B0
// is exactly that interface's five slots, in order:
//   0 PlayerProfileWasSaved            (base @0x46A708)
//   1 PlayerProfileCounterValueDidChange (@0x46A8BC)
//   2 DownloadCloudProfiles            (base @0x46AA6C, empty)
//   3 GetAllProfiles                   (@0x469C3C)
//   4 DeleteProfileWithIdentifier      (base @0x46A570)
// There is NO virtual destructor slot; ~ProfileManager is a plain
// member called directly by derived destructors.
//
// Layout (sizeof 88, proven by sharedManager's operator new(0x58)
// and the ctor @0x469A98 store map):
//   +0  vptr (IPlayerProfileDelegate)
//   +8  std::set<IProfileManagerDelegate*> m_delegates
//   +32 boost::shared_ptr<GUIWindow> m_window (name UNRECOVERED)
//   +48 std::map<std::string, boost::shared_ptr<PlayerProfile>>
//       m_profiles
//   +72 bool m_field72 (UNRECOVERED; zeroed by the ctor)
//   +80 double m_field80 — initialised to -100.0 by the ctor:
//       the last-reported whole-minute timeplayed count, written
//       by PlayerProfileWasSaved (golden 0x46A708) when it reports
//       the "timeplayed" score.
#ifndef CAVER_PROFILEMANAGER_H
#define CAVER_PROFILEMANAGER_H

#include <map>
#include <set>
#include <string>
#include <vector>

#include <boost/shared_ptr.hpp>

#include "PlayerProfile.h"

namespace Caver {

class GUIWindow;
class ProfileManager;
class IProfileManagerDelegate;

// The manager-side delegate interface (single slot, proven by the
// delegate-notification loop in ProfileManager_Android::
// HandleDownloadedProfileData and by GameViewController.h's
// token-identical declaration).
#ifndef CAVER_IPROFILEMANAGERDELEGATE_DEFINED
#define CAVER_IPROFILEMANAGERDELEGATE_DEFINED
class IProfileManagerDelegate {
public:
    virtual void ProfileManagerDidDownloadProfile(
        ProfileManager* manager, const std::string& profileName) = 0;
};
#endif

class ProfileManager : public IPlayerProfileDelegate {
public:
    ProfileManager();
    ~ProfileManager();

    static ProfileManager* sharedManager();
    static void SetSharedManager(
        const boost::shared_ptr<ProfileManager>& manager);

    // IPlayerProfileDelegate (vtable slots 0..4, in order).
    void PlayerProfileWasSaved(PlayerProfile* profile, bool saved) override;
    void PlayerProfileCounterValueDidChange(
        PlayerProfile* profile, const std::string& counter, int oldValue,
        int newValue) override;
    void DownloadCloudProfiles(const std::string& identifier,
                               double delay) override;
    void GetAllProfiles(
        std::vector<boost::shared_ptr<PlayerProfile> >* profiles) override;
    void DeleteProfileWithIdentifier(const std::string& identifier) override;

    void AddProfile(const boost::shared_ptr<PlayerProfile>& profile);
    boost::shared_ptr<PlayerProfile> ProfileWithIdentifier(
        const std::string& identifier);
    void AddDelegate(IProfileManagerDelegate* delegate);
    void RemoveDelegate(IProfileManagerDelegate* delegate);

    std::set<IProfileManagerDelegate*> m_delegates;                 // +8
    boost::shared_ptr<GUIWindow> m_window;                          // +32
    std::map<std::string, boost::shared_ptr<PlayerProfile> >
        m_profiles;                                                 // +48
    bool m_field72;                                                 // +72 UNRECOVERED
    double m_field80;                                               // +80 last-reported timeplayed minutes
};

} // namespace Caver

#endif
