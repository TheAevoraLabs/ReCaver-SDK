// Recovered from libswordigo.so (1.4.13) evidence — IDA/Hex-Rays
// exports in ~/workspace/analysis/swordigo/functions/Caver/
// GameOverView/, the ABS64 relocations of the golden vtable, and the
// ctor @0x4AAF14 / dtor @0x4AB9D0 bodies.
//
// GameOverView: the game-over panel GameOverViewController presents —
// a GUIView holding a single centred "Game Over" label. Tapping (or
// pressing a continue key) forwards to the delegate's
// GameOverViewDidContinue.
//
// Vtable _ZTVN5Caver12GameOverViewE @0x630598, 320 bytes = the 38
// GUIView slots; RTTI @0x6306D8 is a __si_class_type_info whose single
// base is GUIView. Overrides only:
//   2    TouchEnded @0x4AB86C
//   5    HandleKeyboardEvent @0x4AB980
//   7/8  ~GameOverView (D2 @0x4AB9D0, D0 @0x4ABA54 — both WEAK, the
//        implicit-destructor shape: D0 is D2 plus the operator delete
//        tail, so no destructor is declared here)
//   11   LayoutSubviews @0x4AB7DC
//   16   DidMoveToWindow @0x4ABAE0
//   24   CanBecomeFirstResponder @0x4ABB24 (returns true)
// All other slots keep the GUIView/GUIResponder functions.
// AddAnimations(bool) @0x4AB39C is non-virtual.
//
// Layout (sizeof = 280 = 0x118, proven by the operator new(0x118) in
// GameOverViewController::LoadView, the ctor's zero stores and the
// dtor's single member release):
//   +0..255  GUIView base
//   +256     IGameOverViewDelegate* — raw, non-owning; LoadView
//            writes the controller's delegate subobject (controller
//            +72) here and GameOverViewDidContinue clears it. Name
//            UNRECOVERED.
//   +264     boost::shared_ptr<GUILabel> — the "Game Over" label
//            built by the ctor. Name UNRECOVERED.
#ifndef CAVER_GAMEOVERVIEW_H
#define CAVER_GAMEOVERVIEW_H

#include <boost/shared_ptr.hpp>

#include "GUIView.h"

namespace Caver {

class GUILabel;
class IGameOverViewDelegate;

class GameOverView : public GUIView {
public:
    GameOverView();
    // No user-declared destructor: the golden D2 @0x4AB9D0 (132 bytes,
    // WEAK) / D0 @0x4ABA54 pair is the implicit-destructor shape.

    // Declaration order note: LayoutSubviews is declared first so it
    // is the class key function — TouchEnded's body lives in the asm
    // file, and the vtable (with the weak implicit dtors) must be
    // emitted from GameOverView.cpp. Override slot order is inherited
    // from GUIView and does not follow declaration order here.
    void LayoutSubviews() override;                                 // slot 11
    void TouchEnded(const FWTouch& touch) override;  // slot 2 — body in
        // src/asm/GameOverViewTouchEnded.s (see that file's header)
    void HandleKeyboardEvent(const GUIKeyboardEvent& event) override;  // slot 5
    void DidMoveToWindow() override;                                // slot 16
    bool CanBecomeFirstResponder() override;                        // slot 24

    // Non-virtual: builds the label's frame animation (slide in from
    // the bottom edge, or the reversed dismiss variant).
    void AddAnimations(bool dismiss);

    IGameOverViewDelegate* m_delegate;        // +256 name UNRECOVERED
    boost::shared_ptr<GUILabel> m_label;      // +264 name UNRECOVERED
};

} // namespace Caver

#endif
