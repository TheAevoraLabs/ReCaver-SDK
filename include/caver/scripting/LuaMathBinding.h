// ReCaver Lua Math & Raymath Bindings
// Credit: Quantum Creeper / Aevora Labs

#ifndef CAVER_LUAMATHBINDING_H
#define CAVER_LUAMATHBINDING_H

struct lua_State;

namespace Caver {
namespace Scripting {

void RegisterLuaMath(lua_State* L);

} // namespace Scripting
} // namespace Caver

#endif // CAVER_LUAMATHBINDING_H
