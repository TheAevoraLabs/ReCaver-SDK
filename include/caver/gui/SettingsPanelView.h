// SettingsPanelView.h - Native Caver Engine Settings Screen.
// Credit: Quantum Creeper / Aevora Labs.
#ifndef CAVER_SETTINGSPANELVIEW_H
#define CAVER_SETTINGSPANELVIEW_H

#include <vector>
#include <string>
#include <boost/shared_ptr.hpp>
#include <boost/intrusive_ptr.hpp>

#include "GUIView.h"
#include "GUIButton.h"
#include "GUIFrameView.h"
#include "GUILabel.h"
#include "GUIVectorButton.h"
#include "GUIVectorSlider.h"
#include "GUIVectorSwitch.h"

namespace Caver {

class SettingsPanelView : public GUIView {
public:
    SettingsPanelView();
    ~SettingsPanelView() override;

    void LayoutSubviews() override;
    void DrawRect(RenderingContext* context, const Rectangle& rect,
                  const Matrix4& transform) override;
    void AnimateIn();
    void AnimateOut();
    bool CanBecomeFirstResponder() override { return true; }

    void SelectTab(int index);
    int CurrentTab() const { return m_currentTab; }

    void UpdateAudioControls();
    void UpdateDisplayControls();
    void UpdateRenderControls();

    // Dialog Shell
    boost::shared_ptr<GUIFrameView> backgroundFrame;
    boost::shared_ptr<GUILabel> titleLabel;
    boost::shared_ptr<GUIButton> closeButton;

    // Tab Bar (0: AUDIO, 1: DISPLAY, 2: RENDER, 3: CONTROLS)
    boost::shared_ptr<GUIButton> audioTabBtn;
    boost::shared_ptr<GUIButton> displayTabBtn;
    boost::shared_ptr<GUIButton> renderTabBtn;
    boost::shared_ptr<GUIButton> controlsTabBtn;

    // Tab Containers
    boost::shared_ptr<GUIView> audioPage;
    boost::shared_ptr<GUIView> displayPage;
    boost::shared_ptr<GUIView> renderPage;
    boost::shared_ptr<GUIView> controlsPage;

    // --- Audio Tab ---
    boost::shared_ptr<GUIButton> musicButton;
    boost::shared_ptr<GUILabel> musicLabel;
    boost::shared_ptr<GUIVectorSlider> musicSlider;

    boost::shared_ptr<GUIButton> soundButton;
    boost::shared_ptr<GUILabel> soundLabel;
    boost::shared_ptr<GUIVectorSlider> soundSlider;

    boost::shared_ptr<GUILabel> muteLabel;
    boost::shared_ptr<GUIVectorSwitch> muteSwitch;

    // --- Display Tab ---
    boost::shared_ptr<GUILabel> fullscreenLabel;
    boost::shared_ptr<GUIVectorSwitch> fullscreenSwitch;

    boost::shared_ptr<GUILabel> resolutionLabel;
    boost::shared_ptr<GUIVectorButton> resolutionBtn;

    boost::shared_ptr<GUILabel> vsyncLabel;
    boost::shared_ptr<GUIVectorSwitch> vsyncSwitch;

    boost::shared_ptr<GUILabel> scaleLabel;
    boost::shared_ptr<GUIVectorSlider> scaleSlider;

    // --- Render Tab ---
    boost::shared_ptr<GUILabel> rendererTitleLabel;
    boost::shared_ptr<GUILabel> rendererNameLabel;

    boost::shared_ptr<GUILabel> msaaLabel;
    boost::shared_ptr<GUIVectorButton> msaaBtn;

    boost::shared_ptr<GUILabel> ssaoLabel;
    boost::shared_ptr<GUIVectorSwitch> ssaoSwitch;

    boost::shared_ptr<GUILabel> bloomLabel;
    boost::shared_ptr<GUIVectorSwitch> bloomSwitch;

    boost::shared_ptr<GUILabel> fxaaLabel;
    boost::shared_ptr<GUIVectorSwitch> fxaaSwitch;

    boost::shared_ptr<GUILabel> shadowsLabel;
    boost::shared_ptr<GUIVectorSwitch> shadowsSwitch;

    boost::shared_ptr<GUILabel> renderNoteLabel;

    // --- Controls Tab ---
    boost::shared_ptr<GUIButton> configureControlsBtn;
    boost::shared_ptr<GUILabel> touchControlsLabel;
    boost::shared_ptr<GUIVectorSwitch> touchControlsSwitch;
    boost::shared_ptr<GUILabel> controlsHelpLabel;

    boost::intrusive_ptr<Texture> musicOnTexture;
    boost::intrusive_ptr<Texture> musicOffTexture;
    boost::intrusive_ptr<Texture> soundOnTexture;
    boost::intrusive_ptr<Texture> soundOffTexture;

    void UpdateTabStyles();

private:
    int m_currentTab;
};

} // namespace Caver

#endif // CAVER_SETTINGSPANELVIEW_H
