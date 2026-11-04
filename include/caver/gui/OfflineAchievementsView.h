// Recovered from libswordigo.so (1.4.13) evidence — IDA/Hex-Rays exports
// in ~/workspace/analysis/swordigo/functions/Caver/
// OfflineAchievementsView/ (ctor @0x44DF84, dtor @0x4506CC) and the
// golden vtable @0x62C168 (off_62C168 in the ctor/dtor bodies).
//
// OfflineAchievementsView: the offline achievements screen — a title
// label with a divider, a scroll view full of OfflineAchievementView
// (singular) rows, a back/close button, and two Google-services
// buttons (Sync + Google) that only exist when the online controller
// reports SignedInToGoogleServices().
//
// Single base Caver::GUIView (the ctor calls GUIView's ctor, the dtor
// tail calls GUIView::~GUIView). Vtable: the 38 GUIView slots;
// overrides are
//   5  HandleKeyboardEvent @0x450698
//   7/8 ~OfflineAchievementsView (D2 @0x4506CC, D0 @0x450988)
//   11 LayoutSubviews @0x44FB44
//   16 DidMoveToWindow @0x4509AC
//   24 CanBecomeFirstResponder @0x4509F0 (= true)
//
// Layout: GUIView (256 bytes), then (dtor teardown order, ctor
// assignments and every method's accesses agree):
//   +256 boost::shared_ptr<GUIButton>    closeButton (arrow button;
//        HandleKeyboardEvent fires its actions for control event 64)
//   +272 boost::shared_ptr<GUILabel>     titleLabel ("<u> of <t>
//        Achievements (<p> pts[.)]" built in the ctor)
//   +288 boost::shared_ptr<GUIFrameView> dividerView ("white" frame
//        texture, colour 127,127,127,127; a subview of titleLabel)
//   +304 boost::shared_ptr<GUIScrollView> scrollView
//   +320 std::vector<boost::shared_ptr<OfflineAchievementView>>
//        achievementRows (one row per achievement, subviews of
//        scrollView)
//   +344 boost::shared_ptr<GUIButton>    googleButton (image
//        "ui_google_controller", 40x40)
//   +360 boost::shared_ptr<GUIButton>    syncButton (title "Sync",
//        40x22)
// sizeof = 376. Original member names UNRECOVERED — the names above
// are behaviour-derived.
#ifndef CAVER_OFFLINEACHIEVEMENTSVIEW_H
#define CAVER_OFFLINEACHIEVEMENTSVIEW_H

#include <boost/shared_ptr.hpp>
#include <vector>

#include "GUIView.h"

namespace Caver {

class GUIButton;
class GUIFrameView;
class GUILabel;
class GUIScrollView;
class OfflineAchievementView;
class PlayerProfile;

class OfflineAchievementsView : public GUIView {
public:
    OfflineAchievementsView(
        const boost::shared_ptr<PlayerProfile>& profile);
    ~OfflineAchievementsView() override;  // slots 7, 8

    void HandleKeyboardEvent(const GUIKeyboardEvent& event) override;  // 5
    void LayoutSubviews() override;                                    // 11
    void DidMoveToWindow() override;                                   // 16
    bool CanBecomeFirstResponder() override;                           // 24

    void SetGoogleServicesButtonsVisible(bool visible);
    void AnimateIn();
    void AnimateOut();

    boost::shared_ptr<GUIButton> closeButton;       // +256
    boost::shared_ptr<GUILabel> titleLabel;         // +272
    boost::shared_ptr<GUIFrameView> dividerView;    // +288
    boost::shared_ptr<GUIScrollView> scrollView;    // +304
    std::vector<boost::shared_ptr<OfflineAchievementView> >
        achievementRows;                            // +320
    boost::shared_ptr<GUIButton> googleButton;      // +344
    boost::shared_ptr<GUIButton> syncButton;        // +360
};

} // namespace Caver

#endif
