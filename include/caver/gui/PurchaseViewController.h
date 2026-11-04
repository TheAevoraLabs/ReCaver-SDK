// Recovered from libswordigo.so (1.4.13) evidence — IDA/Hex-Rays exports
// in ~/workspace/analysis/swordigo/functions/Caver/PurchaseViewController/
// plus the golden vtable recovered from ABS64 relocations.
//
// PurchaseViewController — primary vtable @0x62F948 (address point).
// It overrides only LoadView (slot 3) and ViewDidAppear (slot 7) of
// GUIViewController; the remaining slots are the base's. No new
// virtuals: UpdateView / ButtonPressed / the completion callbacks are
// plain methods (ButtonPressed and the completions reach the store
// through boost::bind / TargetAction, never through a vtable).
//
// Layout (sizeof = 144, from the ctor @0x499F3C zero-store map, the
// dtor @0x49BFF8 member-release order and the operator new(0x90) in
// StoreViewController::PurchaseProduct):
//   +0..71   GUIViewController base
//   +72      IPurchaseViewControllerDelegate* m_delegate (raw,
//            non-owning; StoreViewController::PurchaseProduct stores
//            its own IPurchaseViewControllerDelegate subobject here;
//            HandlePurchaseSuccess calls its slot 0 with this
//            controller after unlocking the product)
//   +80      shared_ptr<PurchaseView> — the purchase panel built by
//            LoadView (operator new(0x158)); also adopted as the
//            controller's m_view. Name UNRECOVERED.
//   +96      std::string product identifier (SetProduct copies the
//            product's identifier string into it, FetchProduct
//            assigns its argument into it; UpdateView switches the
//            panel content on it — "noads" / "compass" /
//            "coindoubler"; ProductRequestCompletion drops
//            completions whose identifier does not match it).
//            Name UNRECOVERED.
//   +120     shared_ptr<StoreProduct> — the product being purchased
//            (its identifier string at +0 and price string at +24 are
//            read by SetProduct / PurchaseCompletion /
//            EnableBuyButton / HandlePurchaseSuccess).
//            Name UNRECOVERED.
//   +136     bool — set by ProductRequestCompletion when it presents
//            the "OOPS!" error alert; UpdateView reads it to title
//            the (disabled) buy button "Unavailable" instead of
//            "Loading..." while no product is set. Name UNRECOVERED.
// Member names marked UNRECOVERED were not found in any evidence;
// only offsets, types and roles are proven.
#ifndef CAVER_PURCHASEVIEWCONTROLLER_H
#define CAVER_PURCHASEVIEWCONTROLLER_H

#include <string>

#include <boost/shared_ptr.hpp>

#include "StoreViewController.h" // IPurchaseViewControllerDelegate

namespace Caver {

class GUIEvent;
class PurchaseView;
class StoreProduct;

class PurchaseViewController : public GUIViewController {
public:
    PurchaseViewController();
    // No user-declared destructor: the golden D2 @0x49BFF8 / D0
    // @0x49C0E0 pair (232 / 240 bytes, the D0 duplicating the full
    // member-release body before operator delete) is the shape clang
    // emits for an implicit destructor, unlike the sibling
    // controllers' out-of-line empty dtors (36-byte D0).

    virtual void LoadView();       // primary slot 3
    virtual void ViewDidAppear();  // primary slot 7

    // Non-virtual methods (census functions; bodies in the .cpp).
    void ButtonPressed(void* sender, GUIEvent* event);
    void UpdateView();
    void FetchProduct(const std::string& productIdentifier);
    void ProductRequestCompletion(
        const std::string& productIdentifier,
        const boost::shared_ptr<StoreProduct>& product,
        const std::string& error);
    void SetProduct(const boost::shared_ptr<StoreProduct>& product);
    void EnableBuyButton();
    void PurchaseCompletion(const std::string& productIdentifier,
                            bool success, const std::string& error);
    void HandlePurchaseSuccess();

    IPurchaseViewControllerDelegate* m_delegate;       // +72
    boost::shared_ptr<PurchaseView> m_purchaseView;    // +80  name UNRECOVERED
    std::string m_productIdentifier;                   // +96  name UNRECOVERED
    boost::shared_ptr<StoreProduct> m_product;         // +120 name UNRECOVERED
    bool m_field136;                                   // +136 UNRECOVERED name
};

} // namespace Caver

#endif
