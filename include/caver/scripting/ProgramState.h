// Recovered from libswordigo.so (1.4.13) evidence — see docs/PARITY-001.md
// for the method. ProgramState is the engine's Lua script runner: each
// instance owns (root) or borrows (child states, via lua_newthread) a
// lua_State, and scenes drive it through Update/Execute/Resume.
//
// Object size is 0x60 bytes, verified from CreateChildState's
// operator new(0x60). Layout from the constructor/destructor expansions:
//   +0   lua_State* m_luaState
//   +8   ProgramState* m_parentState (0 for the root state)
//   +16  std::list<boost::shared_ptr<ProgramState> > m_childStates
//   +40  boost::intrusive_ptr<SceneObject> m_parentObject
//   +48  ProgramTable m_globalsTable  (index LUA_GLOBALSINDEX, -10002)
//   +64  ProgramTable m_registryTable (index LUA_REGISTRYINDEX, -10000)
//   +80  int m_runState (0 = ready, 1 = waiting on the timer, 2 = aborted)
//   +84  float m_waitTimer
//   +88  bool m_blockChildrenUpdate (children update only if their
//        m_updateWhileParentBlocked is set) — name is a reconstruction
//   +89  bool m_active (initialised true; Update runs only if this or
//        m_updateWhileParentBlocked is set) — name is a reconstruction
//   +90  bool m_updateWhileParentBlocked — name is a reconstruction
//   +91  bool m_finished (set when a resume completes, and by Abort)
//   +92  float m_speed (initialised 1.0f)
#ifndef CAVER_PROGRAMSTATE_H
#define CAVER_PROGRAMSTATE_H

#include <list>
#include <string>
#include <boost/intrusive_ptr.hpp>
#include <boost/shared_ptr.hpp>

#ifdef _MSC_VER
// Win32-macro hygiene: Boost above pulls in windows.h, which defines
// GetMessage and RegisterClass as function-like macros. Without these
// #undefs, the RegisterClass declaration below (line 108) would be
// renamed to RegisterClassA at parse time in every TU that includes
// this header, while the out-of-line definition in ProgramState.cpp
// is plain RegisterClass -- a mismatch MSVC (unlike Itanium, which
// does not encode the difference) refuses to link. The engine never
// calls the Win32 GetMessage/RegisterClass APIs, so undefining the
// macros here is safe for all includers.
#undef GetMessage
#undef RegisterClass
#endif

#include "ProgramTable.h"
// The engine's scene object (components slice's class). ProgramState
// only calls updateSpeedMultiplier() and holds it through an
// intrusive_ptr. Included at file scope (NOT inside namespace Caver).
#include "../scene/SceneObject.h"

struct lua_State;

namespace Caver {

class Program;
class Rectangle;
class Vector3;

// The "Program" library's Lua C functions are file-local statics in
// ProgramState.cpp; declared here (internal linkage, defined only in
// that TU) so ProgramState can befriend them.
static int ProgramWait(lua_State* state);
static int ProgramPrint(lua_State* state);
static int ProgramSetKeepActive(lua_State* state);



// Refcount glue for boost::intrusive_ptr<SceneObject>, matching the
// expansions inlined across the binary (SetParentObject,
// PrepareForRemoval, ~ProgramState, Update): the counter lives at +8
// and the final release dispatches through vtable slot +8.
// The vtable call is noexcept-qualified: the golden binary has no
// standalone intrusive_ptr<SceneObject> destructor anywhere, and
// with a plain (potentially-throwing) function-pointer call Clang
// refuses to inline this release at use sites and instead emits an
// outlined weak intrusive_ptr<SceneObject> destructor (proven on
// BushControllerComponent, ProgramState and BreakableObjectComponent
// before this change). The destructor itself is noexcept in the
// golden, so the qualification changes no observable behaviour; it
// only lets the release inline exactly as in the golden.
inline void intrusive_ptr_add_ref(SceneObject* object)
{
    ++*(int*)((char*)object + 8);
}

inline void intrusive_ptr_release(SceneObject* object)
{
    if (--*(int*)((char*)object + 8) == 0) {
        void** vtable = *(void***)object;
        ((void (*)(SceneObject*) noexcept)(vtable[1]))(object);
    }
}

// One entry of a Lua registration table (the engine's own spelling of
// luaL_Reg; passed straight to luaL_register).
struct LibFunction {
    const char* name;
    int (*func)(lua_State*);
};

// Installed as the lua_State panic function for root states. Throws
// the int 0 (verified: __cxa_allocate_exception(4) + __cxa_throw of int).
int ProgramPanic(lua_State* state);

class ProgramState {
public:
    ProgramState(ProgramState* parent);
    ~ProgramState();

