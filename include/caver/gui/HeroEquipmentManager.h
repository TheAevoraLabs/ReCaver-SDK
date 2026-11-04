// Recovered from libswordigo.so (1.4.13) evidence — IDA exports in
// reference/ida-arm64/functions/Caver/HeroEquipmentManager/ (14 own
// bodies, 0x439AB4..0x43EB00).
//
// HeroEquipmentManager turns the character's owned items into the live
// scene: it rebuilds the hero's weapon objects from the
// SwingableWeapon components, spawns the two orbiting armour wisps,
// pushes the character's damage ranges into the Damage components and
// applies the fire / ice / shadow trinket effects to the weapon, the
// armour and the spells.
//
// Layout (sizeof 72 = 0x48, proven by the constructor @0x43D5B4, which
// zero-fills 72 bytes, and by every body's member accesses):
//   +0   boost::shared_ptr<GameState> m_gameState
//   +16  boost::intrusive_ptr<SceneObject> m_heroObject
//   +24  std::vector<boost::intrusive_ptr<SceneObject>> m_weapons
//        the weapon objects EquipWeapon created from the hero's
//        SwingableWeapon components
//   +48  std::vector<boost::intrusive_ptr<SceneObject>> m_armorWisps
//        the two "weapon_<armor>_wisp" objects ApplyArmorTrinket spawns
//        and orbits around the hero
//
// NON-POLYMORPHIC: the golden exports no _ZTV/_ZTI/_ZTS for this class.
#ifndef CAVER_HEROEQUIPMENTMANAGER_H
#define CAVER_HEROEQUIPMENTMANAGER_H

#include <string>
#include <vector>

#include <boost/intrusive_ptr.hpp>
#include <boost/shared_ptr.hpp>

namespace Caver {

class GameState;
class Item;
class SceneObject;

class HeroEquipmentManager {
public:
    // Golden 0x43D5B4: zero-fills the whole object.
    HeroEquipmentManager();

    // Golden 0x439AB4: the compiler-emitted member teardown in reverse
    // declaration order (wisp vector, weapon vector, hero object, game
    // state).
    ~HeroEquipmentManager();

    // Golden 0x43D5C8: clears both vectors and stores the state/hero.
    void Init(const boost::shared_ptr<GameState>& gameState,
              const boost::intrusive_ptr<SceneObject>& object);

    // Golden 0x43D734: rebuilds the hero's weapon objects. Every
    // SwingableWeaponController component on the hero gets
    // "weapon_<item name>" as its template name, creates its weapon,
    // and the created object is kept in m_weapons with the character's
    // current weapon damage range applied.
    void EquipWeapon(const boost::shared_ptr<Item>& item);

    // Golden 0x43DCC8: stores the character's armour damage multiplier
    // (for the given armour plus the equipped armour trinket) in the
    // hero's HealthComponent.
    void EquipArmor(const boost::shared_ptr<Item>& item);

    // Golden 0x43DA6C: re-applies the character's weapon damage range
    // to one weapon object's DamageComponent.
    void UpdateWeaponDamage(const boost::intrusive_ptr<SceneObject>& object);

    // Golden 0x43DAC4: "hiro_plated" / "hiro_magicplated" / "hiro"
    // for the two armour names the hero model knows.
    std::string ModelNameForArmor(const boost::shared_ptr<Item>& item);

    // Golden 0x43DDD8: applies the trinket to every weapon in
    // m_weapons.
    void ApplyWeaponTrinket(const boost::shared_ptr<Item>& trinket);

    // Golden 0x43DE28: the glow colours and damage fields one trinket
    // gives a single weapon.
    void ApplyTrinketToWeapon(const boost::intrusive_ptr<SceneObject>& weapon,
                              const boost::shared_ptr<Item>& trinket);

    // Golden 0x43E248: respawns the two armour wisps for the trinket's
    // colour theme and stores the fire / ice / shadow charge counts in
    // the hero's HealthComponent.
    void ApplyArmorTrinket(const boost::shared_ptr<Item>& trinket);

    // Golden 0x43E80C: re-applies the weapon damage range to every
    // weapon in m_weapons.
    void ApplyLevelUp();

    // Golden 0x43E8F4: the shadow trinket drives the hero's mana
    // regeneration rate.
    void ApplyTrinketToSpells(const boost::shared_ptr<Item>& trinket);

    // Golden 0x43EA40: the ice trinket stores its count in the
    // spell's DamageComponent.
    void ApplyTrinketToSpell(SceneObject* spell,
                             const boost::shared_ptr<Item>& trinket);

    // Golden 0x43E894: pushes the character's skill damage range into
    // a spell object's DamageComponent (as magic damage, factor 1).
    void ApplyCharacterStatsToSpell(SceneObject* spell);

    boost::shared_ptr<GameState> m_gameState;                 // +0
    boost::intrusive_ptr<SceneObject> m_heroObject;           // +16
    std::vector<boost::intrusive_ptr<SceneObject>> m_weapons; // +24
    std::vector<boost::intrusive_ptr<SceneObject>> m_armorWisps; // +48
};

static_assert(sizeof(HeroEquipmentManager) == 72,
              "sizeof must be 72");

} // namespace Caver

#endif
