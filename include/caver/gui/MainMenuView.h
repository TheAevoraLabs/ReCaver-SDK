// Recovered from libswordigo.so (1.4.13) evidence — IDA/Hex-Rays
// exports in ~/workspace/analysis/swordigo/functions/Caver/MainMenuView/,
// the golden vtable @0x62CD18 and the D2 teardown @0x45BE10.
//
// MainMenuView: the title-screen view. Direct GUIView subclass
// (RTTI @0x62CE58 is a __si_class_type_info on GUIView @0x635630).
// Vtable 320 bytes = the 38 GUIView slots with these overrides:
//   5  HandleKeyboardEvent (@0x45BDE8)
//   7/8 ~MainMenuView D1/D2 (@0x45BE10), D0 (@0x45C654)
//   9  Update (@0x45BD94)
//   11 LayoutSubviews (@0x45AD88)
//   16 DidMoveToWindow (@0x45C678)
//   24 CanBecomeFirstResponder (@0x45C6BC, returns true)
// All other slots keep the GUIView/GUIResponder functions; the class
// adds NO new virtual slots.
//
// Layout (sizeof = 680 = 0x2A8, proven by the operator new(0x2A8)
// in MainMenuViewController::LoadView and by the D2 teardown order,
// which releases the members in exact reverse declaration order —
// the shared_ptr control-block loads in the golden D2 are at
// px+8 for px = 664, 648, ..., 264):
//   +256 IMainMenuViewDelegate* (raw; set by the controller to
//        its own +80 subobject)
//   +264 shared_ptr<SceneView> — the "menu" scene view
//   +280 shared_ptr<GUIView> — plain-GUIView button container
//        (operator new(0x100) in the ctor); the menu buttons are
//        its subviews and LayoutSubviews sizes/centres it
//   +296 shared_ptr<GUIButton> — "Start" (delegate DidStart)
//   +312 shared_ptr<GUIButton> — "Online" (delegate
//        DidShowOfflineAchievements when the online controller's
//        ShowOfflineAchievements() succeeds, else the online menu
//        is shown)
//   +328 shared_ptr<GUIButton> — news (OnlineController::LaunchNews)
//   +344 shared_ptr<GUIButton> — "Offers" shop (delegate DidOpenShop)
//   +360 shared_ptr<GUIButton> — "Credits" (delegate DidGoToCredits)
//   +376 shared_ptr<GUIButton> — privacy button (delegate
//        DidOpenPrivacyPolicy; its title/frame are managed by
//        SetPrivacyButtonTitle/UpdatePrivacyButtonFrameLocation)
//   +392 std::vector<shared_ptr<GUIButton>> — the buttons animated
//        in by AnimateButtonsIn (in LayoutSubviews order)
//   +416 .. +439 UNRECOVERED (POD span: the golden D2 performs no
//        teardown for it and no body in this class reads it
//        directly — it is inside the region the ctor initialises
//        wholesale; see the ctor note in the .cpp)
//   +440 shared_ptr<OnlineMenuView>
//   +456 shared_ptr<GUIButton> — sound toggle (GameOptions byte +49
//        / volume +56; images at +504/+512)
//   +472 shared_ptr<GUIButton> — music toggle (GameOptions byte
//        +48 / volume +52; images at +488/+496)
//   +488 intrusive_ptr<Texture> "ui_button_music_blue"
//   +496 intrusive_ptr<Texture> "ui_button_music_off_blue"
//   +504 intrusive_ptr<Texture> "ui_button_sound_blue"
//   +512 intrusive_ptr<Texture> "ui_button_sound_off_blue"
//   +520 shared_ptr<GUIView> — social panel anchor (LayoutSubviews
//        positions +600/+616 relative to it) — pointee class
//        UNRECOVERED
//   +536 shared_ptr<GUIButton> — google sign-in button's companion
//        (centred by LayoutSubviews together with the label +552)
//   +552 shared_ptr<GUILabel> — label of the +536 button
//   +568 shared_ptr<GUILabel> — label of the +584 button
//   +584 shared_ptr<GUIButton> — feedback (OnlineController::
//        LaunchFeedbackView)
//   +600 shared_ptr<GUIButton> — facebook (OnlineController::
//        LaunchFacebook)
//   +616 shared_ptr<GUIButton> — twitter (OnlineController::
//        LaunchTwitter)
//   +632 shared_ptr<GUIButton> — google sign-in (OnlineController::
//        SignInToGoogleServices)
//   +648 shared_ptr<GUILabel> — label of the +632 button
//   +664 shared_ptr<BadgeCountView> — news badge (Update sets its
//        count from OnlineController::UnreadNewsCount)
// Member names are UNRECOVERED; the roles above are proven by the
// ButtonPressed / LayoutSubviews / Update bodies cited in the .cpp.
#ifndef CAVER_MAINMENUVIEW_H
#define CAVER_MAINMENUVIEW_H

#include <boost/intrusive_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <string>
#include <vector>

#include "GUIView.h"

