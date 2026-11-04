// Recovered from libswordigo.so (1.4.13) evidence — IDA/Hex-Rays exports
// in ~/workspace/analysis/swordigo/functions/Caver/
// AchievementNotificationView/, the ctor @0x445574, the dtor @0x44708C
// teardown order, and the LayoutSubviews/DrawRect tail thunks.
//
// AchievementNotificationView: the slide-in toast shown when an
// achievement unlocks. Single base Caver::GUIFrameView (the ctor calls
// GUIFrameView's ctor, the dtor's tail tears down the GUIFrameView
// members at +256/+264/+272 and re-seats the vptr on GUIFrameView's
// vtable, and slots 10/11 tail-branch to GUIFrameView::DrawRect /
// GUIFrameView::LayoutSubviews). NOT related to NotificationView
// (a separate GameSceneView member class) beyond both being GUIViews.
//
// Vtable: the 38 GUIView slots; overrides are only
//   7/8  ~AchievementNotificationView (D2 @0x44708C, D0 @0x447380)
//   9    Update @0x446744 — 4-byte tail branch to GUIView::Update
//   10   DrawRect @0x446748 — 4-byte tail branch to GUIFrameView::DrawRect
//   11   LayoutSubviews @0x4464F0 — 4-byte tail branch to
//        GUIFrameView::LayoutSubviews
//
// Layout: GUIFrameView (288 bytes), then (dtor teardown order,
// SetAchievement/UpdateLayout accesses):
//   +288 boost::shared_ptr<Achievement>   achievement
//   +304 boost::shared_ptr<GUIAnimation>  animate-in animation
//        (AnimationDidFinish compares its .get() against the sender)
//   +320 boost::shared_ptr<GUIAnimation>  animate-out animation
//   +336 boost::shared_ptr<GUIFrameView>  icon view (frame texture is
//        the achievement icon; sized from the texture's sub-rect,
//        30x30 when there is no texture)
//   +352 boost::shared_ptr<GUILabel>      label fed achievement string
//        at +24 (upper label in UpdateLayout)
//   +368 boost::shared_ptr<GUILabel>      label fed achievement string
//        at +48 (SmallDefaultFont, colour (200,180,160,255), wrapped
//        220-wide lower label in UpdateLayout)
//   +384 boost::shared_ptr<GUILabel>      points label ("<points> pts",
//        carries the TextGradient(0.15, 1.0, 0.85, 1.0) gradient)
// sizeof = 400. Original member names UNRECOVERED.
#ifndef CAVER_ACHIEVEMENTNOTIFICATIONVIEW_H
#define CAVER_ACHIEVEMENTNOTIFICATIONVIEW_H

#include <boost/shared_ptr.hpp>
#include <string>

#include "GUIFrameView.h"
#include "caver/systems/Achievement.h"

namespace Caver {

class GUIAnimation;
class GUIEvent;
class GUILabel;

class AchievementNotificationView : public GUIFrameView {
public:
    AchievementNotificationView();
    ~AchievementNotificationView() override;  // slots 7, 8

    void Update(float deltaTime) override;  // slot 9 (tail thunk)
    void DrawRect(RenderingContext* context, const Rectangle& rect,
                  const Matrix4& transform) override;  // slot 10 (thunk)
    void LayoutSubviews() override;                      // slot 11 (thunk)

    void SetAchievement(const boost::shared_ptr<Achievement>& achievement);
    void UpdateLayout();
    void PresentInView(GUIView* view);
    void AnimateIn();
    void AnimationDidFinish(void* sender, GUIEvent* event);
    void AnimateOutAfterDelay(float delay);

    boost::shared_ptr<Achievement> achievement;        // +288
    boost::shared_ptr<GUIAnimation> animateInAnimation;   // +304
    boost::shared_ptr<GUIAnimation> animateOutAnimation;  // +320
    boost::shared_ptr<GUIFrameView> iconView;             // +336
    boost::shared_ptr<GUILabel> nameLabel;                // +352
    boost::shared_ptr<GUILabel> descriptionLabel;         // +368
    boost::shared_ptr<GUILabel> pointsLabel;              // +384
};

} // namespace Caver

#endif
