// Recovered from libswordigo.so (1.4.13) evidence — layout pinned by
// the ctor @0x541F38 (jni-small SF-1):
//   +0  bool m_field0 — set true when constructed from a non-null
//       profile (UNRECOVERED name; a "has profile" flag)
//   +4  float m_percentCompleted — the profile's cached percent,
//       refreshed via GameState::PercentCompleted() when the
//       profile has a game state (which also writes the fresh value
//       back into the profile at PlayerProfile+108)
//   +8  double m_timePlayed (PlayerProfile+112)
//   +16 std::string m_name (PlayerProfile+48)
//   +36 int m_field36 — zeroed by the ctor, never written after
//       (UNRECOVERED)
// sizeof 40.
#ifndef CAVER_PROFILESTATS_H
#define CAVER_PROFILESTATS_H

#include <string>

namespace Caver {

class PlayerProfile;

class ProfileStats {
public:
    ProfileStats(PlayerProfile* profile);

    bool m_field0;              // +0 UNRECOVERED
    float m_percentCompleted;   // +4
    double m_timePlayed;        // +8
    std::string m_name;         // +16
    int m_field36;              // +36 UNRECOVERED
};

} // namespace Caver

#endif
