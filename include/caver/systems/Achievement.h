// Recovered from libswordigo.so (1.4.13) evidence — IDA/Hex-Rays exports
// in reference/ida-arm64/functions/Caver/Achievement/:
//   Achievement(std::string const&, std::string const&, std::string const&,
//               int, std::string const&, int)  @0x44AC08
//   ~Achievement()                             @0x44AD70
//   Achievement(Achievement const&)            @0x44ADEC
//
// Layout (sizeof 0x70), proven by the ctor's store order and by the dtor's
// teardown order (it releases the +80 string first, then +48, +24, +0):
//   +0    std::string identifier    ("scavenger", "beammeupscotty", ...)
//   +24   std::string title         (shown by AchievementNotificationView's
//                                    upper label)
//   +48   std::string description   (shown by the lower, wrapped label)
//   +72   int points                (added to AchievementsManager's score)
//   +80   std::string counter       (player-profile counter name; empty when
//                                    the achievement is not counter-driven)
//   +104  int threshold             (counter value that unlocks it)
//
// The identifier doubles as the icon base name: AchievementNotificationView
// looks up the texture "achicon_" + identifier.
#ifndef CAVER_ACHIEVEMENT_H
#define CAVER_ACHIEVEMENT_H

#include <string>

namespace Caver {

class Achievement {
public:
    Achievement(const std::string& identifier, const std::string& title,
                const std::string& description, int points,
                const std::string& counter, int threshold);
    ~Achievement();
    Achievement(const Achievement& other);

    std::string identifier;   // +0
    std::string title;        // +24
    std::string description;  // +48
    int points;               // +72
    std::string counter;      // +80
    int threshold;            // +104
};

// Golden aarch64 NDK-libc++ layout anchor; MSVC STL member sizes differ,
// so this assert is Android/Linux-only (the Windows build is self-consistent
// under MSVC STL and does not claim golden byte-layout parity).
#ifndef _MSC_VER
static_assert(sizeof(Achievement) == 0x70, "Achievement size must be 0x70");
#endif

} // namespace Caver

#endif // CAVER_ACHIEVEMENT_H
