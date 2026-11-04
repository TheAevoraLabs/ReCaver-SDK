// Recovered from libswordigo.so (1.4.13) evidence — IDA/Hex-Rays
// exports in ~/workspace/analysis/swordigo/functions/Caver/StripeView/,
// the golden disassembly, and the ABS64 relocations of the golden
// vtable / RTTI.
//
// StripeView: a framed menu stripe (title bar) with an optional tag
// badge sub-stripe pinned to its left and a centred title label.
//
// Derivation (binary-proven): the RTTI @0x632AF0 is a
// __si_class_type_info whose single base (relocation at +16) is
// _ZTIN5Caver12GUIFrameViewE, so StripeView IS-A GUIFrameView — this
// supersedes the PortalView/SceneLoadingView stubs that modelled it
// as a plain GUIView subclass, and agrees with the StoreView stub.
// The golden vtable _ZTVN5Caver10StripeViewE @0x6329B0 is 320 bytes
// (38 slots, same as GUIView): slot 7/8 are ~StripeView, slot 10 is
// inherited GUIFrameView::DrawRect, slot 11 is StripeView's own
// LayoutSubviews; all other slots keep the GUIView/GUIResponder
// functions.
//
// Layout: GUIFrameView (288 bytes), then
//   +288 boost::shared_ptr<GUIFrameView> tagView — the tag badge
//        sub-stripe created by SetHasTag (name UNRECOVERED; its type
//        is proven by the ctor's exception cleanup, which destroys
//        +288 as a shared_ptr<GUIFrameView>, and by SetHasTag's
//        operator new(0x120) + GUIFrameView ctor)
//   +304 boost::shared_ptr<GUILabel> titleLabel — created lazily by
//        SetTitle via GameInterfaceBuilder::NormalLabel
//   +320 float field320 — SetHasTag's float argument; used by
//        LayoutSubviews as the tag frame's x and as the base of the
//        title frame's x (name UNRECOVERED)
//   +324 float titleHorizOffset — written by SetTitleHorizOffset
// sizeof 328 (0x148), proven by the operator new(0x148) in the
// StoreView / PortalView / SceneLoadingView ctors.
#ifndef CAVER_STRIPEVIEW_H
#define CAVER_STRIPEVIEW_H

#include <boost/shared_ptr.hpp>
#include <string>

#include "GUIFrameView.h"

namespace Caver {

class GUILabel;

class StripeView : public GUIFrameView {
public:
    StripeView();
    ~StripeView() override;  // slots 7, 8

    void LayoutSubviews() override;  // slot 11

    void SetHasTag(bool hasTag, float offset);
    void SetTitle(const std::string& title);
    void SetTitleHorizOffset(float offset);

    boost::shared_ptr<GUIFrameView> tagView;     // +288
    boost::shared_ptr<GUILabel> titleLabel;      // +304
    float field320 = 0.0f;                       // +320 (name UNRECOVERED;
                                                 // ctor zeroes +320..327
                                                 // with one 8-byte store)
    float titleHorizOffset = 0.0f;               // +324
};

} // namespace Caver

#endif
