// Lua surface of the mod host. Credit: Quantum Creeper / Aevora Labs.
//
// RegisterLuaApis() installs the shared API tables (Mini, Mini.Character,
// Component, Caver) into the host state. PushModEnvironment() builds the
// per-mod sandbox the entry script runs inside: an environment whose reads
// fall through to the API globals but whose writes stay private, plus a
// ModData table rooted in that mod's own data directory.
#ifndef CAVER_MODLUAAPI_H
#define CAVER_MODLUAAPI_H

// Always pulled in first: call sites guard their bodies with
// #if CAVER_MOD_ENABLED, and an undefined macro evaluates to 0 there,
// which silently drops the code instead of failing to build.
#include "ModSupport.h"

#include <string>

struct lua_State;

namespace Caver {
namespace Mod {

// Installs Mini, Mini.Character, Component, Caver.* into `state`.
void RegisterLuaApis(lua_State* state);

// Pushes the sandbox environment for one mod. Leaves the table on the
// stack; the caller sets it as the entry chunk's environment.
//   identifier     manifest id, shown in logs and Mini.ListMods()
//   scriptDirectory directory `require` resolves from
//   dataDirectory   directory ModData persists into
void PushModEnvironment(lua_State* state, const std::string& identifier,
                        const std::string& scriptDirectory,
                        const std::string& dataDirectory);

}  // namespace Mod
}  // namespace Caver

#endif
