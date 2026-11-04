// ReCaver Mod Events & Native Interceptor Dispatcher.
// Credit: Quantum Creeper / Aevora Labs.
// Provides first-class event hooks without external assembly hooking libraries.

#ifndef CAVER_MODEVENTS_H
#define CAVER_MODEVENTS_H

#include "ModSupport.h"

#include <string>
#include <vector>
#include <functional>
#include <cstddef>

namespace Caver {

class Scene;
class SceneObject;
class RenderingContext;

namespace Mod {
namespace Events {

#if !CAVER_MOD_ENABLED

inline void OnPreUpdate(std::function<void(float dt)>) {}
inline void OnPostUpdate(std::function<void(float dt)>) {}
inline void OnPreRender(std::function<void(RenderingContext* context)>) {}
inline void OnPostRender(std::function<void(RenderingContext* context)>) {}
inline void OnSceneLoaded(std::function<void(Scene* scene, const std::string& name)>) {}
inline void OnSceneUnload(std::function<void(Scene* scene)>) {}
inline void OnEntitySpawned(std::function<void(SceneObject* object)>) {}
inline void OnEntityTakeDamage(std::function<bool(SceneObject* target, float* damage)>) {}
inline void OnResolveAsset(std::function<bool(const std::string& virtualPath, void** outBytes, size_t* outSize)>) {}
inline void ClearAllListeners() {}

inline void DispatchPreUpdate(float) {}
inline void DispatchPostUpdate(float) {}
inline void DispatchPreRender(RenderingContext*) {}
inline void DispatchPostRender(RenderingContext*) {}
inline void DispatchSceneLoaded(Scene*, const std::string&) {}
inline void DispatchSceneUnload(Scene*) {}
inline void DispatchEntitySpawned(SceneObject*) {}
inline bool DispatchEntityTakeDamage(SceneObject*, float*) { return false; }
inline bool DispatchResolveAsset(const std::string&, void**, size_t*) { return false; }

#else

// Public Modder Subscription APIs
void OnPreUpdate(std::function<void(float dt)> callback);
void OnPostUpdate(std::function<void(float dt)> callback);
void OnPreRender(std::function<void(RenderingContext* context)> callback);
void OnPostRender(std::function<void(RenderingContext* context)> callback);
void OnSceneLoaded(std::function<void(Scene* scene, const std::string& name)> callback);
void OnSceneUnload(std::function<void(Scene* scene)> callback);
void OnEntitySpawned(std::function<void(SceneObject* object)> callback);
void OnEntityTakeDamage(std::function<bool(SceneObject* target, float* damage)> callback);
void OnResolveAsset(std::function<bool(const std::string& virtualPath, void** outBytes, size_t* outSize)> callback);

void ClearAllListeners();

// Internal Engine Dispatchers
void DispatchPreUpdate(float dt);
void DispatchPostUpdate(float dt);
void DispatchPreRender(RenderingContext* context);
void DispatchPostRender(RenderingContext* context);
void DispatchSceneLoaded(Scene* scene, const std::string& name);
void DispatchSceneUnload(Scene* scene);
void DispatchEntitySpawned(SceneObject* object);
bool DispatchEntityTakeDamage(SceneObject* target, float* damage);
bool DispatchResolveAsset(const std::string& path, void** outBytes, size_t* outSize);

#endif // CAVER_MOD_ENABLED

} // namespace Events
} // namespace Mod
} // namespace Caver

#endif // CAVER_MODEVENTS_H
