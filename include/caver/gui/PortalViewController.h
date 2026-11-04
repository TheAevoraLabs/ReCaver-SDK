// Recovered from libswordigo.so (1.4.13) evidence — IDA/Hex-Rays exports
// in ~/workspace/analysis/swordigo/functions/Caver/PortalViewController/,
// the golden vtable _ZTVN5Caver20PortalViewControllerE @0x62BA88
// (address point 0x62BA98, recovered from ABS64 relocations) and the
// __vmi RTTI @0x62BBB0 (type name string @0x62BBB8; the +72 base's RTTI
// name string @0x62BBA8 proves it is IMapViewDelegate).
//
// PortalViewController is the world-map / portal-travel modal presented
// by GameViewController::HandleDidEnterPortalGameEvent, which stores
// its IPortalViewControllerDelegate subobject at +104 and copies its
// shared_ptr<PlayerProfile> into +112 (see GameViewController.cpp).
//
// Bases (RTTI / ctor vptr-store order):
//   +0   GUIViewController (public)
//   +72  IMapViewDelegate                    (secondary @0x62BB38)
//   +80  IGuideToggleViewDelegate            (secondary @0x62BB60)
//   +88  IPurchaseViewControllerDelegate     (secondary @0x62BB78)
//   +96  IStoreViewControllerDelegate        (secondary @0x62BB90)
// Each delegate interface is a bare vptr (no data, no destructor slot):
// the matching secondary vtables contain exactly the interface's
// methods as _ZThn<adj>_ thunks and nothing else.
//
// Primary vtable (address point 0x62BA98, 18 slots):
//   0 ~PortalViewController (D1, in D2 @0x444108)  1 ~ (D0 @0x4441F8)
//   2 GUIViewController::NewLoadingView (not overridden)
//   3 LoadView (@0x443360)
//   4-10 GUIViewController::SuspendView / ResumeView / UnloadView /
//        ViewDidAppear / ViewWillDisappear / Update /
//        PresentModalViewController (not overridden)
//   11 MapViewDidSelectionDidChange (@0x443A00)
//   12 MapViewDidTapSelectedNode (@0x443CB0, empty)
//   13 MapViewDidTapEmptySpace (@0x443CB8, empty)
//   14 GuideToggleViewEnableStateChanged (@0x443CC0)
//   15 PurchaseViewControllerDidPurchaseProduct (@0x443F10)
//   16 StoreViewControllerDismissed (@0x443FC0, empty)
//   17 StoreViewControllerDidPurchaseProduct (@0x443FC8)
// Secondary vtables (offset-to-top / slots, all thunks):
//   -72  IMapViewDelegate: DidSelectionDidChange, DidTapSelectedNode,
//        DidTapEmptySpace
//   -80  IGuideToggleViewDelegate: GuideToggleViewEnableStateChanged
//   -88  IPurchaseViewControllerDelegate: DidPurchaseProduct
//   -96  IStoreViewControllerDelegate: Dismissed, DidPurchaseProduct
//
// Layout (sizeof = 144, from the ctor @0x443308 zero-store map
// +104..+143 and the destructor @0x444108 member-release order —
// +136 block before +120 block, exact reverse declaration order):
//   +104 IPortalViewControllerDelegate* m_delegate (raw, non-owning;
//        set by GameViewController; GuideToggleViewEnableStateChanged
//        calls its slot 1, ButtonPressed its slot 0)
//   +112 shared_ptr<PlayerProfile> m_profile (copied in by
//        GameViewController::HandleDidEnterPortalGameEvent)
//   +128 shared_ptr<PortalView> m_portalView (built by LoadView,
//        operator new(0x190) = 400-byte PortalView)
//
// NOTE: the delegate-interface declarations below repeat the identical
// declarations in GameViewController.h / GameMenuViewController.h /
// GuideToggleView.h on purpose (sibling-slice pattern): those headers
// cannot be included in this TU — GameViewController.h defines
// GameViewController on the GUINavigationController.h copy of the
// GUIViewController base, and GuideToggleView.h's
// IGuideToggleViewDelegate carries an inline virtual destructor the
// golden secondary vtable does not have. Reconcile at merge.
#ifndef CAVER_PORTALVIEWCONTROLLER_H
#define CAVER_PORTALVIEWCONTROLLER_H

#include <string>
#include <vector>

#include <boost/shared_ptr.hpp>

#include "GUIViewController.h"

namespace Caver {

class GameState;
class GUIControl;
class GUIEvent;
class GUISwitch;
class GuideToggleView;
class MapNode;
class MapView;
class MapViewNode;
class PlayerProfile;
class PortalView;
class PurchaseViewController;
class StoreViewController;

class PortalViewController;

// Delegate interfaces (bare vptrs, no destructor — see header note).
class IMapViewDelegate {
public:
    virtual void MapViewDidSelectionDidChange(MapView* mapView) = 0;  // slot 0
    virtual void MapViewDidTapSelectedNode(MapView* mapView) = 0;     // slot 1
    virtual void MapViewDidTapEmptySpace(MapView* mapView) = 0;       // slot 2
};

#ifndef CAVER_IGUIDETOGGLEVIEWDELEGATE_DEFINED
#define CAVER_IGUIDETOGGLEVIEWDELEGATE_DEFINED
class IGuideToggleViewDelegate {
public:
    virtual void GuideToggleViewEnableStateChanged(GuideToggleView* view) = 0; // slot 0
};
#endif

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

#ifndef CAVER_IPORTALVIEWCONTROLLERDELEGATE_DEFINED
#define CAVER_IPORTALVIEWCONTROLLERDELEGATE_DEFINED
class IPortalViewControllerDelegate {
public:
    virtual void PortalViewControllerDidGotoLevel(PortalViewController* controller, const std::string& levelName) = 0; // slot 0
    virtual void PortalViewControllerDidSetGuideEnabled(PortalViewController* controller, bool enabled) = 0;          // slot 1
};
#endif

class PortalViewController : public GUIViewController,
                             public IMapViewDelegate,
                             public IGuideToggleViewDelegate,
                             public IPurchaseViewControllerDelegate,
                             public IStoreViewControllerDelegate {
public:
    PortalViewController();
    virtual ~PortalViewController();

    // GUIViewController override (primary slot 3).
    virtual void LoadView();

    // IMapViewDelegate (primary slots 11-13).
    virtual void MapViewDidSelectionDidChange(MapView* mapView);
    virtual void MapViewDidTapSelectedNode(MapView* mapView);
    virtual void MapViewDidTapEmptySpace(MapView* mapView);

    // IGuideToggleViewDelegate (primary slot 14).
    virtual void GuideToggleViewEnableStateChanged(GuideToggleView* view);

    // IPurchaseViewControllerDelegate (primary slot 15).
    virtual void PurchaseViewControllerDidPurchaseProduct(PurchaseViewController* controller);

    // IStoreViewControllerDelegate (primary slots 16-17).
    virtual void StoreViewControllerDismissed(StoreViewController* controller);
    virtual void StoreViewControllerDidPurchaseProduct(StoreViewController* controller, const std::string& productIdentifier);

    // Non-virtual: bound to the PortalView buttons by LoadView.
    void ButtonPressed(void* sender, GUIEvent* event);

    IPortalViewControllerDelegate* m_delegate = nullptr;  // +104
    boost::shared_ptr<PlayerProfile> m_profile;           // +112
    boost::shared_ptr<PortalView> m_portalView;           // +128
};

} // namespace Caver

#endif
