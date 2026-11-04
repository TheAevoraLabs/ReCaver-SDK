// Recovered from libswordigo.so (1.4.13) evidence.
//
// GUIEvent is a PLAIN AGGREGATE — not a polymorphic class and not a base
// class. Proof:
//   * The function census (18,417 rows) contains ZERO member functions
//     of Caver::GUIEvent; all 134 GUIEvent-mentioning rows are other
//     classes' handlers (void*, GUIEvent*) or boost::function /
//     boost::shared_ptr machinery built on the type. (The "211" figure
//     is the raw name-occurrence count across those rows.)
//   * The golden's .dynsym has NO _ZTVN5Caver8GUIEventE (vtable),
//     NO _ZTIN5Caver8GUIEventE / _ZTSN5Caver8GUIEventE (typeinfo) and
//     NO _ZN5Caver8GUIEvent... function — while 380 other Caver
//     vtables are exported. The standalone RTTI name string
//     "N5Caver8GUIEventE" is also absent from the binary (present for
//     e.g. GUIControl, GUIResponder). Same result in the arm32 twin.
//   * boost::detail::sp_counted_impl_p<Caver::GUIEvent>::dispose
//     @0x51F9CC is a bare `operator delete(px)` — no destructor call,
//     so GUIEvent is trivially destructible (no user dtor, no vptr).
// It therefore reconciles exactly with GUIMouseEvent / GUIKeyboardEvent
// (GUIEventTypes.h): three unrelated plain payload structs for three
// different channels. The polymorphic event hierarchy in this engine
// is the separate gameplay system Caver::GameEvent (own out-of-line
// ~GameEvent, SendEvent/ProcessEvents statics, and subclasses such as
// DidEnterPortalGameEvent) — GUIEvent has no subclasses.
//
// Layout (sizeof 16, proven three independent ways):
//   * GUIControl::SendActionsForControlEvent @0x51F3FC allocates the
//     event with operator new(0x10), zeroes +0 (str xzr) and +8
//     (str wzr), then stores its `unsigned int flags` argument at +8
//     (str w24, [x20,#8] @0x51F4F4) and hands the object out as a
//     boost::shared_ptr<GUIEvent> (counted-base vtable @0x6349A0).
//   * GUITargetAction::Send @0x52EC1C copies the event as a single
//     __int128 (16 bytes) before invoking its action.
//   * GUIAnimation::Update @0x52EBD4 and GUIEffect::Update @0x52F450
//     each build a stack GUIEvent, zeroing +0 (qword) and +8 (dword),
//     and pass it to GUITargetAction::Send.
// Fields:
//   +0  void* — producers store 0; GUIApplication::SendEvent
//       @0x51B558 overwrites it with the qword at GUIApplication+48
//       before queueing. UNRECOVERED: the field's name/role, and what
//       GUIApplication+48 is (no GUIApplication function ever writes
//       it; the sharedApplication static is zero-initialised).
//   +8  int — the flags argument of
//       GUIControl::SendActionsForControlEvent(Event, unsigned int).
//       UNRECOVERED: field name and flag-bit meanings.
//   +12 4 bytes never written by any producer: tail padding or a
//       field no producer sets — UNRECOVERED which.
// Spot-checked handlers (SettingsView::SliderTouchUp @0x4D369C,
// GameSceneView::CinematicSkipButtonPressed @0x42A190) never read the
// event pointer at all; handlers identify the sender from the first
// (void*) argument instead.
#ifndef CAVER_GUIEVENT_H
#define CAVER_GUIEVENT_H

namespace Caver {

class GUIEvent {
public:
    void* field0;  // +0  UNRECOVERED name/role
    int field8;    // +8  flags (see header comment)
    int _pad12;    // +12 UNRECOVERED: padding or unset field
};

} // namespace Caver

#endif
