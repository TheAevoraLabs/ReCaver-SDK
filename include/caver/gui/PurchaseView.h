// Recovered from libswordigo.so (1.4.13) evidence — IDA/Hex-Rays
// exports in ~/workspace/analysis/swordigo/functions/Caver/
// PurchaseView/, the ABS64 relocations of the golden vtable, and the
// ctor @0x4978FC / dtor @0x499CB8 bodies.
//
// PurchaseView: the generic store purchase panel — a title label, a
// vector of description labels (populated by SetProductInfo), a
// framed product image view, a buy button and a close button. It is
// the single-product sibling of GuidePurchaseView (same texture names
// "ui_guide_buy"/"ui_guide_buy_pressed", same layout math), driven by
// PurchaseViewController.
//
// Vtable _ZTVN5Caver12PurchaseViewE @0x62F7E0, 320 bytes = the 38
// GUIView slots; RTTI @0x62F920 is a __si_class_type_info whose single
// base is GUIView (@0x635630). Overrides only:
//   5    HandleKeyboardEvent @0x499C84
//   7/8  ~PurchaseView (D1/D2 @0x499CB8, D0 @0x499ECC)
//   11   LayoutSubviews @0x4988C0
//   16   DidMoveToWindow @0x499EF0
//   24   CanBecomeFirstResponder @0x499F34 (returns true)
// All other slots keep the GUIView/GUIResponder functions.
//
// Layout (sizeof = 344 = 0x158, proven by the ctor's zero stores over
// +256..+343 and the dtor's reverse-order teardown):
//   +0..255  GUIView base
//   +256     shared_ptr<GUILabel> title label (created empty by the
//            ctor, text set by SetProductInfo)
//   +272     vector<shared_ptr<GUILabel>> description labels (built
//            by SetProductInfo, one per description string)
//   +296     shared_ptr<GUIFrameView> product image frame view
//   +312     shared_ptr<GUIButton> buy button
//   +328     shared_ptr<GUIButton> close button
// Member NAMES are UNRECOVERED; the names below are descriptive.
#ifndef CAVER_PURCHASEVIEW_H
#define CAVER_PURCHASEVIEW_H

#include <boost/shared_ptr.hpp>
#include <string>
#include <vector>

#include "GUIEventTypes.h"
#include "GUIView.h"

namespace Caver {

class GUIButton;
class GUIFrameView;
class GUILabel;

class PurchaseView : public GUIView {
public:
    PurchaseView();
    ~PurchaseView() override;  // slots 7, 8

    void HandleKeyboardEvent(const GUIKeyboardEvent& event) override;  // slot 5
    void LayoutSubviews() override;                                    // slot 11
    void DidMoveToWindow() override;                                   // slot 16
    bool CanBecomeFirstResponder() override;                           // slot 24

    // NOTE: the descriptions vector is passed BY VALUE in the
    // golden (proven by the mangled signature — no RK on the
    // vector parameter).
    void SetProductInfo(const std::string& title,
                        std::vector<std::string> descriptions,
                        const std::string& imageName, float imageScale);
    void UpdateBuyButtonSize();
    void SetBuyButtonEnabled(bool enabled);
    void SetBuyButtonTitle(const std::string& title);
    void AnimateIn();
    void AnimateOut();

    boost::shared_ptr<GUILabel> titleLabel;  // +256 UNRECOVERED name
    std::vector<boost::shared_ptr<GUILabel> >
        descriptionLabels;  // +272 UNRECOVERED name
    boost::shared_ptr<GUIFrameView> frameView;  // +296 UNRECOVERED name
    boost::shared_ptr<GUIButton> buyButton;     // +312 UNRECOVERED name
    boost::shared_ptr<GUIButton> closeButton;   // +328 UNRECOVERED name
};

} // namespace Caver

#endif
