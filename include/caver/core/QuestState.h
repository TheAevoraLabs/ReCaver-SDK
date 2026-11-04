// Recovered from libswordigo.so (1.4.13) evidence.
// Per-quest progress record owned by GameState. Layout proven so far
// (AddStateForQuestWithName allocates 0x20 bytes; GameState's quest
// filters and PercentCompleted read these fields):
//   +0  std::string m_name (the quest's name; also the map key)
//   +24 bool m_completed (summed as 0/1 by GameState::PercentCompleted
//       and tested by both GetAll* quest filters in the binary —
//       including GetAllQuestsInProgress, which checks this same flag)
// Remaining bytes (+25..+32) are not yet recovered.
#ifndef CAVER_QUESTSTATE_H
#define CAVER_QUESTSTATE_H

#include <string>

namespace Caver {

namespace Proto { class QuestState; }

class QuestState {
public:
    QuestState();

    void LoadFromProtobufMessage(const Proto::QuestState& message);
    void SaveToProtobufMessage(Proto::QuestState* message) const;

    std::string m_name; // +0
    bool m_completed;   // +24
private:
    char m_rest[7];     // +25..+32 (unrecovered)
};

} // namespace Caver

#endif
