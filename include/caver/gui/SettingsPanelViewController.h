#ifndef CAVER_SETTINGSPANELVIEWCONTROLLER_H
#define CAVER_SETTINGSPANELVIEWCONTROLLER_H

#include <boost/shared_ptr.hpp>

#include "GUIViewController.h"

namespace Caver {

class SettingsPanelView;
class GUIEvent;

class SettingsPanelViewController : public GUIViewController {
public:
    SettingsPanelViewController();
    ~SettingsPanelViewController() override;

    void LoadView() override;
    void ViewDidAppear() override;
    void Update(float deltaTime) override;

    void ButtonPressed(void* sender, GUIEvent* event);
    void SliderValueChanged(void* sender, GUIEvent* event);
    void SwitchValueChanged(void* sender, GUIEvent* event);

    boost::shared_ptr<SettingsPanelView> m_settingsView;
};

} // namespace Caver

#endif // CAVER_SETTINGSPANELVIEWCONTROLLER_H
