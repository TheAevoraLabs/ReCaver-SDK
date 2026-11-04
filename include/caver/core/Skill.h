// Caver::Skill — plain data class (no vtable in the golden), proven by
// Skill::LoadFromProtobufMessage @0x47E73C / SaveToProtobufMessage
// @0x47E78C: three strings, a double at +72, an int at +80.
//
// Extracted from gui/SkillPickerView.h, which carried the only complete
// declaration in the tree while core/GameData.h, core/GameState.h and
// gui/MenuItemSlot.h all forward-declare it. InventoryItemPanel reads
// Skill::title (+24) and Skill::description (+48) directly.
#ifndef CAVER_SKILL_H
#define CAVER_SKILL_H

#include <string>

namespace Caver {

namespace Proto { class Skill; }

class Skill {
public:
    // Golden Load @0x47E73C / Save @0x47E78C (see core/GameData.cpp):
    // strings at +0/+24/+48, manaCost/minDamage as the 8-byte store at
    // +72, max_damage at +80. Definitions live in core/GameData.cpp,
    // which owns the registries these serialise.
    void LoadFromProtobufMessage(const Proto::Skill& message);
    void SaveToProtobufMessage(Proto::Skill* message) const;

    std::string name;         // +0
    std::string title;        // +24
    std::string description;  // +48
    int manaCost;             // +72 — dword read by
                               //      GameSceneController::ManaCostForSkill
                               //      (0x428EC4) and written dword-wise by
                               //      SaveToProtobufMessage; the header used
                               //      to call this a double because Load's
                               //      8-byte store at +72 covers this and
                               //      minDamage
    int minDamage;            // +76 (covered by Load's 8-byte store at +72)
    int field80;              // +80 — proto max_damage (name kept: the
                               //      +80 dword in Save is this+20, and
                               //      Load stores proto max_damage here)
};

// Golden GameData::Load allocates a Skill with operator new(0x58);
// three 24-byte strings + three ints = 84 bytes, tail-padded to the
// 8-byte string alignment = 88.
// Golden aarch64 NDK-libc++ layout anchor; MSVC STL member sizes differ,
// so this assert is Android/Linux-only (the Windows build is self-consistent
// under MSVC STL and does not claim golden byte-layout parity).
#ifndef _MSC_VER
static_assert(sizeof(Skill) == 0x58, "Skill sizeof must be 0x58");
#endif

} // namespace Caver

#endif // CAVER_SKILL_H
