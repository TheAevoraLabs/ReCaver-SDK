// Recovered from libswordigo.so (1.4.13) evidence — IDA/Hex-Rays
// exports in ~/workspace/analysis/swordigo/functions/Caver/MapMenuPage/,
// the golden vtable _ZTVN5Caver11MapMenuPageE @0x6317F8 and RTTI
// @0x631980 (__vmi_class_type_info: bases MenuViewPage @0 and
// IMapViewDelegate @+280), and the ctor @0x4BF454 / dtor @0x4C11AC
// store maps.
//
// MapMenuPage: the "WORLD MAP" tab page — a MenuViewPage that owns a
// GUIScrollView containing a MapView, a bottom label and a StripeView
// title stripe, and implements IMapViewDelegate so a tap on a map node
// can teleport the player (when field536 is set).
//
// Vtable (primary, address point @0x631808): the 38 GUIView slots,
// slot 38 = Dismiss (MenuViewPage's pure slot, overridden empty),
// slots 39-41 = the IMapViewDelegate methods in interface order.
// Secondary vtable @+280 (address point @0x631968): offset-to-top
// -280, then the three _ZThn280_ thunks in the same order.
//
// Layout (sizeof 544, proven by TabbedMenuView's operator new(0x220)
// for this class and by the ctor/dtor store maps — every member below
// lands exactly where the golden bodies read/write it):
//   +0..279  MenuViewPage base (GUIView 256 + pageTitle string @+256)
//   +280     IMapViewDelegate base (vptr only)
//   +288     boost::shared_ptr<GameState> gameState (assigned by
//            InitWithGameState; released by the dtor after the
//            rounded-rect texture — see the dtor note in the .cpp)
//   +304     GUIRoundedRect containerBackground (168 bytes; ctor
//            inits it with texture "ui_menu_container")
//   +472     boost::shared_ptr<GUIScrollView> scrollView
//   +488     boost::shared_ptr<MapView> mapView (its m_delegate @+256
//            is set to this+280 by the ctor)
//   +504     boost::shared_ptr<GUILabel> bottomLabel (ctor: default
//            font + white text colour, added as a subview of this;
//            no MapMenuPage function ever sets its text — name
//            UNRECOVERED, role inferred from LayoutSubviews, which
//            frames it along the page's bottom edge)
//   +520     boost::shared_ptr<StripeView> stripeView (SetTitle /
//            SetTitleHorizOffset target in UpdateTitle/Layout)
//   +536     bool field536 (InitWithGameState's bool argument, also
//            passed to MapView::Init; gates the portal teleport in
//            MapViewDidTapSelectedNode — name UNRECOVERED)
//
// MERGE NOTES:
// - IMapViewDelegate is declared here exactly as in
//   PortalViewController.h (sibling-slice pattern: that header cannot
//   be included in this TU). At merge, keep exactly one definition.
// - MapView / MapNode / MapViewNode / GameState / GameData / Map /
//   GameEvent / DidEnterPortalGameEvent are NOT declared here: the
//   .cpp carries TU-local layout declarations for those classes
//   (proven offsets only), and MapView::m_delegate (MapViewDelegate*)
//   is pointed at this class's secondary IMapViewDelegate subobject
//   at +280 (MapMenuPage.cpp:123). NOTE: systems/MapNode.h now
//   declares Title(), so the old merge note asking for it is spent;
//   the TU-local decls are retained as-is (behavior unchanged).
#ifndef CAVER_MAPMENUPAGE_H
#define CAVER_MAPMENUPAGE_H

#include <boost/shared_ptr.hpp>
#include <string>

#include "GUIRoundedRect.h"
#include "MenuViewPage.h"

namespace Caver {

class GameState;
class GUILabel;
class GUIScrollView;
class GUIEvent;
class MapView;
class MapViewNode;
class RenderingContext;
class StripeView;
struct Matrix4;
struct Rectangle;

// The map-view delegate interface (bare vptr, no destructor — the
// golden secondary vtable for this base has exactly three slots, the
// _ZThn280_ thunks). Identical to the declaration in
// PortalViewController.h — see the merge note above.
class IMapViewDelegate {
public:
    virtual void MapViewDidSelectionDidChange(MapView* mapView) = 0;  // slot 0
    virtual void MapViewDidTapSelectedNode(MapView* mapView) = 0;     // slot 1
    virtual void MapViewDidTapEmptySpace(MapView* mapView) = 0;       // slot 2
};

class MapMenuPage : public MenuViewPage, public IMapViewDelegate {
public:
    MapMenuPage();
    ~MapMenuPage() override;  // slots 7, 8

    // GUIView overrides.
    void DrawRect(RenderingContext* context, const Rectangle& rect,
                  const Matrix4& transform) override;  // slot 10
    void LayoutSubviews() override;                    // slot 11

    // MenuViewPage's Dismiss (slot 38) — empty in the golden.
    void Dismiss() override;  // slot 38

    // IMapViewDelegate implementation (primary slots 39-41; the
    // secondary vtable holds the _ZThn280_ thunks to these).
    void MapViewDidSelectionDidChange(MapView* mapView) override;  // slot 39
    void MapViewDidTapSelectedNode(MapView* mapView) override;     // slot 40
    void MapViewDidTapEmptySpace(MapView* mapView) override;       // slot 41

    void InitWithGameState(const boost::shared_ptr<GameState>& gameState,
                           bool flag);
    void UpdateMapViewBounds();
    void UpdateTitle();
    void CenterAtNode(MapViewNode* node, bool animated);
    void ShowTravelToButtonFromNode(MapViewNode* node);
    void ScrollNodeToVisible(MapViewNode* node, bool animated);
    void ButtonPressed(void* sender, GUIEvent* event);

    boost::shared_ptr<GameState> gameState;             // +288
    GUIRoundedRect containerBackground;                 // +304
    boost::shared_ptr<GUIScrollView> scrollView;        // +472
    boost::shared_ptr<MapView> mapView;                 // +488
    boost::shared_ptr<GUILabel> bottomLabel;            // +504
    boost::shared_ptr<StripeView> stripeView;           // +520
    bool field536;                                      // +536 (UNRECOVERED)
    char _pad537[7];                                    // +537
};

// Golden aarch64 NDK-libc++ layout anchor; MSVC STL member sizes differ,
// so this assert is Android/Linux-only (the Windows build is self-consistent
// under MSVC STL and does not claim golden byte-layout parity).
#ifndef _MSC_VER
static_assert(sizeof(MapMenuPage) == 544,
              "MapMenuPage layout: MenuViewPage (280) + delegate (8) + "
              "members to +537, padded to 544");
#endif

} // namespace Caver

#endif
