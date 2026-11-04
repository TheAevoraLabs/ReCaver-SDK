// Recovered from libswordigo.so (1.4.13) evidence.
// StoreController base + StoreController_Android (JNI store backend).
//
// Base layout proven from the golden binary:
//   - vtable _ZTVN5Caver15StoreControllerE @0x62DB98, 12 slots:
//     0/1 ~D1/~D0, 2 GetStoreName, 3 IsProductPurchased,
//     4 FetchProductWithIdentifier, 5 FetchProductsWithIdentifiers,
//     6 PurchaseProduct, 7 RestoreCompletedTransactions,
//     8 ConsumePurchasesForTesting, 9 CheckPromotion,
//     10 HasRestoredPurchases, 11 ShowDebugInfo (relocations at
//     0x62DBA8..0x62DC00; the Android vtable @0x636940 repeats the
//     order and inherits slot 11 from the base).
//   - The base ctor was implicit/inlined (no C1/C2 symbol in the
//     golden): SharedController @0x46C158 allocates 0x20 bytes —
//     vptr @+0 and one std::string @+8 (its name is UNRECOVERED;
//     other slices write it, e.g. "MainMenu"/"Map").
//   - The base deleting destructor is NOT a bare tail-branch (its
//     D1 string teardown is inlined into the 0x46D4E8 D0), and the
//     derived D0 (0x542AE4, 36 bytes) calls D1 + operator delete,
//     so both destructors are user-declared with empty bodies.
//
// StoreProduct: sizeof 48 (coordinator-established): two std::string
// members, identifier @+0 and price @+24; its
// (const std::string&, const std::string&) ctor is inline in the
// original (the golden inlines the member copies at call sites).
//
// StoreController_Android: sizeof 0x70 (operator new size in
// setupApplication). Layout proven from its ctor @0x542A2C and
// destructor @0x542A70:
//   +0   vptr
//   +8   base std::string
//   +32  std::map<std::string, TargetAction<FetchSignature>>
//        (__tree header: begin @+32 = &end-node @+40, size @+48;
//        destroyed by the @0x543BF8 helper in ~StoreController_Android;
//        written by FetchProductsWithIdentifiers, read/erased by
//        HandleProductFetch). Member name UNRECOVERED.
//   +56  std::map<std::string, TargetAction<PurchaseSignature>>
//        (same header shape @+56/+64/+72; destroyed by @0x543CD4;
//        written by PurchaseProduct, read/erased by
//        HandleProductPurchase). Member name UNRECOVERED.
//   +80  std::set<std::string> of purchased product identifiers
//        (header @+80/+88/+96; IsProductPurchased @0x542B0C finds in
//        it, HandleProductPurchase inserts on success,
//        StartedRestoringPurchases clears it). Name UNRECOVERED.
//   +104 bool restored-purchases flag (ResetHasRestoredPurchases
//        clears it, FinishedRestoringPurchases sets it,
//        HasRestoredPurchases returns it). Name UNRECOVERED.
#ifndef CAVER_STORECONTROLLER_ANDROID_H
#define CAVER_STORECONTROLLER_ANDROID_H

#include <map>
#include <set>
#include <string>
#include <vector>

#include <boost/shared_ptr.hpp>

#include "OnlineController.h" // Caver::TargetAction<Signature>

namespace Caver {

class StoreProduct {
public:
    StoreProduct(const std::string& identifier, const std::string& price)
        : m_identifier(identifier), m_price(price) {}

    std::string m_identifier; // +0
    std::string m_price;      // +24
};

class StoreController {
public:
    static StoreController* SharedController();
    static void SetSharedController(
        const boost::shared_ptr<StoreController>& controller);

    virtual ~StoreController();

    virtual std::string GetStoreName();               // slot 2
    virtual bool IsProductPurchased(const std::string& identifier); // slot 3
    virtual void FetchProductWithIdentifier(          // slot 4
        const std::string& identifier,
        TargetAction<void(const std::string&,
                          const boost::shared_ptr<StoreProduct>&,
                          const std::string&)> action);
    virtual void FetchProductsWithIdentifiers(         // slot 5
        const std::vector<std::string>& identifiers,
        TargetAction<void(const std::string&,
                          const boost::shared_ptr<StoreProduct>&,
                          const std::string&)> action);
    virtual void PurchaseProduct(                      // slot 6
        const boost::shared_ptr<StoreProduct>& product,
        TargetAction<void(const std::string&, bool,
                          const std::string&)> action);
    virtual void RestoreCompletedTransactions(         // slot 7
        TargetAction<void(const std::vector<std::string>&,
                          const std::string&)> action);
    virtual void ConsumePurchasesForTesting();         // slot 8
    virtual bool CheckPromotion(const std::string& identifier); // slot 9
    virtual bool HasRestoredPurchases();               // slot 10
    virtual void ShowDebugInfo();                      // slot 11

    // Non-virtual base helpers (bodies owned by the StoreController
    // slice; FinishedRestoringPurchases calls this one).
    void ResetProductUnlocksToPurchases();
    static bool IsGuideUnlockedCheck();
    static bool IsCoinDoublerUnlockedCheck();
    static bool IsNoAdsUnlockedCheck();
    static bool IsProductUnlockedCheck(const std::string& identifier);
    static void UnlockProduct(const std::string& identifier);

    std::string m_field8; // +8 (name UNRECOVERED)
};

class StoreController_Android : public StoreController {
public:
    StoreController_Android();
    virtual ~StoreController_Android();

    virtual std::string GetStoreName();
    virtual bool IsProductPurchased(const std::string& identifier);
    virtual void FetchProductWithIdentifier(
        const std::string& identifier,
        TargetAction<void(const std::string&,
                          const boost::shared_ptr<StoreProduct>&,
                          const std::string&)> action);
    virtual void FetchProductsWithIdentifiers(
        const std::vector<std::string>& identifiers,
        TargetAction<void(const std::string&,
                          const boost::shared_ptr<StoreProduct>&,
                          const std::string&)> action);
    virtual void PurchaseProduct(
        const boost::shared_ptr<StoreProduct>& product,
        TargetAction<void(const std::string&, bool,
                          const std::string&)> action);
    virtual void RestoreCompletedTransactions(
        TargetAction<void(const std::vector<std::string>&,
                          const std::string&)> action);
    virtual void ConsumePurchasesForTesting();
    virtual bool CheckPromotion(const std::string& identifier);
    virtual bool HasRestoredPurchases();

    // JNI-callback entry points (called from the Native JNI cluster
    // when the Java StoreController reports results).
    void HandleProductFetch(const std::string& identifier,
                            const boost::shared_ptr<StoreProduct>& product,
                            const std::string& error);
    void HandleProductPurchase(const std::string& identifier, bool success,
                               const std::string& error);
    void ResetHasRestoredPurchases();
    void StartedRestoringPurchases();
    void FinishedRestoringPurchases();

private:
    typedef TargetAction<void(const std::string&,
                              const boost::shared_ptr<StoreProduct>&,
                              const std::string&)> FetchAction;
    typedef TargetAction<void(const std::string&, bool,
                              const std::string&)> PurchaseAction;

    std::map<std::string, FetchAction> m_fetchCallbacks;       // +32
    std::map<std::string, PurchaseAction> m_purchaseCallbacks; // +56
    std::set<std::string> m_purchasedProducts;                 // +80
    bool m_hasRestoredPurchases;                               // +104
};

} // namespace Caver

#endif
