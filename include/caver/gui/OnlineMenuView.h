// Recovered from libswordigo.so (1.4.13) evidence — IDA/Hex-Rays
// exports in ~/workspace/analysis/swordigo/functions/Caver/
// OnlineMenuView/, the golden vtable @0x62CF20 and RTTI @0x62D060.
//
// OnlineMenuView: the "online" submenu of the main menu (the
// Achievements / Leaderboards panel with an arrow close button).
// RTTI @0x62D060 is a __si_class_type_info whose single base is
// Caver::GUIView (@0x635630) — a direct GUIView subclass, NOT a
// GUIFrameView. Vtable @0x62CF20, 320 bytes = the 38 GUIView slots
// with only these overrides (ABS64 relocation dump):
//   7/8  ~OnlineMenuView D1/D0 (@0x45DD2C / @0x45DF40)
//   11   LayoutSubviews (@0x45D724)
// All other slots keep the GUIView/GUIResponder functions; the class
// adds NO new virtual slots. The golden D1/D2 are WEAK, so the
// destructor is implicit here too (its body is exactly the member
// teardown below, in reverse declaration order, plus ~GUIView).
//
// Layout (sizeof = 344 = 0x158, proven by the operator new(0x158)
// in MainMenuView's ctor, by MainMenuView's SetOnlineMenuVisible
// reading the close button pointer at +328, and by the D2 teardown
// @0x45DD2C, which releases the shared_ptr control blocks at
// +336, +320, +304, +288 and then destroys the vector at +256):
//   +256 std::vector<shared_ptr<GUIButton>> — every button, in
//        creation order; AnimateIn/AnimateOut/LayoutSubviews walk
//        this vector (indexed loop, begin/end reloaded per step)
//   +280 shared_ptr<GUIButton> — "Achievements" (MainMenuButtonWith-
//        Title); ButtonPressed maps it to OnlineController vtable
//        slot 5 (LaunchAchievements)
//   +296 shared_ptr<GUIButton> — "Leaderboards"; ButtonPressed maps
//        it to slot 6 (LaunchLeaderboards)
//   +312 shared_ptr<GUIButton> — NEVER assigned or read by any
//        OnlineMenuView body (the ctor's member-init zero stores
//        leave it null; role/name UNRECOVERED)
//   +328 shared_ptr<GUIButton> — the close button: MainMenuButton-
//        WithTitle(""), frame widened by 15 in the ctor, images
//        "ui_button_arrow" / "ui_button_arrow_pressed", and its
//        GUIButton image matrix (field336) set to a 0.8 scale.
//        MainMenuView::ButtonPressed compares its sender against
//        the raw pointer at +328 of this object.
#ifndef CAVER_ONLINEMENUVIEW_H
#define CAVER_ONLINEMENUVIEW_H

#include <boost/shared_ptr.hpp>
#include <vector>

#include "GUIView.h"

namespace Caver {

class GUIButton;
class GUIEvent;

class OnlineMenuView : public GUIView {
public:
    OnlineMenuView();

    void LayoutSubviews() override;  // slot 11

    // Non-virtual members (ButtonPressed is the boost::bind target
    // registered for every button in the ctor).
    void ButtonPressed(void* sender, GUIEvent* event);
    void AnimateIn();
    void AnimateOut();

    std::vector<boost::shared_ptr<GUIButton> > m_buttons;   // +256
    boost::shared_ptr<GUIButton> m_achievementsButton;      // +280
    boost::shared_ptr<GUIButton> m_leaderboardsButton;      // +296
    boost::shared_ptr<GUIButton> m_button312;               // +312
                                  // (never assigned — see header note)
    boost::shared_ptr<GUIButton> m_closeButton;             // +328
};

// Golden aarch64 NDK-libc++ layout anchor; MSVC STL member sizes differ,
// so this assert is Android/Linux-only (the Windows build is self-consistent
// under MSVC STL and does not claim golden byte-layout parity).
#ifndef _MSC_VER
static_assert(sizeof(OnlineMenuView) == 344,
              "OnlineMenuView layout: operator-new / D2 proven, "
              "ends at +344");
#endif

} // namespace Caver

#endif
