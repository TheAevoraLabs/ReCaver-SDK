// Recovered from libswordigo.so (1.4.13) evidence — IDA/Hex-Rays exports
// in ~/workspace/analysis/swordigo/functions/Caver/StoreViewController/
// plus the golden vtables recovered from ABS64 relocations.
//
// StoreViewController — vtable group @0x62FD98 (primary address point),
// secondary @0x62FE08 for the IPurchaseViewControllerDelegate subobject
// at +72 (its only slot is the _ZThn72_ thunk to
// PurchaseViewControllerDidPurchaseProduct).
//
// Primary vtable (12 slots):
//   0 ~D1 (@0x49FA98), 1 ~D0 (@0x49FC10),
//   2 GUIViewController::NewLoadingView,
//   3 LoadView (@0x49FC34), 4 SuspendView (@0x4A2A34, empty),
//   5 ResumeView (@0x4A2A38, empty), 6 UnloadView (@0x4A2A3C, empty),
//   7 ViewDidAppear (@0x4A29F4), 8 GUIViewController::ViewWillDisappear,
//   9 Update (@0x4A1CCC), 10 GUIViewController::PresentModalViewController,
//   11 PurchaseViewControllerDidPurchaseProduct (@0x4A273C)
//
// Layout (sizeof = 192, from the ctor @0x49FA38 operator-new size in the
// callers (0xC0), the ctor zero-store map and the dtor @0x49FA98
// member-release order):
//   +0..71   GUIViewController base
//   +72      IPurchaseViewControllerDelegate vptr
//   +80      IStoreViewControllerDelegate* m_delegate (raw, non-owning;
//            the GameMenuViewController slice's stub already modelled
//            this offset from its call sites — confirmed here)
//   +88      shared_ptr<StoreView> (the store view built by LoadView)
//   +104     bool (error-alert-shown flag: ProductRequestCompletion
//            tests it before presenting the "OOPS!" alert and sets it;
//            member name UNRECOVERED)
//   +112     vector<string> product identifiers (LoadView pushes
//            "noads", "compass", "coindoubler"; every lookup/loop in
//            the class walks this vector; name UNRECOVERED)
//   +136     vector<shared_ptr<StoreProduct>> fetched products,
//            index-parallel to the identifiers (ProductRequestCompletion
//            assigns by matching index; name UNRECOVERED)
//   +160     set<string> of product identifiers purchased in this
//            session (PurchaseViewControllerDidPurchaseProduct inserts,
//            UpdateProductBuyButton finds, LoadView clears, the dtor
//            destroys it with the libc++ __tree destroy helper;
//            name UNRECOVERED)
//   +184     bool (a product request has completed: set by
//            ProductRequestCompletion, cleared by LoadView; drives the
//            "Unavailable" vs "Loading..." button title; name
//            UNRECOVERED)
//   +185     bool (last HasRestoredPurchases() value polled by Update)
//   +186     bool (shadow of +184 used by Update's change detection)
//   +187     bool (shadow of +185 used by Update's change detection)
// Member names marked UNRECOVERED were not found in any evidence; only
// offsets, types and roles are proven.
#ifndef CAVER_STOREVIEWCONTROLLER_H
#define CAVER_STOREVIEWCONTROLLER_H

#include <set>
#include <string>
#include <vector>

#include <boost/shared_ptr.hpp>

#include "GUIViewController.h"

namespace Caver {

class GUIEvent;
class PurchaseViewController;
class StoreProduct;
class StoreProductView;
class StoreView;
class StoreViewController;

// Delegate interface for StoreViewController's owner (implemented by
// GameMenuViewController at its +120 subobject and by
// GameViewController / PortalViewController). Identical declaration to
// the copies in GameMenuViewController.h / GameViewController.h.
#ifndef CAVER_ISTOREVIEWCONTROLLERDELEGATE_DEFINED
#define CAVER_ISTOREVIEWCONTROLLERDELEGATE_DEFINED
class IStoreViewControllerDelegate {
public:
    virtual void StoreViewControllerDismissed(StoreViewController* controller) = 0; // slot 0
    virtual void StoreViewControllerDidPurchaseProduct(StoreViewController* controller, const std::string& productIdentifier) = 0; // slot 1
};
#endif

// Delegate interface implemented by StoreViewController itself (at
// +72) so PurchaseViewController can report a completed purchase.
// Its vtable has exactly this one slot (golden secondary vtable
// @0x62FE08 and the GameMenuViewController slice's +112 subobject).
#ifndef CAVER_IPURCHASEVIEWCONTROLLERDELEGATE_DEFINED
#define CAVER_IPURCHASEVIEWCONTROLLERDELEGATE_DEFINED
class IPurchaseViewControllerDelegate {
public:
    virtual void PurchaseViewControllerDidPurchaseProduct(PurchaseViewController* controller) = 0; // slot 0
};
#endif

class StoreViewController : public GUIViewController,
                            public IPurchaseViewControllerDelegate {
public:
    StoreViewController();
    virtual ~StoreViewController();

    virtual void LoadView();       // primary slot 3
    virtual void SuspendView();    // primary slot 4 (empty in the golden)
    virtual void ResumeView();     // primary slot 5 (empty in the golden)
    virtual void UnloadView();     // primary slot 6 (empty in the golden)
    virtual void ViewDidAppear();  // primary slot 7
    virtual void Update(float deltaTime); // primary slot 9

    // IPurchaseViewControllerDelegate (primary slot 11, secondary
    // slot 0 through the _ZThn72_ thunk).
    virtual void PurchaseViewControllerDidPurchaseProduct(
        PurchaseViewController* controller);

    // Non-virtual methods (census functions; bodies in the .cpp).
    void ButtonPressed(void* sender, GUIEvent* event);
    void UpdateProductBuyButton(const std::string& productIdentifier);
    void ProductRequestCompletion(
        const std::string& productIdentifier,
        const boost::shared_ptr<StoreProduct>& product,
        const std::string& error);
    StoreProductView* ProductViewForProduct(
        const std::string& productIdentifier);
    void RestoreCompletion(const std::vector<std::string>& productIdentifiers,
                           const std::string& error);
    void PurchaseProduct(const boost::shared_ptr<StoreProduct>& product);

    IStoreViewControllerDelegate* m_delegate;              // +80
    boost::shared_ptr<StoreView> m_storeView;              // +88
    bool m_field104;                                       // +104 UNRECOVERED name
    std::vector<std::string> m_productIdentifiers;         // +112 name UNRECOVERED
    std::vector<boost::shared_ptr<StoreProduct> > m_products; // +136 name UNRECOVERED
    std::set<std::string> m_purchasedProducts;             // +160 name UNRECOVERED
    bool m_field184;                                       // +184 UNRECOVERED name
    bool m_field185;                                       // +185 UNRECOVERED name
    bool m_field186;                                       // +186 UNRECOVERED name
    bool m_field187;                                       // +187 UNRECOVERED name
};

} // namespace Caver

#endif
