// Recovered from libswordigo.so (1.4.13) evidence.
// Layout established from Caver::Item::Item (0x477508),
// Caver::Item::LoadFromProtobufMessage (0x477530) and
// Caver::Item::SaveToProtobufMessage (0x4775AC), cross-checked against
// direct field reads in CharacterState::HighestLevelItemOfType
// (item+0 = type, item+108 = level) and the SwordigoDesktop ItemDef
// field order (binary wins: level sits at +108, before the damage pair).
//
// Object size is 120 bytes. There is NO out-of-line ~Item in the golden
// binary — the destructor was inlined at every destruction site.
#ifndef CAVER_ITEM_H
#define CAVER_ITEM_H

#include <string>

namespace Caver {

namespace Proto { class Item; }

// Item-space item type. LoadFromProtobufMessage maps the
// Caver::Proto::Item.Type enum (CONSUMABLE=1, WEAPON=2, ARMOR=3,
// TRINKET=4, QUEST=5) by subtracting 1, so the values here are the
// proto values minus one. CharacterState::HighestLevelItemOfType(1)
// is the equipped weapon and (2) the equipped armor, matching
// Weapon=1 / Armor=2 below.
enum ItemType {
    ItemTypeConsumable = 0,
    ItemTypeWeapon = 1,
    ItemTypeArmor = 2,
    ItemTypeTrinket = 3,
    ItemTypeQuest = 4
};

class Item {
public:
    Item();

    void LoadFromProtobufMessage(const Proto::Item& message);
    void SaveToProtobufMessage(Proto::Item* message) const;

    ItemType m_type;                // +0
    std::string m_name;             // +8
    std::string m_title;            // +32
    std::string m_shortDescription; // +56
    std::string m_description;      // +80
    bool m_unique;                  // +104
    int m_level;                    // +108
    int m_minDamage;                // +112
    int m_maxDamage;                // +116
};

} // namespace Caver

#endif
