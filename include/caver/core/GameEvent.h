#ifndef CAVER_GAMEEVENT_H
#define CAVER_GAMEEVENT_H

#include <map>
#include <string>
#include <boost/shared_ptr.hpp>

namespace Caver {

class IGameEventResponder;

class GameEvent {
public:
    GameEvent();
    ~GameEvent();

    static GameEvent* NewCounterEvent(const std::string& name, int value);
    static void SendEvent(GameEvent* event);
    static void ProcessEvents();
    static void ClearEvents();
    static void SetFirstResponder(IGameEventResponder* responder);
    static void SetEventsEnabled(bool enabled);

    int m_type;                                                 // +0
    int _pad4;                                                  // +4
    std::map<std::string, std::string> m_stringProperties;      // +8
    std::map<std::string, int> m_intProperties;                 // +32
};

// Golden aarch64 NDK-libc++ layout anchor; MSVC STL member sizes differ,
// so this assert is Android/Linux-only (the Windows build is self-consistent
// under MSVC STL and does not claim golden byte-layout parity).
#ifndef _MSC_VER
static_assert(sizeof(GameEvent) == 0x38, "GameEvent size must be 0x38");
#endif

} // namespace Caver

#endif // CAVER_GAMEEVENT_H
