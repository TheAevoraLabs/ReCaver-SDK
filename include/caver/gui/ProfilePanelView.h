// Recovered from libswordigo.so (1.4.13) evidence — IDA/Hex-Rays
// exports in ~/workspace/analysis/swordigo/functions/Caver/
// ProfilePanelView/, the golden disassembly of the ctor / dtor /
// delegate thunks, and the march-1 GUI foundations.
//
// ProfilePanelView: one save-slot panel of the profile-selection
// screen — a GUIFrameView showing the hero (a SceneView rendering
// the "hero" scene with the profile's equipment applied), the
// player name / completion / last-played / level labels, a Start
// button, a "Set name" button and a close button that opens the
// delete confirmation. It is its own alert-view and text-field
// delegate for the two alerts it presents.
//
// Bases (RTTI/vptr evidence: the ctor stores the primary vptr at +0
// and the two secondary vptrs at +288 / +296; the golden thunks are
// _ZThn288_ for the alert-view methods and _ZThn296_ for the
// text-field methods):
//   +0   GUIFrameView (288 bytes incl. tail padding)
//   +288 IGUIAlertViewDelegate
//   +296 IGUITextFieldDelegate
//
// Layout (sizeof 640; member order proven by the D2 teardown
// sequence @0x463DE8, which releases the members in exact reverse
// order, and by the ctor @0x45E55C store map):
//   +304 shared_ptr<GUIAlertView> m_deleteAlert
//   +320 GUIButton* m_deleteConfirmButton (RAW — points at
//        m_deleteAlert->buttons[0]; captured in ShowDeleteConfirmation)
//   +328 bool field328 (zeroed by the ctor and again when the delete
//        alert is dismissed; no reader recovered — name UNRECOVERED)
//   +336 double field336 (ctor stores the 16-byte constant at
//        0x25A710 here and at +344: +336 = 0.0; role UNRECOVERED)
//   +344 double m_lastPlayedUpdateTime (ctor value -4.4765625;
//        Update() refreshes the "Played ... ago." label when the
//        current time is more than 60 s past this stamp)
//   +352 shared_ptr<GUIAlertView> m_nameAlert
//   +368 IProfilePanelDelegate* m_delegate (raw; no in-class writer)
//   +376 shared_ptr<PlayerProfile> m_profile
//   +392 shared_ptr<GUIButton> m_deleteButton (close-icon button;
//        ButtonPressed routes it to ShowDeleteConfirmation, and
//        AnimateIn/AnimateOut animate THIS view — bug-for-bug)
//   +408 shared_ptr<GUIButton> m_startButton ("Start")
//   +424 shared_ptr<GUIButton> m_renameButton ("Set name")
//   +440 shared_ptr<GUILabel> m_nameLabel
//   +456 shared_ptr<GUILabel> m_percentLabel
//   +472 shared_ptr<GUILabel> m_lastPlayedLabel
//   +488 shared_ptr<GUILabel> m_levelLabel
//   +504 shared_ptr<GUILabel> m_newGameLabel
//   +520 shared_ptr<SceneView> m_sceneView (created lazily by
//        InitWithProfile)
//   +536 HeroEquipmentManager m_equipmentManager (72 bytes)
//   +608 boost::intrusive_ptr<SceneObject> m_heroObject
//   +616 int field616 (Update() gate for the hero spin; ctor 0)
//   +620 .. +631 never written by the ctor (UNRECOVERED)
//   +632 float field632 (hero spin velocity; ctor 0; Update()
//        decays it at 15/s and clamps it to +/-15)
//
// Primary vtable overrides proven from the golden: D1/D2/D0,
// Update (slot 9), DrawRect (slot 10, a bare tail branch to
// GUIFrameView::DrawRect), LayoutSubviews (slot 11),
// ScrollViewCanCancelTouchInContentView (slot 27).
#ifndef CAVER_PROFILEPANELVIEW_H
#define CAVER_PROFILEPANELVIEW_H

#include <boost/intrusive_ptr.hpp>
#include <boost/shared_ptr.hpp>

#include "../systems/PlayerProfile.h"
#include "GUIAlertView.h"
#include "HeroEquipmentManager.h"

