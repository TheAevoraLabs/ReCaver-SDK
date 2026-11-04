// Recovered from libswordigo.so (1.4.13) evidence — IDA/Hex-Rays
// exports in ~/workspace/analysis/swordigo/functions/Caver/
// PauseView/, the ABS64 relocations of the golden vtable, and the
// ctor @0x4CADF0 / dtor @0x4CB3AC bodies.
//
// PauseView: the pause panel PauseViewController presents — a
// GUIView holding a single centred "Tap to continue" label.
// Tapping (or pressing a continue key) notifies the delegate's
// PauseViewDidContinue; the Escape-style key (33) quits the app
// through AndroidQuitApplication instead.
//
// Vtable _ZTVN5Caver9PauseViewE @0x632120, 320 bytes = the 38
// GUIView slots; RTTI @0x632260 is a __si_class_type_info whose
// single base is GUIView. Overrides only (ABS64 relocations of
// slots 2, 5, 7/8, 11, 16, 24):
//   2    TouchEnded @0x4CB1A8
//   5    HandleKeyboardEvent @0x4CB350
//   7/8  ~PauseView (D2 @0x4CB3AC, D0 @0x4CB430 — both WEAK, the
//        implicit-destructor shape: D0 is D2 plus the operator
//        delete tail, so no destructor is declared here)
//   11   LayoutSubviews @0x4CB124
//   16   DidMoveToWindow @0x4CB4BC
//   24   CanBecomeFirstResponder @0x4CB500 (returns true)
// All other slots keep the GUIView/GUIResponder functions.
//
// Layout (sizeof = 280 = 0x118, proven by the operator new(0x118)
// in PauseViewController::LoadView @0x442D48, the ctor's zero
// stores and the dtor's single member release):
//   +0..255  GUIView base
//   +256     IPauseViewDelegate* — raw, non-owning; LoadView
//            writes the controller's delegate subobject
//            (controller +72) here. Name UNRECOVERED.
//   +264     boost::shared_ptr<GUILabel> — the "Tap to continue"
//            label built by the ctor. Name UNRECOVERED.
#ifndef CAVER_PAUSEVIEW_H
#define CAVER_PAUSEVIEW_H

#include <boost/shared_ptr.hpp>

#include "GUIView.h"

namespace Caver {

class GUILabel;
class IPauseViewDelegate;

class PauseView : public GUIView {
public:
    PauseView();
    // No user-declared destructor: the golden D2 @0x4CB3AC (132
    // bytes, WEAK) / D0 @0x4CB430 pair is the implicit-destructor
    // shape.

    // Declaration order note: LayoutSubviews is declared first so
    // it is the class key function — TouchEnded's body lives in the
    // asm file, and the vtable (with the weak implicit dtors) must
    // be emitted from PauseView.cpp. Override slot order is
    // inherited from GUIView and does not follow declaration order
    // here.
    void LayoutSubviews() override;                                 // slot 11
    void TouchEnded(const FWTouch& touch) override;  // slot 2 — body in
        // src/asm/PauseViewTouchEnded.s (see that file's header)
    void HandleKeyboardEvent(const GUIKeyboardEvent& event) override;  // slot 5
    void DidMoveToWindow() override;                                // slot 16
    bool CanBecomeFirstResponder() override;                        // slot 24

    IPauseViewDelegate* m_delegate;        // +256 name UNRECOVERED
    boost::shared_ptr<GUILabel> m_label;   // +264 name UNRECOVERED
};

} // namespace Caver

#endif
