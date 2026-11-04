// Recovered from libswordigo.so (1.4.13) evidence — IDA/Hex-Rays
// exports in ~/workspace/analysis/swordigo/functions/Caver/
// TargetFrame/ and the golden vtable @0x62F688 (overrides:
// ~TargetFrame slots 7/8, Update slot 9, DrawRect slot 10,
// LayoutSubviews slot 11).
//
// TargetFrame: the floating name/health plate shown over the
// current target. Derives GUIView (pinned layout, sizeof 256).
//
// Layout: GUIView (0..255), then
//   +252 float m_timer — overlaps the GUIView base's last four
//        bytes: the ctor stores one qword at +252 whose low dword
//        is FLT_MAX (0x7F7FFFFF) and high dword is 0, so the timer
//        starts at FLT_MAX and m_characterLevel at 0.
//   +256 int m_characterLevel
//   +264 std::string m_name
//   +288 int m_targetLevel
//   +292 bool m_field292 (UNRECOVERED name; the level-hidden flag:
//        set by SetTargetInfo, forced on by UpdateInfo when the
//        level difference is 5+)
//   +296 float m_health (ctor -1.0)
//   +300 bool m_shown
//   +304 boost::shared_ptr<GUISlider> m_slider
//   +320 FontText m_nameText (240 bytes)
//   +560 FontText m_healthText (240 bytes)
//   +800 boost::shared_ptr<GUIAnimation> m_hideAnimation
// sizeof 816 (ctor/dtor evidence).
#ifndef CAVER_TARGETFRAME_H
#define CAVER_TARGETFRAME_H

#include <string>

#include <boost/shared_ptr.hpp>

#include "FontText.h"
#include "GUIView.h"

namespace Caver {

class GUIAnimation;
class GUIEvent;
class GUISlider;

class TargetFrame : public GUIView {
public:
    TargetFrame();
    ~TargetFrame() override;

    void Update(float deltaTime) override;              // slot 9
    void DrawRect(RenderingContext* context, const Rectangle& rect,
                  const Matrix4& transform) override;   // slot 10
    void LayoutSubviews() override;                     // slot 11

    void SetCharacterLevel(int level);
    void SetTargetInfo(const std::string& name, int level, bool hideLevel);
    void UpdateInfo();
    void SetHealth(float health);
    void UpdateHealthText();
    void Show();
    void Hide();
    void HideAnimationFinished(void* sender, GUIEvent* event);
    void SetFrameWidth(float width);

    int m_characterLevel;                            // +256
    std::string m_name;                              // +264
    int m_targetLevel;                               // +288
    bool m_field292;                                 // +292 UNRECOVERED
    float m_health;                                  // +296
    bool m_shown;                                    // +300
    boost::shared_ptr<GUISlider> m_slider;           // +304
    FontText m_nameText;                             // +320
    FontText m_healthText;                           // +560
    boost::shared_ptr<GUIAnimation> m_hideAnimation; // +800
};

static_assert(sizeof(TargetFrame) == 816, "TargetFrame size mismatch");

} // namespace Caver

#endif
