// Recovered from libswordigo.so (1.4.13) evidence — IDA/Hex-Rays
// exports in ~/workspace/analysis/swordigo/functions/Caver/StoreView/
// plus the Ghidra dump arbitration (~/workspace/analysis/ghidra64/).
//
// StoreView: the in-app-purchase store panel built by
// StoreViewController::LoadView. A GUIView (sizeof base 256) holding a
// close button, two StripeView title stripes, a restore-purchases
// button, a GUIScrollView and the vector of StoreProductView rows
// managed by SetProductCount.
//
// Layout (sizeof = 360, proven by the ctor @0x49DADC zero-store map
// (own members zeroed +256..+359), the dtor @0x49F200 member-release
// order (vector first, then the shared_ptrs in reverse declaration
// order) and every method body's accesses):
//   +256     shared_ptr<GUIButton> close button (IconButton with the
//            "ui_button_close" / "ui_button_close_pressed" textures;
//            HandleKeyboardEvent fires its actions, AnimateOut scales
//            it out)
//   +272     shared_ptr<StripeView> "Offers" stripe (SetHasTag(true,
//            0), SetTitle("Offers"); LayoutSubviews centres the close
//            button on it)
//   +288     shared_ptr<StripeView> second stripe (SetTitle(""), its
//            title label is recoloured to (200,180,160,255) in the
//            ctor; LayoutSubviews centres the restore button on it)
//   +304     shared_ptr<GUIButton> restore button
//            (FramedButtonWithTitle("Restore Purchases", false); NOT
//            added as a subview by the ctor — bug-for-bug)
//   +320     shared_ptr<GUIScrollView> scroll view (added first in
//            the ctor; SetProductCount adds product rows to it)
//   +336     vector<shared_ptr<StoreProductView>> product rows
// Member NAMES are UNRECOVERED; the names below are descriptive.
//
// Vtable: GUIView's 38 slots with overrides at slot 5
// (HandleKeyboardEvent), 7/8 (dtors), 11 (LayoutSubviews), 16
// (DidMoveToWindow) and 24 (CanBecomeFirstResponder).
#ifndef CAVER_STOREVIEW_H
#define CAVER_STOREVIEW_H

#include <boost/shared_ptr.hpp>
#include <vector>

#include "GUIView.h"

namespace Caver {

class GUIButton;
class GUIScrollView;
class StoreProductView;
class StripeView;

class StoreView : public GUIView {
public:
    StoreView();
    ~StoreView() override;  // slots 7, 8

    void HandleKeyboardEvent(const GUIKeyboardEvent& event) override;  // slot 5
    void LayoutSubviews() override;                                    // slot 11
    void DidMoveToWindow() override;                                   // slot 16
    bool CanBecomeFirstResponder() override;                           // slot 24

    void SetProductCount(int count);
    void AnimateIn();
    void AnimateOut();

    boost::shared_ptr<GUIButton> closeButton;    // +256 UNRECOVERED name
    boost::shared_ptr<StripeView> offersStripe;  // +272 UNRECOVERED name
    boost::shared_ptr<StripeView> infoStripe;    // +288 UNRECOVERED name
    boost::shared_ptr<GUIButton> restoreButton;  // +304 UNRECOVERED name
    boost::shared_ptr<GUIScrollView> scrollView; // +320 UNRECOVERED name
    std::vector<boost::shared_ptr<StoreProductView> >
        productViews;  // +336 UNRECOVERED name
};

} // namespace Caver

#endif
