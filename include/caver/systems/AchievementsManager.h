// Recovered from libswordigo.so (1.4.13) evidence — IDA/Hex-Rays exports in
// reference/ida-arm64/functions/Caver/AchievementsManager/ (11 real bodies;
// the 0x60xxxx files are PLT thunks that pin the mangled signatures).
//
// AchievementsManager: the singleton that owns the 28 built-in achievements,
// turns player-profile counters into unlock progress, forwards progress and
// score to OnlineController, and queues the unlock toasts that
// AchievementNotificationView presents.
//
// Layout (sizeof 0x128) proven by the ctor's container initialisation, by
// SetCounterValue's deque push at +144 (__first_/__begin_/__end_/__end_cap_/
// __start_/__size_ = +144/+152/+160/+168/+176/+184) and by Update's
// notification accesses at +192/+208:
//   +0    std::vector<boost::shared_ptr<Achievement> >
//              every achievement in registration order (the ctor pushes one
//              shared_ptr per table entry here)
//   +24   std::map<std::string, boost::shared_ptr<Achievement> >
//              keyed by Achievement::identifier
//   +48   std::vector<std::string>
//              the distinct counter keys registered by the ctor
//   +72   std::multimap<std::string, boost::shared_ptr<Achievement> >
//              keyed by counter name (SetCounterValue's equal_range)
//   +96   std::set<std::string>
//              identifiers already reported or unlocked
//   +120  int
//              accumulated points of every reported achievement
//   +128  GUIView*
//              the view the notifications are presented in (set outside this
//              class; Update only reads it)
//   +136  void*
//              only ever zeroed by the ctor — no reader in any recovered body
//   +144  std::deque<boost::shared_ptr<Achievement> >
//              unlock notifications waiting for the toast timer
//   +192  boost::shared_ptr<AchievementNotificationView>
//              the toast currently on screen
//   +208  float
//              seconds left before the next toast may appear; SetCounterValue
//              sets it to 3.0f when it queues while nothing is on screen
//   +216  long long
//              last score handed to OnlineController::ReportScore
//   +224  std::map<std::string, double>
//              last progress reported per achievement identifier
//   +248  std::map<std::string, double>
//              progress held back until the next report pass
//   +272  std::map<std::string, int>
//              every counter value seen by SetCounterValue
//
// Member names are UNRECOVERED; the names below describe the proven role.
#ifndef CAVER_ACHIEVEMENTS_MANAGER_H
#define CAVER_ACHIEVEMENTS_MANAGER_H

#include <deque>
#include <map>
#include <set>
#include <string>
#include <vector>

#include <boost/shared_ptr.hpp>

#include "Achievement.h"

namespace Caver {

class AchievementNotificationView;
class GUIView;
class PlayerProfile;

class AchievementsManager {
public:
    static AchievementsManager* sharedManager();

    AchievementsManager();
    ~AchievementsManager();

    // _ZN5Caver19AchievementsManager6UpdateEf @0x44B1E4
    void Update(float deltaTime);

    // _ZN5Caver19AchievementsManager24AchievementForIdentifierERKNSt6__ndk112basic_stringIcNS1_11char_traitsIcEENS1_9allocatorIcEEEE @0x44B51C
    boost::shared_ptr<Achievement> AchievementForIdentifier(const std::string& identifier);

    // _ZN5Caver19AchievementsManager23SetAchievementCompletedERKN5boost10shared_ptrINS_11AchievementEEEb @0x44B594
    void SetAchievementCompleted(const boost::shared_ptr<Achievement>& achievement,
                                 bool completed);

    // _ZN5Caver19AchievementsManager11ReportScoreERKNSt6__ndk112basic_stringIcNS1_11char_traitsIcEENS1_9allocatorIcEEEElS9_ @0x44B800
    void ReportScore(const std::string& category, long long score,
                     const std::string& title);

    // _ZN5Caver19AchievementsManager29ClearReportedAchievementsCacheEv @0x44B844
    void ClearReportedAchievementsCache();

    // _ZN5Caver19AchievementsManager21ReportPercentCompletedERKN5boost10shared_ptrINS_11AchievementEEEd @0x44B874
    void ReportPercentCompleted(const boost::shared_ptr<Achievement>& achievement,
                                double percent);

    // _ZN5Caver19AchievementsManager24ReportPendingAchievementsEv @0x44BB70
    void ReportPendingAchievements();

    // _ZN5Caver19AchievementsManager15SetCounterValueERKNSt6__ndk112basic_stringIcNS1_11char_traitsIcEENS1_9allocatorIcEEEEii @0x44BDEC
    // Returns true when the call queued at least one unlock notification.
    bool SetCounterValue(const std::string& counter, int oldValue, int newValue);

    // _ZN5Caver19AchievementsManager30ReportAllAchievementsForProfileERKN5boost10shared_ptrINS_12PlayerProfileEEE @0x44C060
    void ReportAllAchievementsForProfile(const boost::shared_ptr<PlayerProfile>& profile);

    // _ZN5Caver19AchievementsManager21ReportAllAchievementsEv @0x44C0EC
    void ReportAllAchievements();

    std::vector<boost::shared_ptr<Achievement> > m_allAchievements;                    // +0
    std::map<std::string, boost::shared_ptr<Achievement> > m_achievements;             // +24
    std::vector<std::string> m_counters;                                              // +48
    std::multimap<std::string, boost::shared_ptr<Achievement> > m_achievementsByCounter; // +72
    std::set<std::string> m_reportedAchievementIdentifiers;                            // +96
    int m_points;                                                                     // +120
    GUIView* m_view;                                                                  // +128
    void* m_field136;                                                                 // +136
    std::deque<boost::shared_ptr<Achievement> > m_pendingNotifications;                // +144
    boost::shared_ptr<AchievementNotificationView> m_notification;                     // +192
    float m_notificationDelay;                                                        // +208
    long long m_lastReportedPoints;                                                   // +216
    std::map<std::string, double> m_reportedProgress;                                  // +224
    std::map<std::string, double> m_pendingProgress;                                   // +248
    std::map<std::string, int> m_counterValues;                                        // +272
};

// Golden aarch64 NDK-libc++ layout anchor; MSVC STL member sizes differ,
// so this assert is Android/Linux-only (the Windows build is self-consistent
// under MSVC STL and does not claim golden byte-layout parity).
#ifndef _MSC_VER
static_assert(sizeof(AchievementsManager) == 0x128,
              "AchievementsManager size must be 0x128");
#endif

} // namespace Caver

#endif // CAVER_ACHIEVEMENTS_MANAGER_H
