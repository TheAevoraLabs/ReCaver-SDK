// Recovered from libswordigo.so (1.4.13) evidence.
// GUIResponder is the responder-chain base of the whole GUI layer.
//
// Vtable (from ABS64 relocations, golden vtable @0x635050, 72 bytes:
// 16-byte header + exactly 7 function slots — there is NO destructor
// slot, so GUIResponder has no virtual destructor):
//   slot 0 (+0)  TouchBegan(FWTouch const&)
//   slot 1 (+8)  TouchMoved(FWTouch const&)
//   slot 2 (+16) TouchEnded(FWTouch const&)
//   slot 3 (+24) TouchCancelled(FWTouch const&)
//   slot 4 (+32) HandleMouseEvent(GUIMouseEvent const&)
//   slot 5 (+40) HandleKeyboardEvent(GUIKeyboardEvent const&)
//   slot 6 (+48) nextResponder() const
// The golden GUIView vtable (@0x6354F0) reuses these exact functions for
// its slots 0-5 and overrides only nextResponder (slot 6), which pins
// both the slot order and GUIView's derivation from GUIResponder.
//
// RTTI (@0x6350A8, __vmi_class_type_info, 1 base): the single base is
// boost::enable_shared_from_this<GUIResponder>.
//
// Layout (sizeof = 32):
//   +0  vptr
//   +8  enable_shared_from_this weak_ptr (px +8, refcount block +16)
//   +24 m_nextResponder — read directly by nextResponder()
//       (_ZN5Caver12GUIResponder13nextResponderEv @0x523710:
//        `return *((_QWORD*)this + 3);`)
#ifndef CAVER_GUIRESPONDER_H
#define CAVER_GUIRESPONDER_H

#include <boost/enable_shared_from_this.hpp>

#include "../core/FWTouch.h"

namespace Caver {

// struct, not class: GUIEventTypes.h defines both as struct, and MSVC
// mangles struct (U) and class (V) parameters differently, so the
// class forward declarations made every HandleMouseEvent /
// HandleKeyboardEvent override reference a symbol nothing defines.
// Itanium does not encode struct/class, which is why this only
// surfaced on the Windows build.
struct GUIMouseEvent;
struct GUIKeyboardEvent;

class GUIResponder : public boost::enable_shared_from_this<GUIResponder> {
public:
    // Every base implementation forwards the event to nextResponder()
    // when one is set (see GUIResponder.cpp for the per-method evidence).
    virtual void TouchBegan(const FWTouch& touch);              // slot 0
    virtual void TouchMoved(const FWTouch& touch);              // slot 1
    virtual void TouchEnded(const FWTouch& touch);              // slot 2
    virtual void TouchCancelled(const FWTouch& touch);          // slot 3
    virtual void HandleMouseEvent(const GUIMouseEvent& event);  // slot 4
    virtual void HandleKeyboardEvent(const GUIKeyboardEvent& event); // slot 5
    virtual GUIResponder* nextResponder() const;                // slot 6

    // +24. Member name unrecovered; the offset is proven by
    // nextResponder() and by the forwarding methods.
    GUIResponder* m_nextResponder;
};

} // namespace Caver

#endif
