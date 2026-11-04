// Recovered from libswordigo.so (1.4.13) evidence.
// Layout (from ~GUIApplication 0x51C200, DispatchEvents 0x51BFC4 and the
// Cancel* walkers, which index the deque internals directly):
//   +0  a raw pointer member, released with operator delete in the
//       destructor — UNRECOVERED: its role/type.
//   +8  std::deque<QueuedEvent>: map_begin +8, map_end +16,
//       map_end_cap +24, start +32, size +40 (56 elements per block,
//       72-byte elements — the libc++ deque layout, at offset +8).
//   +48 a qword member SendEvent copies into the queued event's first
//       field — UNRECOVERED: its role/type.
// There is no user constructor in the census: sharedApplication's
// function-local static is zero-initialised and registers ~GUIApplication
// with __cxa_atexit (0x51B4E4).
//
// QueuedEvent (72 bytes, from ~QueuedEvent 0x51B844 and the move-assign
// 0x51D02C): target +0, boost::function +8, shared_ptr<void> sender
// (px +40 / counted-base +48), shared_ptr<GUIEvent> event (+56/+64).
#ifndef CAVER_GUIAPPLICATION_H
#define CAVER_GUIAPPLICATION_H

#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>
#include <cstddef>
#include <deque>

namespace Caver {

class GUIEvent;

class GUIApplication {
public:
    struct QueuedEvent {
        void* target;                                     // +0
        boost::function<void(void*, GUIEvent*)> action;   // +8
        boost::shared_ptr<void> sender;                   // +40
        boost::shared_ptr<GUIEvent> event;                // +56

        ~QueuedEvent();
        QueuedEvent& operator=(QueuedEvent&& other);
    };

    std::deque<QueuedEvent> events;    // +0 (48 bytes: map_first, map_begin, map_end, map_end_cap, start, size)
    union {
        double m_lastTouchTimestamp;   // +48 (8 bytes)
        void* unknown48;               // +48
    };

    ~GUIApplication();

    static GUIApplication* sharedApplication();
    void SendEvent(void* target,
                   const boost::function<void(void*, GUIEvent*)>& action,
                   const boost::shared_ptr<void>& sender,
                   const boost::shared_ptr<GUIEvent>& event);
    void CancelEventsWithTargetAndSender(void* target, void* sender);
    void CancelEventsWithSender(void* sender);
    void DispatchEvents();
};

// Load-bearing: GUIWindow::StoreTouchTimestamp writes the touch
// timestamp at hardcoded +48 into the shared application, so this
// offset must hold for the NDK r27 libc++ deque layout.
// Golden aarch64 NDK-libc++ layout anchor; MSVC STL member sizes differ,
// so this assert is Android/Linux-only (the Windows build is self-consistent
// under MSVC STL and does not claim golden byte-layout parity).
#ifndef _MSC_VER
static_assert(offsetof(GUIApplication, m_lastTouchTimestamp) == 48,
              "GUIApplication timestamp must sit at +48");
#endif

} // namespace Caver

#endif
