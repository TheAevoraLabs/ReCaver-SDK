# ReCaver Native Modding SDK

Welcome to the official **ReCaver Modding SDK**.

ReCaver is the reconstructed, enhanced edition of CaverEngine (the custom 2.5D C++ game engine behind *Swordigo*). Unlike traditional legacy game modding solutions that rely on dangerous runtime binary patching (e.g. Dobby, GlossHook, memory injection, raw memory offsets, or fragile OpenGL hooks), ReCaver provides **first-class native engine integration**.

Every ReCaver binary (on both Linux desktop and Android) is compiled with exported dynamic symbols (`-rdynamic -Wl,--export-dynamic`). Stripped release binaries retain 100% of the mangled C++ engine symbols in `.dynsym`. This means modders can write standard C++, C, or Rust shared libraries (`.so` / `.dll`), `#include <caver/...>` real engine headers, and directly interact with live game objects and the event relay without hacking assembly.

---

## Key Features

- **Direct C++ Engine Headers**: 380+ real engine headers matching internal classes (`Scene`, `SceneObject`, `RenderingContext`, `Model`, `Mesh`, `GameState`, `PlayerProfile`, `GUIView`).
- **No-Hook Event Relay**: High-performance pre/post update, pre/post render, scene lifecycle, entity spawn, damage filtering, and asset override events via `Caver::Mod::Events`.
- **Dual Runtime Pipeline**: Support for pure Lua sandbox mods, pure native shared library mods, or hybrid Lua + C++ mods.
- **Zero Raw Pointers / Offset Hacks**: Mod code calls typed engine methods directly. The dynamic linker resolves symbols against the running game executable or `libswordigo.so`.
- **Multi-Platform Support**: Cross-platform modding targeting Linux x86_64 and Android ARM64 (`arm64-v8a`).

---

## Directory Structure

```text
ReCaver-SDK/
├── include/
│   └── caver/              Real C++ engine headers
│       ├── animation/      Keyframes, skeletal animation, particle systems
│       ├── audio/          Sound library, OpenAL audio emitters, music player
│       ├── component/      Entity-component-system components (Health, Attack, Spell, etc.)
│       ├── core/           VectorMath, Matrix4, Color, Camera, GameState, ResourceFetch
│       ├── gui/            GUIView, GUIWindow, GameViewController, HUD bars
│       ├── mod/            ModEvents.h, ModLoader.h, ModSupport.h
│       ├── model/          GLBLoader, PODLoader, Model, Mesh, Skeleton
│       ├── physics/        CollisionShape, PhysicsObjectState, contact resolution
│       ├── platform/       CaverShell, FWKeyboard, FWMouse, NativeJNI
│       ├── proto/          Protobuf serialization schemas and state definitions
│       ├── render/         RenderingContext, GL3Backend, PostProcessPipeline, Texture
│       ├── scene/          Scene, SceneObject, SceneGrid, ObjectTemplate
│       └── systems/        PlayerProfile, ProfileManager, AchievementsManager, Items
├── docs/                   Guides, architecture specifications, packaging docs
└── examples/               Starter examples in C++ and Rust
```

---

## Quickstart: Creating Your First Native C++ Mod

### 1. Mod Layout

Create a directory inside the ReCaver `mods/` directory (e.g. `mods/my_awesome_mod/`):

```text
mods/my_awesome_mod/
├── mod.toml
└── bin/
    └── linux-x86_64/
        └── libmy_awesome_mod.so
```

### 2. Manifest (`mod.toml`)

```toml
[mod]
id = "my_awesome_mod"
name = "My Awesome Native Mod"
version = "1.0.0"
author = "Your Name"
description = "Demonstration of native ReCaver event relay"

[entry]
# Optional: explicitly specify library path if not in standard bin/<arch>/ directory
native = "bin/linux-x86_64/libmy_awesome_mod.so"
```

### 3. Native Entry Point (`mod_main.cpp`)

```cpp
#include <caver/mod/ModEvents.h>
#include <caver/scene/Scene.h>
#include <caver/scene/SceneObject.h>
#include <caver/render/RenderingContext.h>
#include <iostream>

// Entry point called by ReCaver when the mod is loaded
extern "C" void recaver_mod_init() {
    std::cout << "[MyAwesomeMod] Initializing native module..." << std::endl;

    // Listen to scene load events
    Caver::Mod::Events::OnSceneLoaded([](Caver::Scene* scene, const std::string& name) {
        std::cout << "[MyAwesomeMod] Scene loaded: " << name << std::endl;
    });

    // Intercept damage (Godmode or damage multiplier)
    Caver::Mod::Events::OnEntityTakeDamage([](Caver::SceneObject* target, float* damage) -> bool {
        // Return false to cancel damage, or modify *damage in-place
        return true;
    });

    // Hook frame updates
    Caver::Mod::Events::OnPostUpdate([](float dt) {
        // Execute per-frame logic
    });
}

// Optional exit point called when the mod host shuts down
extern "C" void recaver_mod_shutdown() {
    std::cout << "[MyAwesomeMod] Shutting down..." << std::endl;
}
```

### 4. Compiling the Shared Library

Using GCC/Clang on Linux:

```bash
g++ -O2 -fPIC -shared -std=c++14 \
    -I/path/to/ReCaver-SDK/include \
    mod_main.cpp \
    -o libmy_awesome_mod.so
```

Because ReCaver is compiled with `-rdynamic -Wl,--export-dynamic`, your shared library does not need a static stub library; the OS dynamic loader resolves all `Caver::*` symbols automatically when ReCaver loads your `.so` via `dlopen`.

---

## Event Relay Reference

All event relays live in `<caver/mod/ModEvents.h>` under namespace `Caver::Mod::Events`:

| Function | Signature | Description |
|---|---|---|
| `OnPreUpdate` | `(float dt)` | Executed before engine subsystem and scene updates |
| `OnPostUpdate` | `(float dt)` | Executed after engine subsystem and scene updates |
| `OnPreRender` | `(RenderingContext* ctx)` | Executed before engine draws window and scene rect |
| `OnPostRender` | `(RenderingContext* ctx)` | Executed after engine finishes GUI and scene drawing |
| `OnSceneLoaded` | `(Scene* scene, const std::string& name)` | Dispatched when a new `.scene` file has loaded |
| `OnSceneUnload` | `(Scene* scene)` | Dispatched immediately before the active scene is destroyed |
| `OnEntitySpawned` | `(SceneObject* obj)` | Dispatched whenever an entity is activated into the world |
| `OnEntityTakeDamage` | `(SceneObject* target, float* dmg) -> bool` | Filter/modify damage. Return false to cancel |
| `OnResolveAsset` | `(const std::string& path, void** bytes, size_t* sz) -> bool` | Virtual asset filesystem interceptor |

---

## License & Credits

- Maintained by: Quantum Creeper / Aevora Labs
- Target Engine: TouchFoo CaverEngine / ReCaver
- License: MIT
