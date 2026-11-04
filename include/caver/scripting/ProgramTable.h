// Recovered from libswordigo.so (1.4.13) evidence.
// A typed view over one Lua table (identified by a Lua pseudo-index):
// ProgramState embeds two of these — one for the globals table
// (LUA_GLOBALSINDEX, -10002) and one for the registry (LUA_REGISTRYINDEX,
// -10000). Layout verified from ProgramState's constructor stores and
// ProgramState::FromLuaState's stack-built instance:
//   +0  lua_State* m_state
//   +8  int        m_index        (the table's pseudo-index)
// sizeof = 16 bytes.
#ifndef CAVER_PROGRAMTABLE_H
#define CAVER_PROGRAMTABLE_H

#include <string>

struct lua_State;

#include <string>

namespace Caver {

class ProgramTable {
public:
    ProgramTable() : m_state(0), m_index(0) {}
    ProgramTable(lua_State* state, int index) : m_state(state), m_index(index) {}

    void SetPointerForPointerKey(void* key, void* value);
    void* PointerForPointerKey(void* key);

    void SetBoolForKey(const std::string& key, bool value);
    bool BoolForKey(const std::string& key);
    void SetIntForKey(const std::string& key, int value);
    int IntForKey(const std::string& key);
    void SetFloatForKey(const std::string& key, float value);
    float FloatForKey(const std::string& key);
    void SetStringForKey(const std::string& key, const std::string& value);
    std::string StringForKey(const std::string& key);
    std::string StringAtIndex(int index);

    // String-keyed accessors, proven by Caver::MusicPlayer::Register-
    // ProgramLibrary and its Lua callbacks in libswordigo.so, which call
    // SetPointerForKey("musicPlayer", this) / PointerForKey("musicPlayer")
    // on ProgramState's globals table (state + 48). Declarations added by
    // the systems/MusicPlayer slice; definitions belong to this slice.
    void SetPointerForKey(const std::string& key, void* value);
    void* PointerForKey(const std::string& key);

    lua_State* m_state; // +0
    int m_index;        // +8
};

} // namespace Caver

#endif
