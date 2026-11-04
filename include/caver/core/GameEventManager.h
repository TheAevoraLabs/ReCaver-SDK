// Recovered from libswordigo.so (1.4.13) evidence — GameEventManager
// layout pinned by GameEvent::SendEvent @0x472B78, ClearEvents
// @0x472F5C, SetFirstResponder @0x47301C, SetEventsEnabled @0x47306C
// and ProcessEvents @0x472DC0 (jni-small SF-1):
//   +0  IGameEventResponder* m_firstResponder (SetFirstResponder
//       stores the raw pointer here; ProcessEvents calls its vtable
//       slot 0)
//   +8  std::vector<boost::shared_ptr<GameEvent>> m_events
//       (begin +8, end +16, capacity +24 — SendEvent push_backs at
//       this+8)
//   +32 bool m_eventsEnabled (initialised to 1 by the lazy
//       currentManager() allocation in every GameEvent static)
// sizeof 40 (0x28 — the currentManager operator new(0x28)).
// NOTE: IGameEventResponder is also declared by the gui slice in
// GameViewController.h with more commentary; the two declarations
// must be reconciled at merge (same single virtual, slot 0).
#ifndef CAVER_GAMEEVENTMANAGER_H
#define CAVER_GAMEEVENTMANAGER_H

#include <vector>
#include <boost/shared_ptr.hpp>

namespace Caver {

class GameEvent;

// Responder interface: slot 0 is HandleGameEvent, proven by
// GameViewController's vtable and by ProcessEvents dispatching
// through it. Return type bool per the gui slice's finding.
class IGameEventResponder {
public:
    virtual bool HandleGameEvent(const GameEvent& event) = 0;
};

class GameEventManager {
public:
    void ProcessEvents();

    IGameEventResponder* m_firstResponder;                    // +0
    std::vector<boost::shared_ptr<GameEvent> > m_events;      // +8
    bool m_eventsEnabled;                                     // +32
};

} // namespace Caver

#endif
