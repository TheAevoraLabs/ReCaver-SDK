#ifndef CAVER_EXPERIENCEBAR_H
#define CAVER_EXPERIENCEBAR_H

#include <string>
#include <boost/shared_ptr.hpp>

#include "GUIView.h"
#include "GUISlider.h"
#include "FontText.h"

namespace Caver {

class GameState;
class GUIAnimation;

class ExperienceBar : public GUIView {
public:
    ExperienceBar();
    ~ExperienceBar() override;

    void LayoutSubviews() override;
    void Update(float deltaTime) override;
    void DrawRect(RenderingContext* context, const Rectangle& rect,
                  const Matrix4& transform) override;

    void InitWithGameState(const boost::shared_ptr<GameState>& gameState);
    void UpdateExperience();
    void SetExperience(int level, int currentExp, int prevLevelExp, int nextLevelExp);
    void UpdateText();

    boost::shared_ptr<GUISlider> slider() const { return m_slider; }
    int level() const { return m_level; }
    int currentExp() const { return m_currentExp; }

    boost::shared_ptr<GUISlider> m_slider;       // +256
    boost::shared_ptr<GameState> m_gameState;    // +272
    bool field288;                               // +288 (level up mode)
    char _pad289[3];                             // +289..291
    int m_level;                                 // +292
    int m_currentExp;                            // +296
    int m_prevLevelExp;                          // +300
    int m_nextLevelExp;                          // +304
    int m_expToGain;                             // +308
    FontText m_fontText[3];                      // +312, +552, +792
    bool m_field1032;                            // +1032
    bool m_field1033;                            // +1033
    char _pad1034[6];                            // +1034..1039
    boost::shared_ptr<GUIAnimation> m_animation; // +1040
};

static_assert(sizeof(ExperienceBar) == 1056, "ExperienceBar layout mismatch");

} // namespace Caver

#endif // CAVER_EXPERIENCEBAR_H
