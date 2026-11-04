// Recovered from libswordigo.so (1.4.13) evidence — see docs/PARITY-001.md
// and docs/census/FUNCTION_CENSUS.tsv.
//
// The SceneObject Lua library's state helpers. All three are STATIC in
// the golden (mangled with (ProgramState*, SceneObject*) and no this
// pointer; PushSceneObject is at 0x5066B8 in the census). The bodies
// live in SceneObject.cpp; this header is their single declaration, so
// callers no longer need a TU-local stub class declaring only
// PushSceneObject. The ProgramState* every caller passes is the Scene's
// embedded root state at Scene +40 (by value — see Scene.h).
#ifndef CAVER_SCENEOBJECTLIB_H
#define CAVER_SCENEOBJECTLIB_H

namespace Caver {

class ProgramState;
class SceneObject;

class SceneObjectLib {
public:
    static void PerformCleanupForSceneObject(ProgramState* state,
                                             SceneObject* object);
    static void PushSceneObject(ProgramState* state, SceneObject* object);
    static void RegisterLibrary(ProgramState* state);
};

} // namespace Caver

#endif // CAVER_SCENEOBJECTLIB_H
