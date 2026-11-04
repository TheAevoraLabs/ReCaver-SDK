// Recovered from libswordigo.so (1.4.13) evidence — IDA/Hex-Rays exports
// in ~/workspace/analysis/swordigo/functions/Caver/
// GuidePurchaseViewController/ plus the golden vtables recovered from
// ABS64 relocations and the __vmi RTTI @0x62B360.
//
// GuidePurchaseViewController — vtable group @0x62B268, 216 bytes.
// Bases (RTTI order): GUIViewController (+0),
// IGuidePurchaseViewDelegate (+72), IGUIAlertViewDelegate (+80).
//
// Primary vtable (address point 0x62B278, 16 slots):
//   0 ~D2 (@0x43C94C, WEAK), 1 ~D0 (@0x43CA84, WEAK),
//   2 GUIViewController::NewLoadingView, 3 LoadView (@0x43AAB4),
//   4 GUIViewController::SuspendView, 5 ResumeView, 6 UnloadView,
//   7 ViewDidAppear, 8 ViewWillDisappear, 9 Update,
//   10 PresentModalViewController (all base, not overridden),
//   11 GuidePurchaseViewCloseButtonPressed (@0x43B44C),
//   12 GuidePurchaseViewBuyButtonPressed (@0x43B794),
//   13 GuidePurchaseViewRestoreButtonPressed (@0x43BFB8),
//   14 AlertViewClickedButtonAtIndex (@0x43C934),
//   15 AlertViewWasDismissed (@0x43C944)
// Secondary @+72 (IGuidePurchaseViewDelegate, address point
// 0x62B308): _ZThn72_ Close, _ZThn72_ Buy, _ZThn72_ Restore.
// Secondary @+80 (IGUIAlertViewDelegate, address point 0x62B330):
// _ZThn80_ AlertViewClickedButtonAtIndex, _ZThn80_
// AlertViewWasDismissed. Neither interface vtable has a dtor slot.
//
// Layout (sizeof = 144, from ctor @0x43AA64 and dtor @0x43C94C;
// member types identified by the purchase-flow bodies, second pass):
//   +0..71   GUIViewController base
//   +72      IGuidePurchaseViewDelegate vptr
//   +80      IGUIAlertViewDelegate vptr
//   +88      IGuidePurchaseViewControllerDelegate* (raw, non-owning;
//            zeroed by the ctor, never released by the dtor.
//            HandlePurchaseSuccess (@0x43C7D4) calls its slot 0 with
//            this controller after a successful purchase. The
//            concrete implementor and the interface/member names are
//            UNRECOVERED — the writing call site was not found in the
//            exports; the PurchaseViewController analogue (its +72
//            delegate, set by StoreViewController to its own
//            IPurchaseViewControllerDelegate subobject) shows the
//            same slot-0 (delegate, controller) call shape.)
//   +96      shared_ptr<GuidePurchaseView> — the view built by
//            LoadView (@0x43AAB4: operator new(0x180) + assign);
//            AnimateOut / the buy, close and restore buttons are all
//            reached through it. Name UNRECOVERED.
//   +112     shared_ptr<GUIAlertView> — assigned from
//            GameInterfaceBuilder::AlertView in the three completion
//            functions' store-error path. Name UNRECOVERED.
//   +128     shared_ptr<StoreProduct> — assigned in
//            ProductRequestCompletion from its product argument;
//            SetBuyButtonEnabled reads the string at StoreProduct+24
//            (the price text). Name UNRECOVERED.
// The dtor releases the three shared_ptrs in reverse order
// (+128, +112, +96) and then runs the base destructor — the
// compiler-generated body of an empty destructor.
#ifndef CAVER_GUIDEPURCHASEVIEWCONTROLLER_H
#define CAVER_GUIDEPURCHASEVIEWCONTROLLER_H

#include <string>
#include <vector>

#include <boost/shared_ptr.hpp>

#include "GUIViewController.h"

namespace Caver {

class GUIAlertView;
class GuidePurchaseView;
class StoreProduct;

class IGuidePurchaseViewDelegate {
public:
    virtual void GuidePurchaseViewCloseButtonPressed(GuidePurchaseView* view) = 0;   // slot 0
    virtual void GuidePurchaseViewBuyButtonPressed(GuidePurchaseView* view) = 0;     // slot 1
    virtual void GuidePurchaseViewRestoreButtonPressed(GuidePurchaseView* view) = 0; // slot 2
};

#ifndef CAVER_IGUIALERTVIEWDELEGATE_DEFINED
#define CAVER_IGUIALERTVIEWDELEGATE_DEFINED
class IGUIAlertViewDelegate {
public:
    // Returns bool: the golden body @0x43C934 is `mov w0,#1; ret`.
    virtual bool AlertViewClickedButtonAtIndex(GUIAlertView* alertView, int index) = 0; // slot 0
    virtual void AlertViewWasDismissed(GUIAlertView* alertView) = 0;                    // slot 1
};
#endif

class GuidePurchaseViewController;

// Delegate notified on purchase success through the raw pointer at
// +88 (see the layout note above). Interface and method names are
// UNRECOVERED; only the slot-0 (delegate, controller) call is proven.
class IGuidePurchaseViewControllerDelegate {
public:
    virtual void GuidePurchaseViewControllerDidPurchase(
        GuidePurchaseViewController* controller) = 0;  // slot 0
};

class GuidePurchaseViewController : public GUIViewController,
                                    public IGuidePurchaseViewDelegate,
                                    public IGUIAlertViewDelegate {
public:
    GuidePurchaseViewController();
    virtual ~GuidePurchaseViewController();

    virtual void LoadView();   // primary slot 3

    // IGuidePurchaseViewDelegate (primary slots 11-13, in this order).
    virtual void GuidePurchaseViewCloseButtonPressed(GuidePurchaseView* view);
    virtual void GuidePurchaseViewBuyButtonPressed(GuidePurchaseView* view);
    virtual void GuidePurchaseViewRestoreButtonPressed(GuidePurchaseView* view);

    // IGUIAlertViewDelegate (primary slots 14-15, in this order).
    virtual bool AlertViewClickedButtonAtIndex(GUIAlertView* alertView, int index);
    virtual void AlertViewWasDismissed(GUIAlertView* alertView);

    // Non-virtual methods (census functions; bodies recovered in the
    // .cpp — see the per-function evidence notes there).
    void SetBuyButtonEnabled(bool enabled);
    void SetRestoreButtonEnabled(bool enabled);
    void ProductRequestCompletion(const std::string& productId,
                                  const boost::shared_ptr<StoreProduct>& product,
                                  const std::string& error);
    void PurchaseCompletion(const std::string& productId, bool success,
                            const std::string& error);
    void RestoreCompletion(const std::vector<std::string>& productIds,
                           const std::string& error);
    void HandlePurchaseSuccess();

    IGuidePurchaseViewControllerDelegate* m_delegate;         // +88  (name UNRECOVERED)
    boost::shared_ptr<GuidePurchaseView> m_guideView;         // +96  (name UNRECOVERED)
    boost::shared_ptr<GUIAlertView> m_alertView;              // +112 (name UNRECOVERED)
    boost::shared_ptr<StoreProduct> m_product;                // +128 (name UNRECOVERED)
};

} // namespace Caver

#endif
