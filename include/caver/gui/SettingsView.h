// Recovered from libswordigo.so (1.4.13) evidence — IDA/Hex-Rays
// exports in ~/workspace/analysis/swordigo/functions/Caver/
// SettingsView/, the golden vtable _ZTVN5Caver12SettingsViewE
// @0x6326A8 (39 slots) and the RTTI _ZTIN5Caver12SettingsViewE
// @0x6327E0 (__si_class_type_info, single base Caver::MenuViewPage
// @0x6301E8).
//
// SettingsView: the OPTIONS page of the tabbed menu — "Quit to Menu"
// and "$Configure Controls" framed buttons, a small "Game is saved
// automatically." label, the online buttons (Achievements always,
// Leaderboards only when offline achievements cannot be shown), and
// the music/sound rows: a PlainButton whose image is swapped between
// the on/off button textures, plus a 0..1 GUISlider each.
//
// Derivation (binary-proven): vtable slots 0-37 are the GUIView
// slots with Update (9) and LayoutSubviews (11) overridden, slot 38
// (MenuViewPage's Dismiss) holds SettingsView::Dismiss @0x4D41D0 —
// a lone `ret`, so Dismiss is an empty override. AnimateIn and
// InitWithGameState are non-virtual (no vtable slots past 38).
// The MenuViewPage base is proven by the D2 body's vtable reset to
// the MenuViewPage vtable @0x630258 before the page-title string at
// +256 is destroyed, and the tab handler's operator new(0x200)
// cross-checks sizeof 512.
//
// Layout (sizeof 512, byte-verified against the ctor @0x4D1828
// zero-store map, the D2 @0x4D420C teardown order — exact reverse
// declaration order — and every method body's accesses):
//   +0..279  MenuViewPage base (GUIView + page title string @+256)
//   +280     ISettingsViewDelegate* delegate (raw, non-owning;
//            GameMenuViewController implements the interface)
//   +288     boost::shared_ptr<GameState> gameState
//   +304     boost::shared_ptr<GUIButton> quitButton
//   +320     boost::shared_ptr<GUIButton> configureButton
//   +336     boost::shared_ptr<GUILabel> savedLabel
//   +352     boost::shared_ptr<GUIButton> achievementsButton
//   +368     boost::shared_ptr<GUIButton> leaderboardsButton
//   +384     std::vector<boost::shared_ptr<GUIButton> > onlineButtons
//            (member name UNRECOVERED — the buttons AnimateIn and
//            LayoutSubviews iterate: achievements, plus leaderboards
//            when it was added as a subview)
//   +408     boost::shared_ptr<GUIButton> musicButton
//   +424     boost::shared_ptr<GUIButton> soundButton
//   +440     boost::shared_ptr<GUISlider> musicSlider
//   +456     boost::shared_ptr<GUISlider> soundSlider
//   +472     float clickTimer (seconds since the last slider click
//            sound; Update adds the delta, SliderValueChanged tests
//            it against 0.15 and resets it — name UNRECOVERED)
//   +480     boost::intrusive_ptr<Texture> musicOnTexture
//   +488     boost::intrusive_ptr<Texture> musicOffTexture
//   +496     boost::intrusive_ptr<Texture> soundOnTexture
//   +504     boost::intrusive_ptr<Texture> soundOffTexture
#ifndef CAVER_SETTINGSVIEW_H
#define CAVER_SETTINGSVIEW_H

#include <vector>

#include <boost/intrusive_ptr.hpp>
#include <boost/shared_ptr.hpp>

#include "MenuViewPage.h"

namespace Caver {

class GameState;
class GUIButton;
class GUIEvent;
class GUILabel;
class GUISlider;
class SettingsView;
class Texture;

class ISettingsViewDelegate {
public:
    virtual void SettingsViewWantsToConfigureControls(SettingsView* view) = 0;       // slot 0
    virtual void SettingsViewDidQuitToMenu(SettingsView* view) = 0;                  // slot 1
    virtual void SettingsViewWantsToPresentOnlineView(SettingsView* view) = 0;       // slot 2
    virtual void SettingsViewDidShowOfflineAchievements(SettingsView* view) = 0;     // slot 3
};

class SettingsView : public MenuViewPage {
public:
    SettingsView();
    ~SettingsView() override;  // slots 7, 8

    void InitWithGameState(const boost::shared_ptr<GameState>& gameState);
    void UpdateAudioControls();
    void AnimateIn();

    void ButtonPressed(void* sender, GUIEvent* event);
    void SliderValueChanged(void* sender, GUIEvent* event);
    void SliderTouchUp(void* sender, GUIEvent* event);

    void Update(float deltaTime) override;   // slot 9
    void LayoutSubviews() override;          // slot 11
    void Dismiss() override;                 // slot 38

    ISettingsViewDelegate* delegate;                          // +280
    boost::shared_ptr<GameState> gameState;                   // +288
    boost::shared_ptr<GUIButton> quitButton;                  // +304
    boost::shared_ptr<GUIButton> configureButton;             // +320
    boost::shared_ptr<GUILabel> savedLabel;                   // +336
    boost::shared_ptr<GUIButton> achievementsButton;          // +352
    boost::shared_ptr<GUIButton> leaderboardsButton;          // +368
    std::vector<boost::shared_ptr<GUIButton> > onlineButtons; // +384 (name UNRECOVERED)
    boost::shared_ptr<GUIButton> musicButton;                 // +408
    boost::shared_ptr<GUIButton> soundButton;                 // +424
    boost::shared_ptr<GUISlider> musicSlider;                 // +440
    boost::shared_ptr<GUISlider> soundSlider;                 // +456
    float clickTimer;                                         // +472 (name UNRECOVERED)
    boost::intrusive_ptr<Texture> musicOnTexture;             // +480
    boost::intrusive_ptr<Texture> musicOffTexture;            // +488
    boost::intrusive_ptr<Texture> soundOnTexture;             // +496
    boost::intrusive_ptr<Texture> soundOffTexture;            // +504
};

// Golden aarch64 NDK-libc++ layout anchor; MSVC STL member sizes differ,
// so this assert is Android/Linux-only (the Windows build is self-consistent
// under MSVC STL and does not claim golden byte-layout parity).
#ifndef _MSC_VER
static_assert(sizeof(SettingsView) == 512,
              "SettingsView layout: ctor/dtor store maps, golden");
#endif

} // namespace Caver

#endif
