// Recovered from libswordigo.so (1.4.13) evidence — see docs/CORE_HEADERS.md.
// A single touch point as delivered by the platform layer. The position
// pair at +8/+16 is scaled by CaverShell::ConvertTouchToWindow; the full
// 40-byte size is verified from the shell's stack copies.
#ifndef CAVER_FWTOUCH_H
#define CAVER_FWTOUCH_H

#include "Vector2.h"

namespace Caver {

class GUIView;

struct FWTouch {
    // The golden binary has an out-of-line copy constructor and assignment
    // operator for this struct (_ZN5Caver7FWTouchC2ERKS0_ @0x53315C, 36 B;
    // _ZN5Caver7FWTouchaSERKS0_ @0x533180), both plain field copies — so
    // every copy in the shell's touch paths is a call, not an inline move.
    // The definition belongs in the core slice (FWTouch.cpp); it is not
    // defined here.
    FWTouch(const FWTouch& other);
    // _ZN5Caver7FWTouchaSERKS0_ @0x533180, 36 bytes: a plain field copy
    // that returns the receiver.
    FWTouch& operator=(const FWTouch& other);

    // Out-of-line in the golden binary
    // (_ZNK5Caver7FWTouch14LocationInViewEPKNS_7GUIViewE). Declared
    // here for the systems slice's MapView reconstruction
    // (2026-10-23); the definition belongs to the core slice.
    Vector2 LocationInView(const GUIView* view) const;
    Vector2 PreviousLocationInView(const GUIView* view) const;

    unsigned long long identifier;
    Vector2 position;
    Vector2 previousPosition;
    double timestamp;
    int tapCount;
    int phase;

    // FWShell keeps its live touches in a std::set<FWTouch>; the inlined
    // std::less<FWTouch> comparison in the original compares the first
    // eight bytes only (FWShell::TouchMoved @0x50C1B8 loads a single u64 at
    // +0 of the touch and compares it against the node key).
    bool operator<(const FWTouch& other) const { return identifier < other.identifier; }
};

} // namespace Caver

#endif
