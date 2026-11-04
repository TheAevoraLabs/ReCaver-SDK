// Recovered from libswordigo.so (1.4.13) evidence — IDA/Hex-Rays
// exports in ~/workspace/analysis/swordigo/functions/Caver/NewMenuView/,
// the golden vtable _ZTVN5Caver11NewMenuViewE @0x631E60 (320 bytes,
// ABS64 relocations) and the RTTI _ZTIN5Caver11NewMenuViewE @0x631FA0.
//
// NewMenuView: the in-game menu tab bar — a row of four GUIButtons
// with "ui_tab_<name>_selected" textures ("char", "map", "inventory"
// and "inventory" again — the duplicated name is in the golden ctor
// @0x4C96D8, reproduced bug-for-bug), laid out centred in
// LayoutSubviews and popped in with a staggered scale animation by
// PresentFromRectangle.
//
// Derivation (binary-proven): the RTTI is a __si_class_type_info
// whose single base is Caver::GUIView (@0x631FB0 -> _ZTI GUIView) —
// NewMenuView derives DIRECTLY from GUIView, NOT from MenuViewPage
// (no page-title string; the vtable has exactly the 38 GUIView slots
// and Dismiss is a plain non-virtual method that tail-calls the
// slot-15 RemoveFromSuperview through the vtable).
//
// Primary vtable overrides only: slot 0 TouchBegan, slots 7/8 the
// destructors, slot 9 Update and slot 10 DrawRect (both 4-byte tail
// thunks to the GUIView bodies), slot 11 LayoutSubviews. All other
// slots keep the GUIView/GUIResponder functions.
//
// Layout (sizeof 288, byte-verified against the ctor's zero stores
// at +264/+272/+280, the dtor @0x4CA1F8 vector teardown at
// +264/+272, and every method body's accesses):
//   +0..255  GUIView base
//   +256     INewMenuViewDelegate* delegate (raw, non-owning; the
//            ctor NEVER initialises it — bug-for-bug, the presenting
//            code is expected to set it after construction; member
//            name UNRECOVERED)
//   +264     std::vector<boost::shared_ptr<GUIButton> > buttons
//            (the four tab buttons, in ctor order; member name
//            UNRECOVERED)
#ifndef CAVER_NEWMENUVIEW_H
#define CAVER_NEWMENUVIEW_H

#include <boost/shared_ptr.hpp>
#include <vector>

#include "GUIView.h"

namespace Caver {

class GUIButton;
class GUIEvent;
class NewMenuView;

// Delegate interface (bare vptr, no destructor — the two slots are
// proven by the call sites: ButtonPressed @0x4C9BC8 calls slot 0 with
// (this view, pressed button index) after matching the sender against
// the button vector; TouchBegan @0x4CA1B4 calls slot 1 with (this
// view), then clears the pointer. Method names UNRECOVERED.)
class INewMenuViewDelegate {
public:
    virtual void NewMenuViewButtonPressed(NewMenuView* view,
                                          int buttonIndex) = 0;  // slot 0
    virtual void NewMenuViewTouchBegan(NewMenuView* view) = 0;     // slot 1
};

class NewMenuView : public GUIView {
public:
    NewMenuView();
    ~NewMenuView() override;  // slots 7, 8

    void TouchBegan(const FWTouch& touch) override;  // slot 0
    void Update(float deltaTime) override;           // slot 9 (forwarder)
    void DrawRect(RenderingContext* context, const Rectangle& rect,
                  const Matrix4& transform) override;  // slot 10 (forwarder)
    void LayoutSubviews() override;                      // slot 11

    void ButtonPressed(void* sender, GUIEvent* event);
    void PresentFromRectangle(const Rectangle& rect);
    void Dismiss();

    INewMenuViewDelegate* delegate;  // +256 (name UNRECOVERED)
    std::vector<boost::shared_ptr<GUIButton> > buttons;  // +264 (name UNRECOVERED)
};

// Golden aarch64 NDK-libc++ layout anchor; MSVC STL member sizes differ,
// so this assert is Android/Linux-only (the Windows build is self-consistent
// under MSVC STL and does not claim golden byte-layout parity).
#ifndef _MSC_VER
static_assert(sizeof(NewMenuView) == 288,
              "NewMenuView layout: ctor/dtor store maps, golden");
#endif

} // namespace Caver

#endif
