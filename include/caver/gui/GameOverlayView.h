// Recovered from libswordigo.so (1.4.13) evidence — IDA/Hex-Rays exports
// in ~/workspace/analysis/swordigo/functions/Caver/GameOverlayView/.
//
// GameOverlayView: the in-game HUD overlay (touch control buttons, skill
// button, health/mana/experience bars, coin bar, consumable + menu
// buttons) — a direct GUIView subclass (ctor @0x486F78 calls
// GUIView::GUIView and stores the GameOverlayView vtable @0x62EA80;
// dtor tails into GUIView::~GUIView).
//
// Vtable: the 38 GUIView slots with these overrides (golden vtable
// @0x62EA80): slot 5 HandleKeyboardEvent, slot 11 LayoutSubviews,
// slot 24 CanBecomeFirstResponder, slot 29 HitTest (plus the dtors).
//
// Layout (GUIView = 256 bytes, then; every offset byte-verified against
// the ctor zero-store map, the dtor @0x48AF44 member-release order and
// the method bodies):
//   +256 IGameControlDelegate* m_delegate (RAW; the two interface slots
//        are called by ButtonHighlightChanged / HandleKeyboardEvent)
//   +264 boost::shared_ptr<GameState> m_gameState
//   +280 std::map<std::string, boost::shared_ptr<GUIView> >
//        m_subviewsByIdentifier (filled by the ctor; SubviewForIdentifier
//        is a map operator[] on it)
//   +304 float m_buttonScale (name UNRECOVERED; ctor inits 1.0f,
//        SetLayout recomputes it from the rendering context)
//   +308 bool m_controlButtonsInvisible (name UNRECOVERED)
//   +312 boost::shared_ptr<GUIButton> m_leftButton     (control 1)
//   +328 boost::shared_ptr<GUIButton> m_rightButton    (control 2)
//   +344 boost::shared_ptr<GUIButton> m_jumpButton     (control 3)
//   +360 boost::shared_ptr<GUIButton> m_swingButton    (control 4)
//   +376 boost::shared_ptr<GUIButton> m_useButton      (control 5)
//   +392 boost::shared_ptr<GUIButton> m_skillButton    (control 6)
//   +408 bool m_controlsHidden
//   +409 bool m_showsUseButton
//   +416 boost::shared_ptr<Skill> m_currentSkill
//   +432 boost::shared_ptr<OverlayButton> m_skillToggleButton (control 7)
//   +448 boost::shared_ptr<OverlayMenuButton> m_menuButton    (control 8)
//   +464 boost::shared_ptr<ConsumableItemView> m_consumableView (control 9)
//   +480 boost::shared_ptr<HealthBar> m_healthBar
//   +496 boost::shared_ptr<ManaBar> m_manaBar
//   +512 boost::shared_ptr<TargetFrame> m_targetFrame
//   +528 boost::shared_ptr<ExperienceBar> m_experienceBar
//   +544 boost::shared_ptr<ItemOverlay> m_itemOverlay
//   +560 boost::shared_ptr<CoinBar> m_coinBar
//   +576 boost::shared_ptr<GUIView> m_view576 (name UNRECOVERED; a plain
//        GUIView created by the ctor and added to the container)
//   +592 Rectangle m_safeBoundsCache (name UNRECOVERED; LayoutSubviews
//        stores safeBounds() here, then controlBounds() at +608)
//   +608 Rectangle m_controlBoundsCache (name UNRECOVERED)
//   +624 boost::shared_ptr<GUIViewLayout> m_layout
//   +640 boost::shared_ptr<GUIView> m_containerView (name UNRECOVERED;
//        the ctor adds it as a subview of *this and adds most other
//        members as subviews of it; LayoutSubviews sets its frame to
//        this view's bounds)
// sizeof = 656 (0x290).
//
// The HUD helper classes below (HealthBar, ManaBar, TargetFrame,
// ExperienceBar, ItemOverlay, CoinBar, OverlayButton, OverlayMenuButton,
// ConsumableItemView) have no in-tree headers yet; these are minimal
// declarations carrying only the members/methods GameOverlayView's
// bodies prove. They belong to whichever slice recovers those classes —
// reconcile at merge (GameViewController.cpp already carries TU-local
// views of ConsumableItemView / ItemOverlay / ExperienceBar).
#ifndef CAVER_GAMEOVERLAYVIEW_H
#define CAVER_GAMEOVERLAYVIEW_H

#include <map>
#include <string>

#include <boost/shared_ptr.hpp>

#include "../core/Rectangle.h"
#include "GUIButton.h"
#include "GUIView.h"
#include "GUIViewLayout.h"
#include "ManaBar.h"
#include "HealthBar.h"
#include "ExperienceBar.h"
#include "CoinBar.h"
#include "ItemOverlay.h"
#include "OverlayButton.h"
#include "ConsumableItemView.h"
#include "TargetFrame.h"

