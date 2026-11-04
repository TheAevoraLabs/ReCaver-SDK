// Recovered from libswordigo.so (1.4.13) evidence.
// A compiled script program (loaded from scene resources); loading it
// into a lua_State prepares it for ProgramState::Execute. Layout proven
// (sizeof 0x38): name string +0, source string +24, bytecode string
// +40 (both shared_ptr<std::string>).
#ifndef CAVER_PROGRAM_H
#define CAVER_PROGRAM_H

#include <string>

#include <boost/shared_ptr.hpp>

struct lua_State;

namespace Caver {

namespace Proto {
class Program;
}

class Program {
public:
    bool LoadIntoState(lua_State* state) const;
    bool InitWithString(const std::string& string, std::string* outString);
    // Declared for the systems slice (Caver::ObjectLibrary calls both,
    // proven at 0x4F0744 / 0x4F0C00); bodies belong to this slice.
    void LoadFromProtobufMessage(const Proto::Program& message);
    void SaveToProtobufMessage(Proto::Program* message) const;
    ~Program();

    std::string m_string0;                            // +0
    boost::shared_ptr<std::string> m_stringValue24;   // +24
    boost::shared_ptr<std::string> m_stringValue40;   // +40
};

} // namespace Caver

#endif
