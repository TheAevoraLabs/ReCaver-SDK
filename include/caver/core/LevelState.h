// Recovered from libswordigo.so (1.4.13) evidence.
// Per-level progress record owned by GameState. Layout proven
// (StateForLevelWithName allocates 0x40 bytes; ctor 0x47772C;
// Load/Save 0x477780/0x477804):
//   +0  std::string m_name (the level's name; also the map key)
//   +24 bool m_visited
//   +32 StateProperties m_properties (24 bytes; GuideTargetCompleted
//       calls StateProperties::HasFlag on this member)
//   +56 int m_numTreasures (ctor -1)
//   +60 int m_visitedNodeCount (ctor 0; serialised as the proto
//       field treasures_found)
#ifndef CAVER_LEVELSTATE_H
#define CAVER_LEVELSTATE_H

#include <set>
#include <string>

namespace Caver {

namespace Proto {
class LevelState;
class StateProperties;
}

// StateProperties (24 bytes): a single std::set<std::string> of
// flags — the ctor (0x47E898) builds the tree header, Load
// (0x47E8AC) inserts each repeated "flag" string, HasFlag (0x47E9EC)
// is a set find, AddFlag (0x47E908) a tail-call insert, and the const
// Save (0x47E910) walks the set in order appending each flag.
// Definitions in StateProperties.cpp.
class StateProperties {
public:
    StateProperties();

    bool HasFlag(const std::string& name);
    void AddFlag(const std::string& name);
    void LoadFromProtobufMessage(const Proto::StateProperties& message);
    void SaveToProtobufMessage(Proto::StateProperties* message) const;
private:
    std::set<std::string> m_flags;   // +0
};

class LevelState {
public:
    LevelState();

    void LoadFromProtobufMessage(const Proto::LevelState& message);
    void SaveToProtobufMessage(Proto::LevelState* message) const;

    std::string m_name;                 // +0
    bool m_visited = false;             // +24
private:
    char m_pad25[7];                    // +25
public:
    StateProperties m_properties;       // +32
    int m_numTreasures = -1;            // +56
    int m_visitedNodeCount = 0;         // +60 (proto: treasures_found)
};

} // namespace Caver

#endif
