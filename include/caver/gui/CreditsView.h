// Recovered from libswordigo.so (1.4.13) evidence — IDA/Hex-Rays
// exports in ~/workspace/analysis/swordigo/functions/Caver/CreditsView/,
// the golden vtable @0x62CA40 and the ctor/dtor bodies.
//
// CreditsView: the credits screen presented from the main menu. A
// GUIView rendering the "credits" scene (a SceneView over the scene
// loaded from PathForResourceOfType("credits", "scene")) with a
// letterbox GUIEffectView over it, plus a flipped back-arrow button
// that the CreditsViewController wires to dismiss the screen.
//
// RTTI @0x62CB80 is a __si_class_type_info whose single base is
// GUIView (@0x635630) — CHECKED via the relocation at vtable+0x140
// (typeinfo ptr) and the D2's tail to ~GUIView. Vtable 320 bytes =
// the 38 GUIView slots with these overrides only:
//   5  HandleKeyboardEvent (@0x456488)
//   7/8 ~CreditsView D2 (@0x4564BC) / D0 (@0x456594) — both WEAK,
//       the implicit-destructor shape (no user dtor declared)
//   11 LayoutSubviews (@0x45630C)
//   16 DidMoveToWindow (@0x456674, WEAK)
//   24 CanBecomeFirstResponder (@0x4566B8, WEAK, returns true)
// AnimateIn (@0x456388) is NON-virtual (no vtable slot).
//
// Layout (sizeof = 288 = 0x120, proven by the ctor's zero stores at
// +256..+287, the D2 teardown releasing the control blocks at +280
// then +264, and CreditsViewController::LoadView's operator
// new(0x120) for this class):
//   +0..255  GUIView base
//   +256     boost::shared_ptr<SceneView> — the credits scene view
//   +272     boost::shared_ptr<GUIButton> — back button
//            (GameInterfaceBuilder::PlainButton with null textures,
//            then SetImage "ui_button_arrow" / SetHighlightedImage
//            "ui_button_arrow_pressed", frame size 80x50, tint colour
//            dword 0xB2B2B2B2, and its field336 matrix set to
//            diag(-1, 1, 1, 1) so the arrow points left)
// Member names are UNRECOVERED; the roles above are proven by the
// ctor / LayoutSubviews / AnimateIn / HandleKeyboardEvent bodies.
#ifndef CAVER_CREDITSVIEW_H
#define CAVER_CREDITSVIEW_H

#include <boost/shared_ptr.hpp>

#include "GUIView.h"

namespace Caver {

class GUIButton;
class SceneView;

class CreditsView : public GUIView {
public:
    CreditsView();

    // No user-declared destructor: the golden D2 @0x4564BC (216
    // bytes, WEAK) / D0 @0x456594 (224 bytes, WEAK) pair is the
    // implicit-destructor shape — the two member shared_ptrs are
    // released in reverse declaration order, then ~GUIView.

    // Declaration order note: LayoutSubviews is declared first so it
    // is the class key function and the vtable (with the weak
    // implicit dtors) is emitted from CreditsView.cpp.
    void LayoutSubviews() override;  // slot 11

    void HandleKeyboardEvent(const GUIKeyboardEvent& event) override;  // slot 5
    void DidMoveToWindow() override;                                   // slot 16
    bool CanBecomeFirstResponder() override;                           // slot 24

    // Non-virtual: fades the back button in (AlphaAnimation
    // 0 -> 1 over 0.4, delay 0.3).
    void AnimateIn();

    boost::shared_ptr<SceneView> m_sceneView;   // +256 name UNRECOVERED
    boost::shared_ptr<GUIButton> m_backButton;  // +272 name UNRECOVERED
};

} // namespace Caver

#endif