namespace Caver {

class GameState;
class GUIButton;
class GUIEvent;
class GUILabel;
class GUITextField;
class Item;
class ProfilePanelView;
class SceneObject;
class SceneView;

// The alert-view delegate interface, reproduced token-identically
// from GuidePurchaseViewController.h (the canonical declaration);
// the two must be unified at merge time. Slot order proven by the
// golden IGUIAlertViewDelegate implementors: 0 = clicked (bool),
// 1 = dismissed.
#ifndef CAVER_IGUIALERTVIEWDELEGATE_DEFINED
#define CAVER_IGUIALERTVIEWDELEGATE_DEFINED
class IGUIAlertViewDelegate {
public:
    virtual bool AlertViewClickedButtonAtIndex(GUIAlertView* alertView, int index) = 0;
    virtual void AlertViewWasDismissed(GUIAlertView* alertView) = 0;
};
#endif

// Delegate notified by the panel (raw pointer at +368). The
// implementor is ProfileSelectionView: its two methods
// ProfilePanelViewDidStart / ProfilePanelDidDeleteProfile are the
// namesakes of these slots (slot order proven by the call sites:
// ButtonPressed calls slot 0 after starting, DeleteProfile and
// TextFieldShouldReturn call slot 1). Interface name UNRECOVERED.
class IProfilePanelDelegate {
public:
    virtual void ProfilePanelViewDidStart(ProfilePanelView* panel) = 0;       // slot 0
    virtual void ProfilePanelDidDeleteProfile(ProfilePanelView* panel) = 0;   // slot 1
};

class ProfilePanelView : public GUIFrameView,
                         public IGUIAlertViewDelegate,
                         public IGUITextFieldDelegate {
public:
    ProfilePanelView();
    ~ProfilePanelView() override;  // primary slots 7, 8

    void Update(float deltaTime) override;                 // slot 9
    void DrawRect(RenderingContext* context, const Rectangle& rect,
                  const Matrix4& transform) override;      // slot 10
    void LayoutSubviews() override;                        // slot 11
    bool ScrollViewCanCancelTouchInContentView(GUIView* view) override;  // slot 27

    // IGUIAlertViewDelegate (secondary vtable at +288).
    bool AlertViewClickedButtonAtIndex(GUIAlertView* alertView, int index) override;
    void AlertViewWasDismissed(GUIAlertView* alertView) override;

    // IGUITextFieldDelegate (secondary vtable at +296).
    void TextFieldDidEndEditing(GUITextField* textField) override;
    bool TextFieldShouldReturn(GUITextField* textField) override;

    // Non-virtual members.
    void InitWithProfile(const boost::shared_ptr<PlayerProfile>& profile);
    void AnimateIn();
    void AnimateOut();
    void CreateProfile();
    void DeleteProfile();
    void ShowDeleteConfirmation();
    void ShowNamePrompt();
    void ButtonPressed(void* sender, GUIEvent* event);
    void DeleteTextFieldValueChanged(void* sender, GUIEvent* event);

    boost::shared_ptr<GUIAlertView> m_deleteAlert;         // +304
    GUIButton* m_deleteConfirmButton;                      // +320 (raw)
    bool field328;                                         // +328 UNRECOVERED
    double field336;                                       // +336 UNRECOVERED
    double m_lastPlayedUpdateTime;                         // +344
    boost::shared_ptr<GUIAlertView> m_nameAlert;           // +352
    IProfilePanelDelegate* m_delegate;                     // +368 (raw)
    boost::shared_ptr<PlayerProfile> m_profile;            // +376
    boost::shared_ptr<GUIButton> m_deleteButton;           // +392
    boost::shared_ptr<GUIButton> m_startButton;            // +408
    boost::shared_ptr<GUIButton> m_renameButton;           // +424
    boost::shared_ptr<GUILabel> m_nameLabel;               // +440
    boost::shared_ptr<GUILabel> m_percentLabel;            // +456
    boost::shared_ptr<GUILabel> m_lastPlayedLabel;         // +472
    boost::shared_ptr<GUILabel> m_levelLabel;              // +488
    boost::shared_ptr<GUILabel> m_newGameLabel;            // +504
    boost::shared_ptr<SceneView> m_sceneView;              // +520
    HeroEquipmentManager m_equipmentManager;               // +536
    boost::intrusive_ptr<SceneObject> m_heroObject;        // +608
    int field616;                                          // +616 UNRECOVERED
    char _pad620[12];                                      // +620 UNRECOVERED
    float field632;                                        // +632 (spin velocity)
};

// File-local in the golden TU (exported as a plain Caver:: symbol
// @0x464638): orders profiles by last-played time, newest first.
bool ProfileSortPredicate(const boost::shared_ptr<PlayerProfile>& a,
                          const boost::shared_ptr<PlayerProfile>& b);

} // namespace Caver

#endif
