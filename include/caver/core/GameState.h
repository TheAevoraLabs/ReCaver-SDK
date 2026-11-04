// Recovered from libswordigo.so (1.4.13) evidence.
// GameState is one save slot's worth of progress: the character, the
// per-level and per-quest state records, and the guide/visited data.
// Layout from the constructor (sizeof at least 488):
//   +0   boost::shared_ptr<GameData> m_gameData
//   +16  CharacterState m_characterState (160 bytes; its own first
//        member is a shared_ptr<GameData>, assigned in GameState's
//        ctor body — proven by the ctor instruction stream)
//   +176 std::map<std::string, boost::shared_ptr<LevelState> >
//        m_levelStates
//   +200 std::map<std::string, boost::shared_ptr<QuestState> >
//        m_questStates
//   +224 StateProperties m_stateProperties (24 bytes)
//   +248 std::string m_currentLevelName (read by CurrentLevelState)
//   +272 std::string (name unrecovered)
//   +296 std::string (name unrecovered)
//   +320 boost::shared_ptr<void> (pointee type unrecovered)
//   +336 std::string (name unrecovered)
//   +360 std::string (name unrecovered)
//   +384 std::string (name unrecovered)
//   +408 bool pair (names unrecovered; zeroed by the ctor)
//   +416 std::string (name unrecovered)
//   +440 std::string (name unrecovered)
//   +464 std::vector<boost::shared_ptr<QuestText> > m_questTexts
//   +488 four bools (guide_enabled, guide_toggled,
//        coin_doubler_enabled, coin_doubler_toggled — copied as one
//        dword by Load; sizeof 496)
// String fields named from Load/Save evidence (2026-10-23):
//   +272 current_spawn_point, +296 previous_portal_level,
//   +336 carried_object_template, +360 carried_object_identifier,
//   +384 selected_menu_tab, +416 flashing_item_name,
//   +440 flashing_skill_name; +408/+409 = menu_button_flashing /
//   skill_toggle_button_flashing. The +320 shared_ptr is the current
//   map node (Map::NodeForName over the GameData +48 holder, which is
//   the world Map).
#ifndef CAVER_GAMESTATE_H
#define CAVER_GAMESTATE_H

#include <map>
#include <string>
#include <vector>
#include <boost/shared_ptr.hpp>

#include "GameData.h"
#include "LevelState.h"
#include "QuestState.h"
#include "../systems/Item.h"

namespace Caver {

class GuideTarget;
class MapNode;
class QuestText;

namespace Proto {
class CharacterState;
class GameState;
class StateProperties;
}

// CharacterState (160 bytes): full layout settled 2026-10-23 from the
// ctor (0x46D5B8), dtor (0x47418C), Load (0x46D600) and Save
// (0x46E044): a GameData back-pointer, the owned-item map, the
// learned-skill list, the current skill, three trinket slots and the
// RPG scalar block (defaults 6 health / 30 mana / attributes 1 — the
// engine's max-health 2*hl+4 and max-mana 20*el+10 at level 1). The
// proto's equipped_weapon / equipped_armor fields have NO members
// here: neither Load nor Save touches them in this build.

// Packed (min, max) damage pair returned in one register pair by the
// CharacterState damage queries (name reconstructed; the return type
// is not part of the mangled names).
struct DamageRange {
    int m_min;
    int m_max;
};

class CharacterState {
public:
    // Owned-item record: the canonical Item plus a stack count.
    struct ItemState {
        boost::shared_ptr<Item> m_item;   // +0
        int m_count;                      // +16
    };

    CharacterState();
    ~CharacterState();

    bool HasSkillWithName(const std::string& name);
    void LoadFromProtobufMessage(const Proto::CharacterState& message);
    void SaveToProtobufMessage(Proto::CharacterState* message) const;

    bool AddItem(const boost::shared_ptr<Item>& item);
    void RemoveItem(Item* item);
    bool HasItemWithName(const std::string& name);
    int ItemCount(const boost::shared_ptr<Item>& item);
    int ItemCount(const std::string& name);
    void GetItemsOfType(ItemType type, std::vector<boost::shared_ptr<Item> >* items);
    boost::shared_ptr<Item> AnyItemOfType(ItemType type);
    boost::shared_ptr<Item> HighestLevelItemOfType(ItemType type);
    boost::shared_ptr<Item> HighestLevelItemOfType(int type);

    bool AddSkill(const boost::shared_ptr<Skill>& skill);

    int ExperiencePointsRequiredForLevel(int level);
    int BaseExperienceForKillingMonster(int unused, int level, int multiplier);
    int ExperiencePointsForKillingMonster(int monsterLevel, int basePoints);

