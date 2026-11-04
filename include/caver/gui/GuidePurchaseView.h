// Recovered from libswordigo.so (1.4.13) evidence — IDA/Hex-Rays
// exports in ~/workspace/analysis/swordigo/functions/Caver/
// GuidePurchaseView/, the ABS64 relocations of the golden vtable, the
// ctor @0x4ABB2C / dtor @0x4AEDE4 bodies, and the allocation site in
// GuidePurchaseViewController::LoadView (operator new(0x180)).
//
// GuidePurchaseView: the store panel for the "Guiding Compass" guide
// purchase — a title label, three description labels, a screenshot
// frame view, buy / close / restore buttons and a restore hint label.
// Button presses are routed by ButtonPressed to the delegate (the
// GuidePurchaseViewController, which implements
// IGuidePurchaseViewDelegate with exactly these three slots, in this
// order: Close, Buy, Restore).
//
// Vtable _ZTVN5Caver17GuidePurchaseViewE @0x6306F0, 320 bytes = the
// 38 GUIView slots; RTTI @0x630830 is a __si_class_type_info whose
// single base is GUIView. Overrides only:
//   7/8  ~GuidePurchaseView (D1/D2 @0x4AEDE4, D0 @0x4AF0A0)
//   10   DrawRect @0x4AED38
//   11   LayoutSubviews @0x4AD588
// All other slots keep the GUIView/GUIResponder functions.
//
// Layout (sizeof = 384 = 0x180, proven by the operator new(0x180) in
// GuidePurchaseViewController::LoadView, the ctor's zero stores over
// +256..+383 and the dtor's reverse-order teardown):
//   +0..255  GUIView base
//   +256     IGuidePurchaseViewDelegate* delegate (raw, non-owning;
//            zeroed by the ctor, never written inside this class —
//            the owning controller installs itself)
//   +264     shared_ptr<GUILabel> title label ("Guiding Compass")
//   +280     vector<shared_ptr<GUILabel>> description labels (3,
//            built by the ctor from a stack string array)
//   +304     shared_ptr<GUIFrameView> screenshot frame view
//   +320     shared_ptr<GUIButton> buy button
//   +336     shared_ptr<GUIButton> close button
//   +352     shared_ptr<GUIButton> restore button (starts hidden)
//   +368     shared_ptr<GUILabel> restore label (starts hidden)
// Member NAMES are UNRECOVERED; the names below are descriptive.
#ifndef CAVER_GUIDEPURCHASEVIEW_H
#define CAVER_GUIDEPURCHASEVIEW_H

#include <boost/shared_ptr.hpp>
#include <vector>

#include "GUIView.h"

namespace Caver {

class GUIButton;
class GUIEvent;
class GUIFrameView;
class GUILabel;
class IGuidePurchaseViewDelegate;

class GuidePurchaseView : public GUIView {
public:
    GuidePurchaseView();
    ~GuidePurchaseView() override;  // slots 7, 8

    void DrawRect(RenderingContext* context, const Rectangle& rect,
                  const Matrix4& transform) override;  // slot 10
    void LayoutSubviews() override;                    // slot 11

    void AnimateIn();
    void AnimateOut();
    void UpdateBuyButtonSize();
    void ButtonPressed(void* sender, GUIEvent* event);

    IGuidePurchaseViewDelegate* delegate;  // +256 UNRECOVERED name
    boost::shared_ptr<GUILabel> titleLabel;  // +264 UNRECOVERED name
    std::vector<boost::shared_ptr<GUILabel> >
        descriptionLabels;  // +280 UNRECOVERED name
    boost::shared_ptr<GUIFrameView> frameView;  // +304 UNRECOVERED name
    boost::shared_ptr<GUIButton> buyButton;     // +320 UNRECOVERED name
    boost::shared_ptr<GUIButton> closeButton;   // +336 UNRECOVERED name
    boost::shared_ptr<GUIButton> restoreButton;  // +352 UNRECOVERED name
    boost::shared_ptr<GUILabel> restoreLabel;   // +368 UNRECOVERED name
};

} // namespace Caver

#endif
