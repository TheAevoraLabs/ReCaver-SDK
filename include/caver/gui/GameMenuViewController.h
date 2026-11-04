// Recovered from libswordigo.so (1.4.13) evidence — IDA/Hex-Rays exports
// in ~/workspace/analysis/swordigo/functions/Caver/GameMenuViewController/,
// the golden vtable _ZTVN5Caver22GameMenuViewControllerE @0x629E98 (424
// bytes, recovered from ABS64 relocations) and the RTTI
// _ZTIN5Caver22GameMenuViewControllerE @0x62A0B0 (152 bytes,
// __vmi_class_type_info, 8 bases).
//
// GameMenuViewController is the in-game menu controller presented by
// GameViewController::ShowMenu (@0x430344): operator new(0xD0) fixes
// sizeof = 208, ShowMenu stores `gameVC + 88` (its
// IGameMenuViewControllerDelegate subobject) into +128 and copies
// GameViewController's shared_ptr<PlayerProfile> (+136) into +136.
//
// Bases (RTTI order, offset_flags in the RTTI):
//   +0   GUIViewController (public)
//   +72  IInventoryViewDelegate                 (0x4802)
//   +80  ICharacterViewDelegate                 (0x5000)
//   +88  ITabbedMenuViewDelegate                (0x5800)
//   +96  ISettingsViewDelegate                  (0x6000)
//   +104 IGuideToggleViewDelegate               (0x6800)
//   +112 IPurchaseViewControllerDelegate        (0x7000)
//   +120 IStoreViewControllerDelegate           (0x7800)
// Each delegate interface is a bare vptr (no data, no destructor slot):
// the matching secondary vtables in the golden contain exactly the
// interface's methods as _ZThn<adj>_ thunks and nothing else.
//
// Primary vtable (address point 0x629EA8, 24 slots):
//   0 ~GameMenuViewController (D1 @0x41FF74)  1 ~ (D0 @0x420120)
//   2 GUIViewController::NewLoadingView (not overridden)
//   3 LoadView (@0x420144)
//   4-7 GUIViewController::SuspendView/ResumeView/UnloadView/
//       ViewDidAppear (not overridden)
//   8 ViewWillDisappear (@0x420600)
//   9 GUIViewController::Update (not overridden)
//   10 GUIViewController::PresentModalViewController (not overridden)
//   11 TabbedMenuViewSelectedTabDidChange (@0x420638)
//   12 TabbedMenuViewDismissButtonPressed (@0x421C64)
//   13 CharacterViewDismissed (@0x421C7C, empty)
//   14 InventoryViewSelectedItemDidChange (@0x421C84, empty)
//   15 InventoryViewTrinketDidChange (@0x421C8C)
//   16 SettingsViewWantsToConfigureControls (@0x4222B8)
//   17 SettingsViewDidQuitToMenu (@0x4225D4)
//   18 SettingsViewWantsToPresentOnlineView (@0x422644)
//   19 SettingsViewDidShowOfflineAchievements (@0x422910)
//   20 GuideToggleViewEnableStateChanged (@0x422C28)
//   21 PurchaseViewControllerDidPurchaseProduct (@0x4231BC)
//   22 StoreViewControllerDismissed (@0x423418, empty)
//   23 StoreViewControllerDidPurchaseProduct (@0x423420)
// Secondary vtables (offset-to-top / slots, all thunks):
//   -72  IInventoryViewDelegate: SelectedItemDidChange, TrinketDidChange
//   -80  ICharacterViewDelegate: CharacterViewDismissed
//   -88  ITabbedMenuViewDelegate: SelectedTabDidChange,
//        DismissButtonPressed
//   -96  ISettingsViewDelegate: WantsToConfigureControls, DidQuitToMenu,
//        WantsToPresentOnlineView, DidShowOfflineAchievements
//   -104 IGuideToggleViewDelegate: GuideToggleViewEnableStateChanged
//   -112 IPurchaseViewControllerDelegate: DidPurchaseProduct
//   -120 IStoreViewControllerDelegate: Dismissed, DidPurchaseProduct
//
// Layout (sizeof = 208, from ShowMenu's operator new(0xD0), the ctor
// @0x41FF08 zero-store map +128..+199 and the destructor @0x41FF74
// member-release order, exact reverse declaration order):
//   +128 IGameMenuViewControllerDelegate* m_delegate (raw, non-owning;
//        ViewWillDisappear calls its slot 1, SettingsViewDidQuitToMenu
//        slot 0, GuideToggleViewEnableStateChanged slots 2/3)
//   +136 shared_ptr<PlayerProfile> m_profile (copied from the presenting
//        GameViewController by ShowMenu; LoadView reads the selected
//        menu tab string through profile -> gameState (+240) -> +384,
//        which is GameState's selected_menu_tab string)
//   +152 shared_ptr<TabbedMenuView> m_tabbedMenuView (built by LoadView,
//        operator new(0x198); also assigned into the inherited m_view)
//   +168 shared_ptr<GuideToggleView> — the guide toggle view: compared
//        against the sender in GuideToggleViewEnableStateChanged, whose
//        guide branch it is, and whose GUISwitch (+280 in the view) the
//        "compass" purchase path in PurchaseViewControllerDidPurchase-
//        Product / StoreViewControllerDidPurchaseProduct switches on.
//        Member name UNRECOVERED.
//   +184 shared_ptr<GuideToggleView> — the coin-doubler toggle view
//        (the other GuideToggleViewEnableStateChanged branch, which
//        forwards to the delegate's DidSetCoinDoublerEnabled slot).
//        Member name UNRECOVERED.
//   +200 8 bytes never written by the ctor and never released by the
//        dtor — UNRECOVERED (a raw pointer or scalar member, or tail
//        padding behind a byte member at +200; sizeof stays 208 either
//        way because of the 8-byte alignment).
//
// NOTE: TabbedMenuViewSelectedTabDidChange (@0x420638, 3888 bytes) is
// declared here (primary slot 11 + ITabbedMenuViewDelegate slot 0 —
// LoadView calls it through the vtable) but is NOT defined in this
// slice's assigned function list; its body is a hand-off.
//
// NOTE: IGuideToggleViewDelegate is also declared in GuideToggleView.h
// (gui slice) WITH an inline virtual destructor; the golden secondary
// vtable for this base has exactly one slot and no destructor, so the
// no-destructor form below is the binary-true one — reconcile at merge.
// IGameMenuViewControllerDelegate / IStoreViewControllerDelegate below
// repeat the (identical) declarations in GameViewController.h, which
// cannot be included in this TU: that header defines GameViewController
// itself on top of the GUINavigationController.h copy of the
// GUIViewController base, while this header uses GUIViewController.h's.
#ifndef CAVER_GAMEMENUVIEWCONTROLLER_H
#define CAVER_GAMEMENUVIEWCONTROLLER_H