namespace Caver {

class GameState;
class GUIEvent;
class IGameControlDelegate;
class Skill;

// GameControlButton values proven by ControlForButton (@0x48CEA4:
// buttons in declaration order map to 1..9) and SetGameControlButtonState
// (@0x48B968: 1..6 map to the six main buttons). Enumerator NAMES are
// UNRECOVERED — the placeholders below carry the proven values only.
enum GameControlButton : int {
    GameControlButtonNone = 0,
    GameControlButton1 = 1,
    GameControlButton2 = 2,
    GameControlButton3 = 3,
    GameControlButton4 = 4,
    GameControlButton5 = 5,
    GameControlButton6 = 6,
    GameControlButton7 = 7,
    GameControlButton8 = 8,
    GameControlButton9 = 9
};

class GameOverlayView : public GUIView {
public:
    GameOverlayView();
    ~GameOverlayView() override;

    void InitWithGameState(const boost::shared_ptr<GameState>& gameState);
    void SetCurrentSkill(const boost::shared_ptr<Skill>& skill);
    void UpdateSkillToggleButtonVisibility();
    void UpdateUseButtonVisibility();
    void UpdateSwingButtonVisibility();
    void SetLayout(const boost::shared_ptr<GUIViewLayout>& layout);

    void CancelInput();
    void SetControlsHidden(bool hidden);
    void UpdateSkillButtonVisibility();
    void SetGameControlButtonState(GameControlButton button, bool state);
    void SetShowsUseButton(bool shows);
    void SetSkillButtonDisabled(bool disabled);

    void LayoutSubviews() override;  // slot 11
    void SetSubviewLayout(GUIView* view,
                          const boost::shared_ptr<GUIViewLayout>& layout);
    boost::shared_ptr<GUIView> SubviewForIdentifier(const std::string& identifier);
    Rectangle LayoutBoundsForSubview(const std::string& identifier);
    boost::shared_ptr<GUIViewLayout> LayoutForSubview(const std::string& identifier,
                                                      GUIView* view);
    boost::shared_ptr<GUIViewLayout> Layout();

    GameControlButton ControlForButton(GUIButton* button);
    void SetControlButtonsInvisible(bool invisible);
    void HandleKeyboardEvent(const GUIKeyboardEvent& event) override;  // slot 5
    boost::shared_ptr<GUIView> HitTest(const Vector2& point) override;  // slot 29
    bool CanBecomeFirstResponder() override;                            // slot 24

    // Non-virtual callback registered on every control button by the
    // ctor (GUIButton SHARED note: GameOverlayView::ButtonHighlightChanged
    // is the TargetAction target of the buttons).
    void ButtonHighlightChanged(void* sender, GUIEvent* event);

    IGameControlDelegate* m_delegate;                                // +256
    boost::shared_ptr<GameState> m_gameState;                        // +264
    std::map<std::string, boost::shared_ptr<GUIView> >
        m_subviewsByIdentifier;                                      // +280
    float m_buttonScale;                                             // +304
    bool m_controlButtonsInvisible;                                  // +308
    char _pad309[3];                                                 // +309
    boost::shared_ptr<GUIButton> m_leftButton;                       // +312
    boost::shared_ptr<GUIButton> m_rightButton;                      // +328
    boost::shared_ptr<GUIButton> m_jumpButton;                       // +344
    boost::shared_ptr<GUIButton> m_swingButton;                      // +360
    boost::shared_ptr<GUIButton> m_useButton;                        // +376
    boost::shared_ptr<GUIButton> m_skillButton;                      // +392
    bool m_controlsHidden;                                           // +408
    bool m_showsUseButton;                                           // +409
    char _pad410[6];                                                 // +410
    boost::shared_ptr<Skill> m_currentSkill;                         // +416
    boost::shared_ptr<OverlayButton> m_skillToggleButton;            // +432
    boost::shared_ptr<OverlayMenuButton> m_menuButton;               // +448
    boost::shared_ptr<ConsumableItemView> m_consumableView;          // +464
    boost::shared_ptr<HealthBar> m_healthBar;                        // +480
    boost::shared_ptr<ManaBar> m_manaBar;                            // +496
    boost::shared_ptr<TargetFrame> m_targetFrame;                    // +512
    boost::shared_ptr<ExperienceBar> m_experienceBar;                // +528
    boost::shared_ptr<ItemOverlay> m_itemOverlay;                    // +544
    boost::shared_ptr<CoinBar> m_coinBar;                            // +560
    boost::shared_ptr<GUIView> m_view576;                            // +576
    Rectangle m_safeBoundsCache;                                     // +592
    Rectangle m_controlBoundsCache;                                  // +608
    boost::shared_ptr<GUIViewLayout> m_layout;                       // +624
    boost::shared_ptr<GUIView> m_containerView;                      // +640
};

} // namespace Caver

#endif
