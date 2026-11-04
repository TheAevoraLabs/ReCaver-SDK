// Recovered from libswordigo.so (1.4.13) evidence — IDA/Hex-Rays
// exports in ~/workspace/analysis/swordigo/functions/Caver/
// StoreProductView/, the golden vtable relocations, and the
// StoreView ctor's operator new(0x160) for a product row.
//
// StoreProductView: one row of the in-app-purchase store panel
// (owned by StoreView::productViews, built by
// StoreView::SetProductCount). A GUIView (sizeof base 256) holding
// a frame backdrop, a buy button, a 70x70 icon frame, a container
// view holding two labels, in that declaration order.
//
// Layout (sizeof = 352, proven by the ctor @0x49C85C zero-store map
// (own members zeroed +256..+351), the dtor @0x49D890 member-release
// order (reverse declaration order, six shared_ptrs) and every
// method body's accesses):
//   +256     shared_ptr<GUIFrameView> frame backdrop
//            (texture "ui_frame_dark", not tiled; SetFrame(bounds)
//            in LayoutSubviews)
//   +272     shared_ptr<GUIButton> buy button (PlainButton with the
//            "ui_guide_buy" / "ui_guide_buy_pressed" textures,
//            title "Loading...", frame width forced to 144)
//   +288     shared_ptr<GUIFrameView> icon frame (no texture set by
//            the ctor; LayoutSubviews sizes it 70x70 at x=7)
//   +304     shared_ptr<GUIView> labels container (the two labels
//            are added to it, not to this view)
//   +320     shared_ptr<GUILabel> upper label (NormalLabel("", white,
//            black); laid out BELOW the +336 label)
//   +336     shared_ptr<GUILabel> lower label (NormalLabel("", white,
//            black), font swapped to SmallDefaultFont, text colour
//            (200,180,160,255); laid out at the container top, with
//            field348 = 2 and field344 = true in LayoutSubviews)
// Member NAMES are UNRECOVERED; the names below are descriptive.
// The +320/+336 labels' roles (title vs price/description) are
// UNRECOVERED from the view bodies alone — StoreViewController
// sets their texts, which is that slice's evidence.
//
// Vtable _ZTVN5Caver16StoreProductViewE @0x62FA38, 320 bytes = the
// 38 GUIView slots with only slot 7/8 (~StoreProductView) and slot
// 11 (LayoutSubviews) overridden (ABS64 relocation dump). RTTI
// @0x62FB78 is a __si_class_type_info on GUIView, so the base is
// GUIView (not a plain GUIResponder).
#ifndef CAVER_STOREPRODUCTVIEW_H
#define CAVER_STOREPRODUCTVIEW_H

#include <boost/shared_ptr.hpp>

#include "GUIView.h"

namespace Caver {

class GUIButton;
class GUIFrameView;
class GUILabel;

class StoreProductView : public GUIView {
public:
    StoreProductView();
    ~StoreProductView() override;  // slots 7, 8

    void LayoutSubviews() override;  // slot 11

    void SetBuyButtonEnabled(bool enabled);

    boost::shared_ptr<GUIFrameView> frameView;     // +256 UNRECOVERED name
    boost::shared_ptr<GUIButton> buyButton;        // +272 UNRECOVERED name
    boost::shared_ptr<GUIFrameView> iconFrameView; // +288 UNRECOVERED name
    boost::shared_ptr<GUIView> labelsView;         // +304 UNRECOVERED name
    boost::shared_ptr<GUILabel> mainLabel;         // +320 UNRECOVERED name
    boost::shared_ptr<GUILabel> detailLabel;       // +336 UNRECOVERED name
};

} // namespace Caver

#endif