#include <string>
#include <boost/shared_ptr.hpp>

#include "GUIViewController.h"
#include "TabbedMenuView.h"
#include "InventoryView.h"
#include "CharacterView.h"
#include "SettingsView.h"
#include "GuideToggleView.h"

namespace Caver {

class PlayerProfile;
class PurchaseViewController;
class StoreViewController;

class GameMenuViewController;

#ifndef CAVER_IPURCHASEVIEWCONTROLLERDELEGATE_DEFINED
#define CAVER_IPURCHASEVIEWCONTROLLERDELEGATE_DEFINED
class IPurchaseViewControllerDelegate {
public:
    virtual void PurchaseViewControllerDidPurchaseProduct(PurchaseViewController* controller) = 0; // slot 0
};
#endif

#ifndef CAVER_ISTOREVIEWCONTROLLERDELEGATE_DEFINED
#define CAVER_ISTOREVIEWCONTROLLERDELEGATE_DEFINED
class IStoreViewControllerDelegate {
public:
    virtual void StoreViewControllerDismissed(StoreViewController* controller) = 0; // slot 0
    virtual void StoreViewControllerDidPurchaseProduct(StoreViewController* controller, const std::string& productIdentifier) = 0; // slot 1
};
#endif

#ifndef CAVER_IGAMEMENUVIEWCONTROLLERDELEGATE_DEFINED
#define CAVER_IGAMEMENUVIEWCONTROLLERDELEGATE_DEFINED
class IGameMenuViewControllerDelegate {
public:
    virtual void GameMenuViewControllerDidQuitToMenu(GameMenuViewController* controller) = 0; // slot 0
    virtual void GameMenuViewControllerDismissed(GameMenuViewController* controller) = 0;     // slot 1
    virtual void GameMenuViewControllerDidSetGuideEnabled(GameMenuViewController* controller, bool enabled) = 0;       // slot 2
    virtual void GameMenuViewControllerDidSetCoinDoublerEnabled(GameMenuViewController* controller, bool enabled) = 0; // slot 3
};
#endif

class GameMenuViewController : public GUIViewController,
                               public IInventoryViewDelegate,
                               public ICharacterViewDelegate,
                               public ITabbedMenuViewDelegate,
                               public ISettingsViewDelegate,
                               public IGuideToggleViewDelegate,
                               public IPurchaseViewControllerDelegate,
                               public IStoreViewControllerDelegate {
public:
    GameMenuViewController();
    virtual ~GameMenuViewController();

    // GUIViewController overrides (primary slots 3, 8).
    virtual void LoadView();            // slot 3
    virtual void ViewWillDisappear();   // slot 8

    // Delegate-interface implementations, in golden primary-vtable
    // order (slots 11-23).
    virtual void TabbedMenuViewSelectedTabDidChange(TabbedMenuView* view);   // slot 11 (declared; body is a hand-off — see header note)
    virtual void TabbedMenuViewDismissButtonPressed(TabbedMenuView* view);   // slot 12
    virtual void CharacterViewDismissed(CharacterView* view);                // slot 13
    virtual void InventoryViewSelectedItemDidChange(InventoryView* view, InventoryViewItemType type); // slot 14
    virtual void InventoryViewTrinketDidChange(InventoryView* view, InventoryViewItemType type);        // slot 15
    virtual void SettingsViewWantsToConfigureControls(SettingsView* view);   // slot 16
    virtual void SettingsViewDidQuitToMenu(SettingsView* view);              // slot 17
    virtual void SettingsViewWantsToPresentOnlineView(SettingsView* view);  // slot 18
    virtual void SettingsViewDidShowOfflineAchievements(SettingsView* view); // slot 19
    virtual void GuideToggleViewEnableStateChanged(GuideToggleView* view);   // slot 20
    virtual void PurchaseViewControllerDidPurchaseProduct(PurchaseViewController* controller); // slot 21
    virtual void StoreViewControllerDismissed(StoreViewController* controller);                 // slot 22
    virtual void StoreViewControllerDidPurchaseProduct(StoreViewController* controller, const std::string& productIdentifier); // slot 23

    IGameMenuViewControllerDelegate* m_delegate;              // +128
    boost::shared_ptr<PlayerProfile> m_profile;               // +136
    boost::shared_ptr<TabbedMenuView> m_tabbedMenuView;       // +152
    boost::shared_ptr<GuideToggleView> m_guideToggleView;     // +168 (name UNRECOVERED)
    boost::shared_ptr<GuideToggleView> m_coinDoublerToggleView; // +184 (name UNRECOVERED)
    char _pad200[8];                                          // +200 UNRECOVERED
};

static_assert(sizeof(GameMenuViewController) == 208,
              "GameMenuViewController layout: ShowMenu operator new(0xD0)");

} // namespace Caver

#endif
