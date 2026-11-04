// Recovered from libswordigo.so (1.4.13) evidence — IDA/Hex-Rays
// exports in ~/workspace/analysis/swordigo/functions/Caver/LevelUpView/,
// the golden vtable _ZTVN5Caver11LevelUpViewE @0x631528 (320 bytes =
// 16-byte header + exactly the 38 GUIView slots) and the RTTI
// _ZTIN5Caver11LevelUpViewE @0x631668 (__si_class_type_info whose
// single base is GUIView @0x635630).
//
// LevelUpView: the modal "level up" banner presented over the game —
// a close button plus one "ui_xp_frame" GUIFrameView per text line
// ("Experience Level N Achieved!", "Maximum Health Increased",
// "Damage Increased"), each frame holding a centred GUILabel.
//
// Base (binary-proven): GUIView DIRECTLY — not MenuViewPage. The
// ctor @0x4BDE94 calls GUIView::GUIView (no page-title string store
// anywhere), the vtable has no slot 38, and Dismiss is a plain
// non-virtual method (it appears in no vtable). Vtable overrides:
//   7/8 ~LevelUpView, 9 Update (@0x4BED68, a 4-byte tail branch to
//   GUIView::Update), 10 DrawRect (@0x4BED6C, same shape), 11
//   LayoutSubviews (@0x4BEC04).
//
// Layout (sizeof 312, byte-verified against the ctor @0x4BDE94
// zero-store map, the dtor @0x4BEDAC teardown order — exact reverse
// declaration order — and every method body's accesses):
//   +0..255  GUIView base
//   +256     ILevelUpViewDelegate* delegate (raw, non-owning; the
//            ctor zeroes it; Dismiss and ButtonPressed call its
//            slot 0 with this view before DismissModalView(0)).
//            No in-binary presenter exists: the ctor, Init and
//            Dismiss have zero callers in the golden (no PLT slot,
//            no direct call in a full disassembly) — the class is
//            orphaned in 1.4.13, so who was meant to write this
//            pointer is UNRECOVERED, as is the delegate method's
//            name.
//   +264     boost::shared_ptr<GameState> gameState
//   +280     boost::shared_ptr<GUIButton> close button (the
//            "ui_close_button" GUIButton built by InitWithGameState;
//            ButtonPressed compares the sender against it)
//   +296     int count of text lines / frame views built by
//            InitWithGameState (member name UNRECOVERED). NOT
//            written by the ctor — the golden zero map skips +296,
//            so it stays uninitialised until Init (bug-for-bug).
//   +304     boost::scoped_array<boost::shared_ptr<GUIFrameView> >
//            the per-line frame views (member name UNRECOVERED)
#ifndef CAVER_LEVELUPVIEW_H
#define CAVER_LEVELUPVIEW_H

#include <boost/scoped_array.hpp>
#include <boost/shared_ptr.hpp>

#include "GUIView.h"

namespace Caver {

class GameState;
class GUIButton;
class GUIEvent;
class GUIFrameView;
class LevelUpView;
class RenderingContext;

// Delegate notified (slot 0) just before the view dismisses itself
// as a modal. Interface and method names UNRECOVERED (see header
// note); only the slot and the (delegate, view) argument pair are
// proven by the Dismiss @0x4BED70 / ButtonPressed @0x4BEBB4 bodies.
class ILevelUpViewDelegate {
public:
    virtual void LevelUpViewDismissed(LevelUpView* view) = 0;  // slot 0
};

class LevelUpView : public GUIView {
public:
    LevelUpView();
    ~LevelUpView() override;  // slots 7, 8

    void InitWithGameState(const boost::shared_ptr<GameState>& gameState);

    void ButtonPressed(void* sender, GUIEvent* event);
    void Dismiss();

    void Update(float deltaTime) override;  // slot 9
    void DrawRect(RenderingContext* context, const Rectangle& rect,
                  const Matrix4& transform) override;  // slot 10
    void LayoutSubviews() override;                    // slot 11

    ILevelUpViewDelegate* delegate;      // +256 (see note)
    boost::shared_ptr<GameState> gameState;  // +264
    boost::shared_ptr<GUIButton> closeButton;  // +280
    int frameViewCount;                  // +296 (name UNRECOVERED)
    boost::scoped_array<boost::shared_ptr<GUIFrameView> > frameViews;  // +304 (name UNRECOVERED)
};

// Golden aarch64 NDK-libc++ layout anchor; MSVC STL member sizes differ,
// so this assert is Android/Linux-only (the Windows build is self-consistent
// under MSVC STL and does not claim golden byte-layout parity).
#ifndef _MSC_VER
static_assert(sizeof(LevelUpView) == 312,
              "LevelUpView layout: ctor/dtor store maps, golden");
#endif

} // namespace Caver

#endif
