# ReCaver Event Relay Reference

The ReCaver event system is defined in `<caver/mod/ModEvents.h>` under the namespace `Caver::Mod::Events`. It provides thread-safe, non-invasive callbacks throughout the game engine lifecycle.

---

## 1. Frame Lifecycle Events

### `OnPreUpdate`
```cpp
void OnPreUpdate(std::function<void(float dt)> callback);
```
Fired at the start of each engine frame in `CaverShell::Update` before player inputs, physics simulation, camera interpolation, and entity logic run.
- `dt`: Time elapsed in seconds since the previous frame.

### `OnPostUpdate`
```cpp
void OnPostUpdate(std::function<void(float dt)> callback);
```
Fired at the conclusion of each engine frame after all components, particles, scenes, and UI windows have updated. Useful for inspecting modified object transforms or dispatching post-frame logic.

---

## 2. Rendering Lifecycle Events

### `OnPreRender`
```cpp
void OnPreRender(std::function<void(RenderingContext* context)> callback);
```
Fired in `CaverShell::Render` before the main engine window renders.
- `context`: Pointer to active `Caver::RenderingContext` providing draw buffers, matrix stacks, and shader programs.

### `OnPostRender`
```cpp
void OnPostRender(std::function<void(RenderingContext* context)> callback);
```
Fired in `CaverShell::Render` after all game geometry, post-processing pipelines, and GUI overlays have been submitted to OpenGL. Ideal for custom debug HUD drawing or overlays.

---

## 3. Scene Lifecycle Events

### `OnSceneLoaded`
```cpp
void OnSceneLoaded(std::function<void(Scene* scene, const std::string& name)> callback);
```
Dispatched whenever a `.scene` level file finishes loading.
- `scene`: Pointer to the active `Caver::Scene` instance.
- `name`: File path or asset name of the loaded level (e.g. `levels/village.scene`).

### `OnSceneUnload`
```cpp
void OnSceneUnload(std::function<void(Scene* scene)> callback);
```
Dispatched in `Scene::~Scene` immediately before the world graph, entity arrays, and spatial partitioning grids are released. Mod authors must invalidate any cached raw pointers to scene objects.

---

## 4. Entity Lifecycle & Combat Interceptors

### `OnEntitySpawned`
```cpp
void OnEntitySpawned(std::function<void(SceneObject* object)> callback);
```
Dispatched whenever an entity is activated into the active scene via `Scene::ActivateObject`. Allows mods to attach custom components, adjust base attributes, or track monsters.

### `OnEntityTakeDamage`
```cpp
void OnEntityTakeDamage(std::function<bool(SceneObject* target, float* damage)> callback);
```
Fired when any damage calculation occurs against a target entity.
- `target`: The entity receiving damage.
- `damage`: Mutable pointer to the incoming damage value.
- **Return Value**:
  - Return `true` to allow damage to apply.
  - Return `false` to cancel the damage event entirely (e.g. invulnerability or dodging).

---

## 5. Virtual Asset Filesystem Interceptor

### `OnResolveAsset`
```cpp
void OnResolveAsset(std::function<bool(const std::string& path, void** outBytes, size_t* outSize)> callback);
```
Fired in `ResourceFetch.cpp` whenever the engine requests an asset file (textures, models, audio files, scenes).
- `path`: Virtual asset path requested by the engine.
- `outBytes`: Destination pointer where the mod places dynamically loaded or generated file memory.
- `outSize`: Size in bytes of the returned buffer.
- **Return Value**: Return `true` if the mod supplied replacement asset bytes, or `false` to let the engine fall back to vanilla game assets.
