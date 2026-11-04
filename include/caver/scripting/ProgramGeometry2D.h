// ProgramGeometry2D.h - Lua bindings for 2D geometry and math engine.
// Credit: Quantum Creeper / Aevora Labs.

#ifndef CAVER_SCRIPTING_PROGRAMGEOMETRY2D_H
#define CAVER_SCRIPTING_PROGRAMGEOMETRY2D_H

struct lua_State;

namespace Caver {

class ProgramState;

class ProgramGeometry2D {
public:
    static void RegisterLibrary(ProgramState* state);
    static void RegisterLuaState(lua_State* L);
};

} // namespace Caver

#endif // CAVER_SCRIPTING_PROGRAMGEOMETRY2D_H
