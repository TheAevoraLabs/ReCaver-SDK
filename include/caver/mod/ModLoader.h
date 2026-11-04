// Caver::Mod::ModLoader - engine-native mod host.
// Credit: Quantum Creeper / Aevora Labs.
//
// This is the replacement for the old binary-hooking launch stack
// (GlossHook / Dobby, raw struct offsets, fake GLES overlay canvases).
// ReCaver owns the runtime, so a mod is simply Lua running against the
// engine's real classes plus optional native libraries the host loads.
// Nothing here patches machine code, and nothing here is reached at all
// in a CAVER_ORIGINAL parity build (see ModSupport.h).
//
// Lifecycle: the desktop launcher points the loader at a mods directory,
// the engine calls Tick() once per frame from GameViewController::Update,
// and the loader drives the Lua hooks the mods define:
//
//   OnModInit()               once, after a mod's entry script runs
//   OnSceneLoad(levelName)    whenever the live Scene changes
//   OnFrameUpdate(deltaTime)  every frame
//   OnShutdown()              before the Lua host is torn down
#ifndef CAVER_MODLOADER_H
#define CAVER_MODLOADER_H

#include <string>
#include <vector>

#include "ModSupport.h"

struct lua_State;

namespace Caver {

class GameViewController;
class GameSceneController;
class Scene;
class GameState;
class PlayerProfile;

namespace Mod {

// Live engine pointers the Lua API resolves against. This replaces the
// old gsc_from_L() global-table trick: the host is told what the current
// game object is each frame, instead of fishing a pointer out of Lua.
struct EngineContext {
    GameViewController* viewController;
    GameSceneController* sceneController;
    Scene* scene;
    GameState* gameState;
    PlayerProfile* profile;

    EngineContext();
    void Clear();
    bool Available() const { return sceneController != 0 && scene != 0; }
};

// One discovered mod. `identifier` comes from mod.toml; when the manifest
// is missing or has no id, the folder name is used.
struct ModInfo {
    std::string identifier;
    std::string name;
    std::string version;
    std::string author;
    std::string description;
    std::string directory;    // absolute path to the mod folder
    std::string entryScript;  // path relative to the mod folder
    std::string nativeLibrary;// path relative to the mod folder (optional)
    bool loaded;
    bool failed;
    std::string error;
    // Registry reference to the mod's sandbox environment, or -2
    // (LUA_NOREF) when the mod never loaded. Internal to the loader.
    int environmentRef;
    std::vector<void*> nativeHandles;

    ModInfo();
};

class ModLoader {
public:
    static ModLoader& Get();

    // Explicit mods root. Without it the loader uses
    // <ResourcesPath>/mods on first tick.
    void SetModsDirectory(const std::string& directory);
    const std::string& ModsDirectory() const { return m_modsDirectory; }

    // Drops every loaded mod and rescans the mods directory.
    void Reload();

    // Called once per frame by the engine. Initialises on first call.
    void Tick(float deltaTime, GameViewController* viewController);

    // Dispatches OnShutdown and closes the Lua host.
    void Shutdown();

    // Refreshes engine context pointers when scene changes.
    void RefreshContext(GameViewController* viewController);

    const std::vector<ModInfo>& Mods() const { return m_mods; }
    lua_State* LuaState() const { return m_luaState; }
    bool IsInitialized() const { return m_initialized; }
    EngineContext& Context() { return m_context; }

private:
    ModLoader();
    ~ModLoader();
    ModLoader(const ModLoader&);
    ModLoader& operator=(const ModLoader&);

    void EnsureInitialized();
    void CreateHost();
    void DestroyHost();
    bool LoadMod(const std::string& directory, ModInfo* info);
    void DispatchFrame(float deltaTime);
    void DispatchSceneLoad(const std::string& levelName);
    void DispatchShutdown();
    bool RunHook(int modIndex, const char* hookName, bool hasNumberArgument, float number,
                 bool hasStringArgument, const std::string& text);

    std::string m_modsDirectory;
    lua_State* m_luaState;
    bool m_initialized;
    std::vector<ModInfo> m_mods;
    EngineContext m_context;
    Scene* m_lastScene;
    bool m_dispatching;
    bool m_tracePrinted;  // CAVER_MOD_TRACE=1 diagnostics, printed once
};

}  // namespace Mod
}  // namespace Caver

#endif
