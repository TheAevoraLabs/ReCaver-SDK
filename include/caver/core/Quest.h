// Recovered from libswordigo.so (1.4.13) evidence.
// Caver::Quest is the engine-side quest record, converted to/from
// Caver::Proto::Quest by GameData::Load/SaveToProtobufMessage.
//
// Layout proven from the golden functions:
//   Quest::Quest @0x47E36C: int at +0 is -1, the four strings
//     zero-initialised (IDA: *(_DWORD *)this = -1, then 16-byte
//     zero stores covering +8..+103).
//   Quest::LoadFromProtobufMessage @0x47E394: strings assigned at
//     +8/+32/+56/+80 from the proto fields (name/title/
//     follow_up_quest/map_location, Proto::Quest fields 1-4).
//   Quest::SaveToProtobufMessage @0x47E3E0: all four strings set
//     unconditionally (generated set_ allocates from the proto
//     default on first use; the has-bits 1/2/4/8 are set).
// sizeof = 104 (0x68). The +0 int's name is UNRECOVERED (the
// binary gives no member name); only its value (-1) is proven.
// This header unifies the two TU-local Quest declarations it
// replaces (core/GameData.cpp and gui/QuestsView.cpp, which both
// carried exactly this layout) and the layout-only declaration in
// systems/MapView.h (same offsets, same sizeof); QuestsView reads
// only the +32 title string.
#ifndef CAVER_QUEST_H
#define CAVER_QUEST_H

#include <string>

namespace Caver {

namespace Proto { class Quest; }

class Quest {
public:
    Quest();
    void LoadFromProtobufMessage(const Proto::Quest& message);
    void SaveToProtobufMessage(Proto::Quest* message) const;

    int m_field0;                 // +0 (golden ctor: -1; name UNRECOVERED)
    char _pad4[4];                // +4
    std::string m_name;           // +8
    std::string m_title;          // +32 (read by QuestsView)
    std::string m_followUpQuest;  // +56
    std::string m_mapLocation;    // +80
};

// Golden aarch64 NDK-libc++ layout anchor; MSVC STL member sizes differ,
// so this assert is Android/Linux-only (the Windows build is self-consistent
// under MSVC STL and does not claim golden byte-layout parity).
#ifndef _MSC_VER
static_assert(sizeof(Quest) == 104, "Quest must stay 104 bytes");
#endif

} // namespace Caver

#endif
