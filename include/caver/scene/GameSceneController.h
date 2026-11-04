// Recovered from libswordigo.so (1.4.13) evidence —
// Vtable _ZTVN5Caver19GameSceneControllerE @0x62A5D8,
// constructor @0x4253B4, destructor @0x439888.
//
// Layout (sizeof 392 = 0x188, proven from operator new(0x188) @0x42D998):
//   +0   vtable (IGameControlDelegate: GameControlButtonDown, GameControlButtonUp)
//   +8   boost::shared_ptr<GameState> m_gameState (16 bytes)
//   +24  LevelState* m_levelState (8 bytes)
//   +32  boost::shared_ptr<Scene> m_scene (16 bytes)
//   +48  GameOverlayView* m_overlayView (8 bytes)
//   +56  CameraController m_cameraController (160 bytes)
//   +216 SceneObject* m_heroObject (8 bytes)
//   +224 CharControllerComponent* m_charController (8 bytes)
//   +232 EntityComponent* m_heroEntity (8 bytes, raw — golden
//        CreateHeroObjectAt stores the template's HeroEntityComponent
//        here without a refcount bump)
//   +240 HealthComponent* m_heroHealth (8 bytes, raw — owned by the
//        hero object's components vector via AddComponent)
//   +248 ManaComponent* m_heroMana (8 bytes, raw — same ownership)
//   +256 HeroEquipmentManager m_equipmentManager (72 bytes)
//   +328 float m_facingLerp (4 bytes — golden Update lerps the stored
//        facing toward the hero entity's facing here)
//   +332 Vector3 m_heroSpawnPosition (12 bytes — golden SpawnHeroAt
//        writes the computed hero position here)
//   +344 boost::shared_ptr<void> m_castingSkill (16 bytes — the
//        in-progress cast's skill; pointee type UNRECOVERED, the
//        shared_ptr teardown in golden Update matches shared_ptr)
//   +360 float m_castTimer (4 bytes) + 4 bytes pad
//   +368 boost::intrusive_ptr<SceneObject> m_target (8 bytes — the
//        UpdateTarget target; golden UpdateTarget releases it on Hide)
//   +376 bool m_fallDamageApplied (1 byte — name inferred from use:
//        golden Update sets it when fall damage is applied once)
//   +380 float m_experienceTimer (4 bytes, default 0.2f)
//   +384 bool m_field384 (1 byte — gates fall-damage, experience and
//        health/mana sync in golden Update; exact meaning UNRECOVERED,
//        possibly cinematic mode)
//   +388 int m_field388 (4 bytes)

#ifndef CAVER_GAMESCENECONTROLLER_H
#define CAVER_GAMESCENECONTROLLER_H

#include <boost/shared_ptr.hpp>
#include <boost/intrusive_ptr.hpp>
#include <string>

#include "../core/Vector3.h"
#include "../gui/GameViewController.h"
#include "CameraController.h"
#include "../gui/HeroEquipmentManager.h"
#include "../systems/Item.h"

namespace Caver {

class Scene;
class GameState;
class LevelState;
class GameOverlayView;
class SceneObject;
class CharControllerComponent;
class EntityComponent;
class HealthComponent;
class ManaComponent;
class Item;
class Skill;
class GameEvent;

class GameSceneController : public IGameControlDelegate {
public:
    GameSceneController();
    ~GameSceneController();

    void GameControlButtonDown(GameControlButton button) override;
    void GameControlButtonUp(GameControlButton button) override;

