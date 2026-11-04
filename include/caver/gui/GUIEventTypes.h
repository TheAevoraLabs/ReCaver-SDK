// Recovered from libswordigo.so (1.4.13) evidence — type-only classes
// from the small-GUI assignment. None of these types has any member
// function of its own in the function census: GUITextAlignment and
// GUIButtonType are plain enums passed by value, GUIMouseEvent and
// GUIKeyboardEvent are plain event structs consumed by
// GUIResponder/GUIView handlers. Only the fields actually read in the
// binary are laid out; the rest is marked UNRECOVERED.
#ifndef CAVER_GUIEVENTTYPES_H
#define CAVER_GUIEVENTTYPES_H

namespace Caver {

// Passed by value to GUITextFieldImpl::setHorizontalTextAlignment /
// setVerticalTextAlignment and FontText::AlignHorizontally.
// UNRECOVERED: enumerator names/values — no function in this slice
// reads them; do not invent.
enum GUITextAlignment {
    // UNRECOVERED enumerators
};

// Passed by value to GUIButton::GUIButton / SetButtonType.
// UNRECOVERED: enumerator names/values — same reason as above.
enum GUIButtonType {
    // UNRECOVERED enumerators
};

// Event struct: GUIResponder::HandleMouseEvent (0x523650) reads a dword
// at +12 and treats values 2 and 3 specially ((v & ~1) == 2 suppresses
// forwarding to the next responder).
// UNRECOVERED: the fields at +0..+11 and any fields past +16.
struct GUIMouseEvent {
    char _pad0[12];  // +0  UNRECOVERED contents
    int field12;     // +12 (values 2/3 proven)
    char _pad1[4];   // +16
};

// Event struct: only consumed by HandleKeyboardEvent handlers, which
// read no field of it in the base implementations.
// UNRECOVERED: full layout.
struct GUIKeyboardEvent {
    char _pad0[16];  // UNRECOVERED contents
};

} // namespace Caver

#endif
