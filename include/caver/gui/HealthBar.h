#ifndef CAVER_HEALTHBAR_H
#define CAVER_HEALTHBAR_H

#include <vector>
#include <boost/intrusive_ptr.hpp>
#include <boost/shared_ptr.hpp>

#include "GUIView.h"
#include "GUIAnimation.h"
#include "../render/Sprite.h"
#include "../render/Texture.h"
#include "../animation/ParticleSystem.h"

namespace Caver {

class HealthBar : public GUIView {
public:
    HealthBar();
    ~HealthBar() override;

    void LayoutSubviews() override;
    void Update(float deltaTime) override;
    void DrawRect(RenderingContext* context, const Rectangle& rect,
                  const Matrix4& transform) override;

    void UpdateSprite();
    void SetMaxHealth(int maxHealth);
    void SetCurrentHealth(int currentHealth);

    void AddAnimationToHeart(int index, const boost::shared_ptr<GUIAnimation>& anim);
    void ShatterHeart(int index);
    void PopHeart(int index);
    Vector2 PositionOfHeartAtIndex(int index);

    int maxHealth() const { return m_maxHealth; }
    int currentHealth() const { return m_currentHealth; }
    const Vector2& heartSize() const { return m_heartSize; }

private:
    boost::intrusive_ptr<Texture> m_textureHeartFull;              // +256
    boost::intrusive_ptr<Texture> m_textureHeartHalf;              // +264
    boost::intrusive_ptr<Texture> m_textureHeartEmpty;             // +272
    bool m_centered;                                               // +280
    int m_maxHealth;                                               // +284
    int m_currentHealth;                                           // +288
    int m_displayedHealth;                                         // +292
    float m_lowHealthTimer;                                        // +296
    Vector2 m_heartSize;                                           // +300
    Sprite m_sprite;                                               // +312
    boost::shared_ptr<ParticleSystem> m_particleSystem;            // +400
    std::vector<boost::shared_ptr<GUIAnimation>> m_heartAnimations; // +416
    float m_field440;                                              // +440
};

} // namespace Caver

#endif // CAVER_HEALTHBAR_H