namespace Caver {

class BadgeCountView;
class GUIButton;
class GUIEvent;
class GUIFrameView;
class GUILabel;
class OnlineMenuView;
class SceneView;
class Texture;

class MainMenuView;

// Delegate interface (bare vptr, no destructor). Token-identical
// to the declaration in MainMenuViewController.h; the two must be
// unified at merge time. Slot order is proven by the ButtonPressed
// delegate call sites (vtable byte offsets +0/+8/+16/+24/+32).
#ifndef IMAINMENUVIEWDELEGATE_DEFINED
#define IMAINMENUVIEWDELEGATE_DEFINED
class IMainMenuViewDelegate {
public:
    virtual void MainMenuViewDidStart(MainMenuView* view) = 0;                        // slot 0
    virtual void MainMenuViewDidOpenShop(MainMenuView* view) = 0;                     // slot 1
    virtual void MainMenuViewDidGoToCredits(MainMenuView* view) = 0;                  // slot 2
    virtual void MainMenuViewDidShowOfflineAchievements(MainMenuView* view) = 0;      // slot 3
    virtual void MainMenuViewDidOpenPrivacyPolicy(MainMenuView* view) = 0;            // slot 4
};
#endif

class MainMenuView : public GUIView {
public:
    MainMenuView();
    ~MainMenuView() override;  // slots 7, 8

    void HandleKeyboardEvent(const GUIKeyboardEvent& event) override;  // slot 5
    void Update(float deltaTime) override;                               // slot 9
    void LayoutSubviews() override;                                      // slot 11
    void DidMoveToWindow() override;                                     // slot 16
    bool CanBecomeFirstResponder() override;                             // slot 24

    // Non-virtual members (ButtonPressed is the boost::bind target
    // registered for every button in the ctor).
    void ButtonPressed(void* sender, GUIEvent* event);
    void UpdateAudioControls();
    void UpdatePrivacyButtonFrameLocation();
    void AnimateButtonsIn(float delay);
    void SetPrivacyButtonTitle(const std::string& title);
    void SetOnlineMenuVisible(bool visible);
    void SetGoogleSignInVisible(bool visible);

    IMainMenuViewDelegate* m_delegate;                    // +256 (raw)
    boost::shared_ptr<SceneView> m_sceneView;             // +264
    boost::shared_ptr<GUIView> m_buttonContainer;         // +280 (plain
                                  // GUIView, operator new(0x100) in the
                                  // ctor; the menu buttons are ITS
                                  // subviews, and LayoutSubviews sizes
                                  // and centres it over the stack)
    boost::shared_ptr<GUIButton> m_startButton;           // +296
    boost::shared_ptr<GUIButton> m_onlineButton;          // +312
    boost::shared_ptr<GUIButton> m_newsButton;            // +328
    boost::shared_ptr<GUIButton> m_shopButton;            // +344
    boost::shared_ptr<GUIButton> m_creditsButton;         // +360
    boost::shared_ptr<GUIButton> m_privacyButton;         // +376
    std::vector<boost::shared_ptr<GUIButton> > m_buttons; // +392
    bool field416;                                        // +416 (ctor sets
                                  // it to true; no reader in this class
                                  // — name UNRECOVERED)
    char _pad417[3];                                      // +417
    int field420[2];                                      // +420 (ctor zeroes;
                                  // 4-aligned — the golden store is
                                  // unaligned, so these are scalars,
                                  // not pointers)
    int field428[2];                                      // +428 (ctor zeroes)
    char _pad436[4];                                      // +436 UNRECOVERED
    boost::shared_ptr<OnlineMenuView> m_onlineMenuView;   // +440
    boost::shared_ptr<GUIButton> m_soundButton;           // +456
    boost::shared_ptr<GUIButton> m_musicButton;           // +472
    boost::intrusive_ptr<Texture> m_musicOnTexture;       // +488
    boost::intrusive_ptr<Texture> m_musicOffTexture;      // +496
    boost::intrusive_ptr<Texture> m_soundOnTexture;       // +504
    boost::intrusive_ptr<Texture> m_soundOffTexture;      // +512
    boost::shared_ptr<GUIFrameView> m_titleFrame;         // +520 (frame
                                  // texture "swordigo_title" in the ctor)
    boost::shared_ptr<GUIButton> m_achievementsButton;   // +536 achievements (ReportAllAchievements)
    boost::shared_ptr<GUILabel> m_achievementsLabel;           // +552
    boost::shared_ptr<GUILabel> m_feedbackLabel;          // +568
    boost::shared_ptr<GUIButton> m_feedbackButton;        // +584
    boost::shared_ptr<GUIButton> m_facebookButton;        // +600
    boost::shared_ptr<GUIButton> m_twitterButton;         // +616
    boost::shared_ptr<GUIButton> m_googleSignInButton;    // +632
    boost::shared_ptr<GUILabel> m_googleSignInLabel;      // +648
    boost::shared_ptr<BadgeCountView> m_badgeCountView;   // +664
};

// Golden aarch64 NDK-libc++ layout anchor; MSVC STL member sizes differ,
// so this assert is Android/Linux-only (the Windows build is self-consistent
// under MSVC STL and does not claim golden byte-layout parity).
#ifndef _MSC_VER
static_assert(sizeof(MainMenuView) == 680,
              "MainMenuView layout: D2/operator-new proven, ends at +680");
#endif

} // namespace Caver

#endif
