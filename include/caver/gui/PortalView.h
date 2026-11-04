// Recovered from libswordigo.so (1.4.13) evidence — IDA/Hex-Rays exports
// in ~/workspace/analysis/swordigo/functions/Caver/PortalView/, the
// golden vtable (vptr stored by the D2 @0x4CD2FC, GUIView's 38 slots,
// no new virtuals) and the PortalViewController bodies that consume
// this view (operator new(0x190) in LoadView).
//
// PortalView is the world-map / portal-travel screen: a GUIView
// (256 bytes) plus nine shared_ptrs. Layout (sizeof = 400) proven by
// the ctor @0x4CB508 assignment order, the D2 @0x4CD2FC member-release
// order (exact reverse: +392, +376, +360, +344, +328, +312, +296,
// +280, +264 control blocks) and every method body's accesses:
//   +256 shared_ptr<GameState>       (assigned by InitWithGameState)
//   +272 shared_ptr<GUIScrollView>   (ctor; field450 set; the map is
//        added as ITS subview, PortalView::LayoutSubviews frames it
//        with PortalView's bounds, CenterAtNode scrolls it)
//   +288 shared_ptr<MapView>         (ctor; PortalViewController
//        installs its IMapViewDelegate subobject at map+256)
//   +304 shared_ptr<GUIButton>       close button (ctor: IconButton
//        "ui_button_close" / "ui_button_close_pressed", 34x34)
//   +320 shared_ptr<GUIButton>       goto button (IconButton
//        "ui_button_arrow" / "ui_button_arrow_pressed", 62x32,
//        starts hidden)
//   +336 shared_ptr<GUIButton>       help button (IconButton
//        "ui_button_help" / no pressed texture, 34x34, starts hidden,
//        field336 matrix set to diag(0.8, 0.8, 0.8, 1))
//   +352 shared_ptr<StripeView>      title stripe (ctor: SetHasTag
//        (true, 0.0), SetTitle("Portal"))
//   +368 shared_ptr<StripeView>      description stripe (SetDescription
//        forwards to its SetTitle)
//   +384 shared_ptr<GuideToggleView> (ctor; its label is set to
//        "COMPASS")
//
// Vtable overrides (GUIView/GUIResponder slots): 5
// HandleKeyboardEvent, 7/8 destructor, 11 LayoutSubviews, 16
// DidMoveToWindow, 24 CanBecomeFirstResponder (returns true).
#ifndef CAVER_PORTALVIEW_H
#define CAVER_PORTALVIEW_H

#include <string>

#include <boost/shared_ptr.hpp>

#include "GUIView.h"

namespace Caver {

class GameState;
class GUIButton;
class GUIScrollView;
class GuideToggleView;
class MapView;
class MapViewNode;
class StripeView;

class PortalView : public GUIView {
public:
    PortalView();
    virtual ~PortalView();

    void InitWithGameState(const boost::shared_ptr<GameState>& gameState,
                           bool started);
    void LayoutGotoButton();
    void CenterAtNode(MapViewNode* node, bool animated);
    void AnimateIn();
    void AnimateOut();
    void ScrollNodeToVisible(MapViewNode* node, bool animated);
    void SetGotoButtonVisible(bool visible);
    void SetHelpButtonVisible(bool visible);
    void SetDescription(const std::string& description);

    virtual void HandleKeyboardEvent(const GUIKeyboardEvent& event);  // 5
    virtual void LayoutSubviews();                                    // 11
    virtual void DidMoveToWindow();                                   // 16
    virtual bool CanBecomeFirstResponder();                           // 24

    boost::shared_ptr<GameState> m_gameState;              // +256
    boost::shared_ptr<GUIScrollView> m_scrollView;         // +272
    boost::shared_ptr<MapView> m_mapView;                  // +288
    boost::shared_ptr<GUIButton> m_closeButton;            // +304
    boost::shared_ptr<GUIButton> m_gotoButton;             // +320
    boost::shared_ptr<GUIButton> m_helpButton;             // +336
    boost::shared_ptr<StripeView> m_titleStripe;           // +352
    boost::shared_ptr<StripeView> m_descriptionStripe;     // +368
    boost::shared_ptr<GuideToggleView> m_guideToggleView;  // +384
};

} // namespace Caver

#endif