    void InitWithScene(const boost::shared_ptr<Scene>& scene);
    // ------------------------------------------------------------------
    // Lua Character.* support (Job 3, 2026-10-27). Both are called ONLY
    // by the Character.RegisterTreasure / RegisterTreasureCollection
    // Lua functions (see GameViewController.cpp); they are not part of
    // the hero-creation work.
    //   RegisterTreasure: golden GameSceneController::RegisterTreasure
    //     @0x429168 — bumps m_field388 and stores it into
    //     m_levelState->m_numTreasures (LevelState +56).
    //     Merge: MAIN-SAFE (golden body, transcribed).
    //   RegisterTreasureCollection: golden
    //     GameSceneController::RegisterTreasureCollection @0x429180 —
    //     first-time-only (flag "chest_" + object identifier in the
    //     level state's properties): bumps m_visitedNodeCount
    //     (LevelState +60, the proto's treasures_found) and sends a
    //     "treasures" +1 counter event.
    //     Merge: MAIN-SAFE (golden body, transcribed).
    // ------------------------------------------------------------------
    void RegisterTreasure(const boost::intrusive_ptr<SceneObject>& object);
    void RegisterTreasureCollection(
        const boost::intrusive_ptr<SceneObject>& object);
    void SpawnHeroAt(const std::string& marker);
    void CreateHeroObjectAt(const Vector3& position, int facing,
                            bool addToScene);
    void AddHeroObjectToScene();
    void UpdateTarget();
    void Update(float dt);
    void HandleFall();
    void ConsumeItem(const boost::shared_ptr<Item>& item);
    bool HandleGameEvent(const GameEvent& event);
    void EquipItem(const boost::shared_ptr<Item>& item);
    // ------------------------------------------------------------------
    // UnequipArmor: golden @0x426F8C, 136 bytes — EquipArmor with an
    //   empty shared_ptr. Merge: MAIN-SAFE (golden body, transcribed).
    // ApplyTrinketToItemType: golden @0x427054, 44 bytes — forwards to
    //   HeroEquipmentManager::ApplyArmorTrinket / ApplyWeaponTrinket for
    //   ItemTypeArmor / ItemTypeWeapon, nothing for any other type.
    //   Merge: MAIN-SAFE (golden body, transcribed).
    // ManaCostForSkill: golden @0x428EC4, 388 bytes — the skill's mana
    //   cost (+72), zeroed when the skill is named "dimension" and the
    //   scene's globals table answers true for "$inAnotherDimension".
    //   Merge: MAIN-SAFE (golden body, transcribed).
    // ------------------------------------------------------------------
    void UnequipArmor();
    void ApplyTrinketToItemType(ItemType type,
                                const boost::shared_ptr<Item>& trinket);
    int ManaCostForSkill(const boost::shared_ptr<Skill>& skill);
    // BeginCasting: golden @0x4281FC (0xC90 bytes) — gates on
    //   CanBeginCasting and m_heroMana->m_mana > ManaCostForSkill, stores
    //   the skill in m_castingSkill and 0.1f into m_castTimer, builds the
    //   spell base "magic_<name>" for bolt/bomb/hookshot/dimension, takes
    //   the element suffix (_fire/_ice/_shadow) from the equipped skill
    //   trinket's name, looks up "magic_spell_cast"<+element> and
    //   spellBase<+element> in Scene::m_objectLibrary (searchImported),
    //   wires both onto the hero's SkillComponent, pushes character stats
    //   and the trinket into the spell object, starts
    //   CharControllerComponent::BeginCasting and finally deducts
    //   clamp(mana - cost, 0, maxMana).
    //   Merge: MAIN-SAFE (golden body, transcribed).
    void BeginCasting(const boost::shared_ptr<Skill>& skill);
    bool CanCastSkill(const boost::shared_ptr<Skill>& skill);
    void ApplyLevelUp(int v1, int v2, int v3);

    boost::shared_ptr<GameState> m_gameState;       // +8
    LevelState* m_levelState;                        // +24
    boost::shared_ptr<Scene> m_scene;               // +32
    GameOverlayView* m_overlayView;                  // +48
    CameraController m_cameraController;             // +56
    SceneObject* m_heroObject;                       // +216
    CharControllerComponent* m_charController;      // +224
    EntityComponent* m_heroEntity;                   // +232
    HealthComponent* m_heroHealth;                   // +240
    ManaComponent* m_heroMana;                       // +248
    HeroEquipmentManager m_equipmentManager;         // +256
    float m_facingLerp;                              // +328
    Vector3 m_heroSpawnPosition;                     // +332
    boost::shared_ptr<void> m_castingSkill;          // +344
    float m_castTimer;                               // +360
    char _pad364[4];                                 // +364
    boost::intrusive_ptr<SceneObject> m_target;      // +368
    bool m_fallDamageApplied;                        // +376
    char _pad377[3];                                 // +377
    float m_experienceTimer;                         // +380
    bool m_field384;                                 // +384
    char _pad385[3];                                 // +385
    int m_field388;                                  // +388
};

static_assert(sizeof(GameSceneController) == 392, "GameSceneController layout");

} // namespace Caver

#endif // CAVER_GAMESCENECONTROLLER_H
