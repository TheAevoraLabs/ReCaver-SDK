// ReCaver SDK Example: Godmode & Damage Multiplier Mod
// Credit: Quantum Creeper / Aevora Labs

#include <caver/mod/ModEvents.h>
#include <caver/scene/Scene.h>
#include <caver/scene/SceneObject.h>
#include <iostream>

namespace {

bool s_godmodeEnabled = true;
float s_damageMultiplier = 2.5f;

} // namespace

extern "C" void recaver_mod_init() {
    std::cout << "[GodmodeMod] Native module loaded and registering interceptors..." << std::endl;

    // Intercept entity damage
    Caver::Mod::Events::OnEntityTakeDamage([](Caver::SceneObject* target, float* damage) -> bool {
        if (!target || !damage) {
            return true;
        }

        // Check if the entity is the hero
        bool isHero = (target->m_identifier == "hero");
        if (s_godmodeEnabled && isHero) {
            std::cout << "[GodmodeMod] Prevented damage to Hero!" << std::endl;
            return false; // Cancel damage
        }

        // Multiply damage against enemies
        if (!isHero) {
            *damage *= s_damageMultiplier;
        }

        return true;
    });

    Caver::Mod::Events::OnSceneLoaded([](Caver::Scene*, const std::string& name) {
        std::cout << "[GodmodeMod] Active scene loaded: " << name << std::endl;
    });
}

extern "C" void recaver_mod_shutdown() {
    std::cout << "[GodmodeMod] Native module shutting down." << std::endl;
}
