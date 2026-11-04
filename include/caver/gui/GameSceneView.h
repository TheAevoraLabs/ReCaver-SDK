// Recovered from libswordigo.so (1.4.13) evidence — IDA/Hex-Rays exports
// in ~/workspace/analysis/swordigo/functions/Caver/GameSceneView/, the
// golden disassembly of every body, and the ABS64 relocations of the
// golden vtable.
//
// GameSceneView: the in-game root view. It owns the scene controller,
// the HUD overlay (GameOverlayView), the debug overlay, the current
// notification / item-info popup, the cinematic skip button and three
// GUIEffects (letterbox / fade), and forwards touches to the Scene.
//
// Vtable _ZTVN5Caver13GameSceneViewE @0x62A620, 320 bytes = the 38
// GUIView slots, with exactly these overrides (ABS64 relocations):
//   0 TouchBegan, 1 TouchMoved, 2 TouchEnded, 3 TouchCancelled,
//   5 HandleKeyboardEvent, 7/8 ~GameSceneView D1/D0, 9 Update,
//   10 DrawRect, 11 LayoutSubviews.
// All other slots keep the GUIView/GUIResponder functions; the class
// adds NO new virtual slots.
//
// Layout (sizeof = 440 = 0x1B8, proven by the operator new(0x1B8) at
// the only construction site, GameViewController::BackgroundLoad
// @0x42D998, the ctor @0x42939C store map and the dtor @0x4297E8
// teardown order):
//   +256 boost::shared_ptr<GameSceneController> m_sceneController
//   +272 boost::shared_ptr<GameOverlayView>     m_overlayView
//   +288 boost::shared_ptr<DebugInfoOverlay>    m_debugInfoOverlay
//   +304 boost::shared_ptr<NotificationView>    m_notificationView
//   +320 boost::shared_ptr<ItemInfoPopupView>   m_itemInfoPopupView
//   +336 bool field336 — stored from SetCinematicModeEnabled's third
//        argument; gates the skip-button handling in TouchBegan.
//        Name UNRECOVERED.
//   +344 boost::shared_ptr<GUIButton> m_cinematicSkipButton
//   +360 bool field360 — set while the skip button is shown with its
//        hide timer running; Update counts field364 down and calls
//        HideCinematicSkipButton(true) at zero. Name UNRECOVERED.
//   +364 float field364 — the skip-button hide timer (TouchBegan sets
//        3.0f). Name UNRECOVERED.
//   +368 boost::shared_ptr<GUIEffect> field368 — an effect framed,
//        updated and drawn alongside the main effect; never created
//        by this class. Name UNRECOVERED.
//   +384 boost::shared_ptr<GUIEffect> m_effect — created by the ctor
//        (its field60 is then set to 1); framed in LayoutSubviews,
//        updated and drawn unconditionally.
//   +400 bool field400 — cinematic mode (SetCinematicModeEnabled's
//        first argument). Name UNRECOVERED.
//   +408 boost::shared_ptr<GUIEffect> field408 — the letterbox-bars
//        effect, created lazily by SetCinematicModeEnabled(true).
//        Name UNRECOVERED.
//   +424 bool field424 — read by Update (controls are hidden while it
//        is set or the scene is paused). Name UNRECOVERED.
//   +432 bool field432, +433 bool field433, +434 bool field434 — the
//        ctor zeroes all three (strh + strb). Only field434 is read by
//        this class: HandleKeyboardEvent ('W') toggles it together
//        with Scene+892, and DrawRect skips the GUIView draw while it
//        is set. Names UNRECOVERED.
#ifndef CAVER_GAMESCENEVIEW_H
#define CAVER_GAMESCENEVIEW_H

#include <boost/shared_ptr.hpp>
#include <string>

#include "GUIEffect.h"
#include "GUIView.h"

namespace Caver {

class GUIButton;
class GUIEvent;
class GameSceneController;
class GameOverlayView;
class DebugInfoOverlay;
class NotificationView;
class ItemInfoPopupView;
class Item;
class RenderingContext;

class GameSceneView : public GUIView {
public:
    GameSceneView();
    virtual ~GameSceneView();  // slots 7, 8

    void TouchBegan(const FWTouch& touch) override;       // slot 0
    void TouchMoved(const FWTouch& touch) override;       // slot 1
    void TouchEnded(const FWTouch& touch) override;       // slot 2
    void TouchCancelled(const FWTouch& touch) override;   // slot 3
    void HandleKeyboardEvent(const GUIKeyboardEvent& event) override;  // 5

    void Update(float deltaTime) override;               // slot 9
    void DrawRect(RenderingContext* context, const Rectangle& rect,
                  const Matrix4& transform) override;    // slot 10
    void LayoutSubviews() override;                      // slot 11

    // Non-virtual members (golden order is alphabetical in the census;
    // declaration order here follows the layout evidence).
    void InitWithSceneController(
        const boost::shared_ptr<GameSceneController>& controller);
    void ResetSceneController();
    void CinematicSkipButtonPressed(void* target, GUIEvent* event);
    void HideCinematicSkipButton(bool animated);
    void ShowNotification(
        const boost::shared_ptr<NotificationView>& notification);
    void ShowItemInfoPopup(const boost::shared_ptr<Item>& item);
    void HideItemInfoPopup();
    void SetCurrentMenuTab(const std::string& tab);
    void SetMenuButtonHighlighted(bool highlighted);
    void SetSkillToggleButtonHighlighted(bool highlighted);
    void SetCinematicModeEnabled(bool enabled, bool animated,
                                 bool field336Value);
    void ToggleDebugInfo();

    boost::shared_ptr<GameSceneController> m_sceneController;  // +256
    boost::shared_ptr<GameOverlayView> m_overlayView;          // +272
    boost::shared_ptr<DebugInfoOverlay> m_debugInfoOverlay;    // +288
    boost::shared_ptr<NotificationView> m_notificationView;    // +304
    boost::shared_ptr<ItemInfoPopupView> m_itemInfoPopupView;  // +320
    bool field336;                                             // +336
    char _pad337[7];                                           // +337
    boost::shared_ptr<GUIButton> m_cinematicSkipButton;        // +344
    bool field360;                                             // +360
    char _pad361[3];                                           // +361
    float field364;                                            // +364
    boost::shared_ptr<GUIEffect> field368;                     // +368
    boost::shared_ptr<GUIEffect> m_effect;                     // +384
    bool field400;                                             // +400
    char _pad401[7];                                           // +401
    boost::shared_ptr<GUIEffect> field408;                     // +408
    bool field424;                                             // +424
    char _pad425[7];                                           // +425
    bool field432;                                             // +432
    bool field433;                                             // +433
    bool field434;                                             // +434
    char _pad435[5];                                           // +435 -> 440
};

} // namespace Caver

#endif
