// Recovered from libswordigo.so (1.4.13) evidence.
// GameData is the game's static content database: every item, skill,
// quest and entity class, loaded once (from protobuf) and looked up by
// name while scenes run. Container layout verified instruction-level
// from ~GameData and the Add*/ForName functions:
//   +0   std::map<std::string, boost::shared_ptr<Item> > m_itemsByName
//   +24  std::vector<boost::shared_ptr<Skill> > m_skills
//   +48  boost::shared_ptr<void> (member at +48; pointee type not yet
//        recovered — Load/Save evidence pending, see board)
//   +64  std::map<std::string, boost::shared_ptr<Quest> > m_questsByName
//   +88  std::vector<boost::shared_ptr<Quest> > m_quests
//   +112 std::map<std::string, boost::shared_ptr<EntityClass> >
//        m_entityClassesByName
//   +136 std::vector<boost::shared_ptr<GuideTarget> > m_guideTargets
//        (corrected 2026-10-23 from LoadFromProtobufMessage evidence:
//        entity classes live ONLY in the +112 map; the +136 vector is
//        filled with GuideTarget objects by the guide_target loop)
//   +160 ... scalar fields (recovered with Load/Save — TODO)
// There is no out-of-line constructor in the binary; the members
// default-construct. Quirks kept faithful: ItemForName is a map
// operator[] (it INSERTS a null entry for a missing name, and returns
// empty for an empty name), while QuestForName/EntityClassForName are
// plain finds and SkillForName is a linear scan of the vector.
#ifndef CAVER_GAMEDATA_H
#define CAVER_GAMEDATA_H

#include <map>
#include <string>
#include <vector>
#include <boost/shared_ptr.hpp>

#include "caver/core/EntityClass.h"
#include "caver/systems/Item.h"

namespace Caver {

class GuideTarget;
class Map;
class Quest;
class Skill;

namespace Proto { class GameData; }

class GameData {
public:
    friend class GameState;

    ~GameData();

    void LoadFromProtobufMessage(const Proto::GameData& message);
    void SaveToProtobufMessage(Proto::GameData* message) const;

    void AddItem(const boost::shared_ptr<Item>& item);
    void AddSkill(const boost::shared_ptr<Skill>& skill);
    void AddQuest(const boost::shared_ptr<Quest>& quest);
    void AddEntityClass(const boost::shared_ptr<EntityClass>& entityClass);

    boost::shared_ptr<Item> ItemForName(const std::string& name);
    boost::shared_ptr<Skill> SkillForName(const std::string& name);
    boost::shared_ptr<Quest> QuestForName(const std::string& name);
    boost::shared_ptr<EntityClass> EntityClassForName(const std::string& name);

    // Typed view of the +48 holder: the pointee is the world Map
    // (proven by GameState::Load, which calls NodeForName on it).
    // The cast lives in GameData.cpp, not scattered at use sites,
    // so includers migrating off the MapView.h stubs keep a single
    // shared_ptr<Map> value with identical ownership semantics.
    boost::shared_ptr<Map> map() const;

    float EntityHealthMultiplierAtLevel(int level);
    float EntityDamageMultiplierAtLevel(int level);

private:
    std::map<std::string, boost::shared_ptr<Item> > m_itemsByName;                    // +0
    std::vector<boost::shared_ptr<Skill> > m_skills;                                  // +24
    boost::shared_ptr<void> m_unknown48;                                              // +48
    std::map<std::string, boost::shared_ptr<Quest> > m_questsByName;                  // +64
    std::vector<boost::shared_ptr<Quest> > m_quests;                                  // +88
    std::map<std::string, boost::shared_ptr<EntityClass> > m_entityClassesByName;     // +112
    std::vector<boost::shared_ptr<GuideTarget> > m_guideTargets;                      // +136
};

} // namespace Caver

#endif