    static ProgramState* FromLuaState(lua_State* state);
    lua_State* GetLuaState() const { return m_luaState; }

    void RegisterProgramLibrary();
    void RegisterLibrary(const std::string& name, const LibFunction* functions);

    // The two tables are reached directly by Scene::RegisterLibrary
    // in the golden binary (SetPointerForKey called on the embedded
    // globals table at Scene+88), so they are accessible here.
    ProgramTable& GlobalsTable() { return m_globalsTable; }
    ProgramTable& RegistryTable() { return m_registryTable; }
    void RegisterFunctions(const LibFunction* functions);
    void RegisterClass(const std::string& name, const LibFunction* methods,
                       const LibFunction* metamethods);

    boost::shared_ptr<ProgramState> CreateChildState();
    void RemoveChildState(const boost::shared_ptr<ProgramState>& state);
    void SetParentObject(const boost::intrusive_ptr<SceneObject>& object);
    void PrepareForRemoval();

    void Update(float deltaTime);
    int Execute(int argCount);
    int Resume(int argCount);
    int ExecuteProgram(const Program& program);
    bool LoadProgram(const Program& program);
    int ExecuteString(const std::string& code);

    void Wait(float seconds);
    void Abort();

    // The "Program" library's Lua C functions (file-local in
    // ProgramState.cpp) drive the run state directly.
    friend int ProgramWait(lua_State* state);
    friend int ProgramPrint(lua_State* state);
    friend int ProgramSetKeepActive(lua_State* state);

    void PushInt(int value);
    void PushFloat(float value);
    void PushBool(bool value);
    void PushString(const std::string& value);
    void PushPointer(void* pointer);
    void PushVector3(const Vector3& value);
    void PushRectangle(const Rectangle& value);
    void PushValueAtStackIndex(int index);
    void* PointerAtStackIndex(int index);
    int IntAtStackIndex(int index);
    float FloatAtStackIndex(int index);
    bool BoolAtStackIndex(int index);
    std::string StringAtStackIndex(int index);
    bool IsIntAtStackIndex(int index);
    bool IsFloatAtStackIndex(int index);
    bool IsBoolAtStackIndex(int index);
    void Pop(int count);
    void SetTable(int index);
    void* NewUserData(unsigned long size, const std::string& metatableName);
    int NewMetatable(const std::string& name);

private:
    // Assigns m_parentObject with exactly the refcount sequence the
    // binary inlines everywhere (add-ref the new object, store, then
    // release the old one — with the release as a tail dispatch).
    // boost::intrusive_ptr's own assignment goes through a temporary
    // and a swap, which does not reproduce that sequence.
    void AssignParentObject(SceneObject* object)
    {
        if (object)
            intrusive_ptr_add_ref(object);
        SceneObject* old = m_parentObject.get();
        *(SceneObject**)&m_parentObject = object;
        if (old)
            intrusive_ptr_release(old);
    }

    lua_State* m_luaState;                                      // +0
    ProgramState* m_parentState;                                // +8
    std::list<boost::shared_ptr<ProgramState> > m_childStates;  // +16
    boost::intrusive_ptr<SceneObject> m_parentObject;           // +40
    ProgramTable m_globalsTable;                               // +48
    ProgramTable m_registryTable;                               // +64
    int m_runState;                                             // +80
    float m_waitTimer;                                          // +84
    bool m_blockChildrenUpdate;                                 // +88
    bool m_active;                                              // +89
    bool m_updateWhileParentBlocked;                            // +90
    bool m_finished;                                            // +91
    float m_speed;                                              // +92
};

} // namespace Caver

#endif
