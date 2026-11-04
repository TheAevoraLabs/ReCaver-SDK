#ifndef CAVER_OVERLAYBUTTON_H
#define CAVER_OVERLAYBUTTON_H

#include <string>
#include <boost/shared_ptr.hpp>

#include "GUIButton.h"
#include "../animation/ParticleSystem.h"
#include "../animation/FountainParticleEmitter.h"

namespace Caver {

class ManaBar;

class OverlayButton : public GUIButton {
public:
    OverlayButton();
    ~OverlayButton() override;

    void FlashOnce();
    void Update(float deltaTime) override;
    void DrawRect(RenderingContext* context, const Rectangle& rect,
                  const Matrix4& transform) override;

    void* m_field472;                      // +472
    bool m_field480;                       // +480
    boost::shared_ptr<ManaBar> m_manaBar;  // +488
};

class OverlayMenuButton : public OverlayButton {
public:
    OverlayMenuButton();
    ~OverlayMenuButton() override;

    void SetMenuTab(const std::string& tab);
    void SetSparkling(bool sparkling);

    // _ZN5Caver17OverlayMenuButton24FadeOutAnimationFinishedEPvPNS_8GUIEventE
    // @0x495074, 1,584 bytes: when the button is sparkling (+528), scales
    // it 1.2 -> 1.0 over 0.3 s and binds this pair's other half.
    void FadeOutAnimationFinished(void* target, GUIEvent* event);

    // _ZN5Caver17OverlayMenuButton23FadeInAnimationFinishedEPvPNS_8GUIEventE
    // @0x4956AC, 1,592 bytes: the mirror image - 1.0 -> 1.2 over 0.3 s
    // after a 0.5 s delay - so the two callbacks pulse the button
    // forever once SetSparkling(true) has armed the flag.
    void FadeInAnimationFinished(void* target, GUIEvent* event);
    void Update(float deltaTime) override;
    void DrawRect(RenderingContext* context, const Rectangle& rect,
                  const Matrix4& transform) override;

private:
    std::string m_menuTab;                                      // +504
    bool m_sparkling;                                           // +528
    char _pad529[7];                                            // +529..535
    boost::shared_ptr<ParticleSystem> m_particleSystem;         // +536
    boost::shared_ptr<FountainParticleEmitter> m_emitter;       // +552
};

static_assert(sizeof(OverlayMenuButton) == 568, "OverlayMenuButton size mismatch");

} // namespace Caver

#endif // CAVER_OVERLAYBUTTON_H
