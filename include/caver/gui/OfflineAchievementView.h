// Recovered from libswordigo.so (1.4.13) evidence — IDA/Hex-Rays exports
// in ~/workspace/analysis/swordigo/functions/Caver/
// OfflineAchievementView/ (ctor @0x451B28, SetAchievement @0x45262C,
// LayoutSubviews @0x452EC8, dtor D2 @0x45314C, D0 @0x4533C8) and the
// golden vtable @0x62C440.
//
// OfflineAchievementView (SINGULAR): one achievement row inside
// OfflineAchievementsView's scroll view. NOT OfflineAchievementsView
// (plural, the screen) and NOT AchievementNotificationView (the toast).
//
// Single base Caver::GUIView (the ctor calls GUIView's ctor, the dtor
// tail-calls GUIView::~GUIView; the vtable @0x62C440 is the 38 GUIView
// slots with only slots 7/8 (~OfflineAchievementView) and 11
// (LayoutSubviews) overridden).
//
// Layout: GUIView (256 bytes), then (ctor assignments, dtor teardown
// order, and every method's accesses agree):
//   +256 boost::shared_ptr<Achievement> achievement (stored by
//        SetAchievement; the ctor leaves it null)
//   +272 bool unlocked (SetAchievement's bool arg; NOT zeroed by the
//        ctor — the golden ctor stores nothing at +272/+276)
//   +276 float percent (SetAchievement's float arg; same note)
//   +280 boost::shared_ptr<GUIFrameView> iconView (frame texture is
//        "achicon_<field0>" when unlocked, "achicon_notcomplete_Frame"
//        when locked; framed 34x34 in SetAchievement)
//   +296 boost::shared_ptr<GUILabel> progressLabel (shows "?" in
//        LargeDefaultFont when percent <= 0, else "<int>% " in
//        MediumSmallDefaultFont; hidden once the achievement unlocks;
//        colour (153,153,153,153), shadow disabled, laid out over the
//        icon frame)
//   +312 boost::shared_ptr<GUIView> contentView (container holding
//        nameLabel + descriptionLabel; sized to the text stack and
//        vertically centred against the row bounds)
//   +328 boost::shared_ptr<GUILabel> nameLabel (fed achievement string
//        at +24; laid out BELOW descriptionLabel in LayoutSubviews)
//   +344 boost::shared_ptr<GUILabel> descriptionLabel (fed achievement
//        string at +48; SmallDefaultFont, colour (200,180,160,255),
//        field348 = 2, field344 = true)
//   +360 boost::shared_ptr<GUILabel> pointsLabel ("<points> pts";
//        right-aligned, 60 wide, font-height tall; gets the
//        TextGradient(0.15, 1.0, 0.85, 1.0) gradient when unlocked,
//        the (153,153,153,153) colour when locked)
// sizeof = 376. Original member names UNRECOVERED — the names above
// are behaviour-derived.
//
// Achievement layout (this header SUPERSEDES the TU-local definitions
// in AchievementNotificationView.h and OfflineAchievementsView.cpp —
// the plural view's worker extended it with the counter string @+80
// and required-count int @+104, both read by the plural ctor):
//   +0   std::string field0 (icon base name)
//   +24  std::string field24 (fed to nameLabel)
//   +48  std::string field48 (fed to descriptionLabel)
//   +72  int points
//   +80  std::string counter (counter name; read by the plural view)
//   +104 int required (required count; read by the plural view)
// sizeof = 112. Field names UNRECOVERED.
#ifndef CAVER_OFFLINEACHIEVEMENTVIEW_H
#define CAVER_OFFLINEACHIEVEMENTVIEW_H

#include <string>

#include <boost/shared_ptr.hpp>

#include "GUIFrameView.h"
#include "GUILabel.h"
#include "GUIView.h"
#include "../systems/Achievement.h"

namespace Caver {

class OfflineAchievementView : public GUIView {
public:
    OfflineAchievementView();
    ~OfflineAchievementView() override;  // slots 7, 8

    void LayoutSubviews() override;  // slot 11

    void SetAchievement(const boost::shared_ptr<Achievement>& achievement,
                        bool unlocked, float percent);

    boost::shared_ptr<Achievement> achievement;   // +256
    bool unlocked;                                // +272
    char _pad273[3];                              // +273
    float percent;                                // +276
    boost::shared_ptr<GUIFrameView> iconView;     // +280
    boost::shared_ptr<GUILabel> progressLabel;    // +296
    boost::shared_ptr<GUIView> contentView;       // +312
    boost::shared_ptr<GUILabel> nameLabel;        // +328
    boost::shared_ptr<GUILabel> descriptionLabel; // +344
    boost::shared_ptr<GUILabel> pointsLabel;      // +360
};

// Golden aarch64 NDK-libc++ layout anchor; MSVC STL member sizes differ,
// so this assert is Android/Linux-only (the Windows build is self-consistent
// under MSVC STL and does not claim golden byte-layout parity).
#ifndef _MSC_VER
static_assert(sizeof(OfflineAchievementView) == 376,
              "OfflineAchievementView layout must match the golden binary");
#endif

} // namespace Caver

#endif