    float TrinketDamageMultiplier(const boost::shared_ptr<Item>& item);
    DamageRange WeaponDamageRange();
    DamageRange WeaponDamageRangeWithItem(const boost::shared_ptr<Item>& item, const boost::shared_ptr<Item>& trinket);
    float ArmorDamageMultiplier();
    float ArmorDamageMultiplierWithItem(const boost::shared_ptr<Item>& item, const boost::shared_ptr<Item>& trinket);
    DamageRange SkillDamageRange();
    DamageRange SkillDamageRangeWithSkill(const boost::shared_ptr<Skill>& skill, const boost::shared_ptr<Item>& trinket);
    std::string TrinketBonusForWeapons(const boost::shared_ptr<Item>& item);
    std::string TrinketBonusForArmors(const boost::shared_ptr<Item>& item);
    std::string TrinketBonusForSkills(const boost::shared_ptr<Item>& item);

    boost::shared_ptr<GameData> m_gameData;                  // +0
    std::map<std::string, ItemState> m_itemStates;           // +16
    std::vector<boost::shared_ptr<Skill> > m_skills;         // +40
    boost::shared_ptr<Skill> m_currentSkill;                 // +64
    boost::shared_ptr<Item> m_weaponTrinket;                 // +80
    boost::shared_ptr<Item> m_armorTrinket;                  // +96
    boost::shared_ptr<Item> m_skillTrinket;                  // +112
    int m_currentHealth = 6;                                 // +128
    int m_currentMana = 30;                                  // +132
    int m_currentCoins = 0;                                  // +136
    int m_experiencePoints = 0;                              // +140
    int m_experienceLevel = 0;                               // +144
    int m_healthAttribute = 1;                               // +148
    int m_attackAttribute = 1;                               // +152
    int m_magicAttribute = 1;                                // +156
};

// StateProperties is declared in LevelState.h (it is embedded in
// LevelState at +32 as well as here at +224).

class GameState {
public:
    GameState(const boost::shared_ptr<GameData>& gameData);
    ~GameState();

    void LoadFromProtobufMessage(const Proto::GameState& message);
    void SaveToProtobufMessage(Proto::GameState* message) const;

    LevelState* CurrentLevelState(bool createIfMissing);
    LevelState* StateForLevelWithName(const std::string& name, bool createIfMissing);
    QuestState* StateForQuestWithName(const std::string& name);
    QuestState* AddStateForQuestWithName(const std::string& name);
    // The QuestText class is defined only in GameState.cpp (its own
    // slice), so the Character.AddQuestText Lua function cannot
    // construct one directly; this helper does it here. Behaviour
    // follows the golden Character.AddQuestText body @0x436300: a
    // QuestText whose name is arg1 and whose lines are the arg2
    // table's strings is appended to m_questTexts (GameState +464).
    void AddQuestText(const std::string& questName,
                      const std::vector<std::string>& lines);

    void GetAllCompletedQuests(std::vector<QuestState*>* quests);
    void GetAllQuestsInProgress(std::vector<QuestState*>* quests);

    bool AllNodesVisited();
    float PercentCompleted();

    boost::shared_ptr<GuideTarget> CurrentGuideTarget();
    bool GuideTargetCompleted(const boost::shared_ptr<GuideTarget>& target);

    CharacterState* characterState() { return &m_characterState; }
    const CharacterState* characterState() const { return &m_characterState; }

    // Typed views of the void-erased holders, for includers migrating
    // off the MapView.h stubs: +320 is the current MapNode (its +16
    // name is read by SaveToProtobufMessage) and +488 is the guide
    // flag written by Load. The MapNode cast lives in GameState.cpp,
    // not scattered at use sites, so each use keeps a single
    // shared_ptr<MapNode> value with identical ownership semantics.
    boost::shared_ptr<MapNode> currentNode() const;
    bool guideEnabled() const { return m_guideEnabled; }

    boost::shared_ptr<GameData> m_gameData;                                     // +0
    CharacterState m_characterState;                                            // +16
    std::map<std::string, boost::shared_ptr<LevelState> > m_levelStates;        // +176
    std::map<std::string, boost::shared_ptr<QuestState> > m_questStates;        // +200
    StateProperties m_stateProperties;                                          // +224
    std::string m_currentLevelName;                                             // +248
    std::string m_string272;                                                    // +272
    std::string m_string296;                                                    // +296
    boost::shared_ptr<void> m_shared320;                                        // +320
    std::string m_string336;                                                    // +336
    std::string m_string360;                                                    // +360
    std::string m_string384;                                                    // +384
    bool m_bool408 = false;                                                     // +408
    bool m_bool409 = false;                                                     // +409
    std::string m_string416;                                                    // +416
    std::string m_string440;                                                    // +440
    std::vector<boost::shared_ptr<QuestText> > m_questTexts;                    // +464
    bool m_guideEnabled = false;                                                // +488
    bool m_guideToggled = false;                                                // +489
    bool m_coinDoublerEnabled = false;                                          // +490
    bool m_coinDoublerToggled = false;                                          // +491
};

} // namespace Caver

#endif
