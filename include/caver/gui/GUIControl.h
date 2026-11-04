// Recovered from libswordigo.so (1.4.13) evidence.
//
// GUIControl: GUIView + a std::multimap of target/action pairs keyed by
// control event. Vtable 328 bytes @0x634840 = the 38 GUIView slots plus
// one added slot:
//   slot 38 (+304) StateDidChange(State)
// (slots 0-37 identical to GUIView's; only the destructor slots 7/8 point
// at GUIControl's own D1/D0 — see the relocation dump in the midviews
// parity scratch.)
//
// Layout: GUIView (256 bytes), then
//   +256 std::multimap<int, TargetAction> targetActions
// (proven by AddTargetActionForControlEvent @0x51F110 inserting into the
// tree at this+256, RemoveActionTarget @0x51F334 walking it, and the
// destructor @0x51F0A0 destroying it). Further trailing members, if any,
// are UNRECOVERED — no function in this slice touches past +304.
// No out-of-line constructor exists in the golden (implicit in the
// original as well).
#ifndef CAVER_GUICONTROL_H
#define CAVER_GUICONTROL_H

#include <boost/function.hpp>

#include <map>

#include "GUIView.h"

namespace Caver {

class GUIEvent;

class GUIControl : public GUIView {
public:
    // UNRECOVERED: enumerator names/values — only the type identity is
    // proven (by the mangled signatures and the multimap key compares).
    enum State {
        // UNRECOVERED enumerators
    };

    // UNRECOVERED: enumerator names/values — same as above.
    enum Event {
        // UNRECOVERED enumerators
    };

    struct TargetAction {
        void* target;                                    // +0
        boost::function<void(void*, GUIEvent*)> action;  // +8

        ~TargetAction();
    };

    ~GUIControl();

    virtual void StateDidChange(State state);  // vtable slot 38

    void AddTargetActionForControlEvent(
        void* target,
        const boost::function<void(void*, GUIEvent*)>& action, Event event);
    void RemoveActionTarget(void* target);
    void SendActionsForControlEvent(Event event, unsigned int flags);

    std::multimap<int, TargetAction> targetActions;  // +256
};

} // namespace Caver

#endif
