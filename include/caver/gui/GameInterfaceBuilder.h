// Recovered from libswordigo.so (1.4.13) evidence — IDA/Hex-Rays
// exports in ~/workspace/analysis/swordigo/functions/Caver/
// GameInterfaceBuilder/. GameInterfaceBuilder is a stateless
// factory: every function is static, the mangled names carry no
// instance state, and the bodies build gui-lane widgets from the
// texture/font libraries. All texture names and label strings are
// copied from the binary's string references exactly.
#ifndef CAVER_GAMEINTERFACEBUILDER_H
#define CAVER_GAMEINTERFACEBUILDER_H

#include <string>

#include <boost/intrusive_ptr.hpp>
#include <boost/shared_ptr.hpp>

#include "../core/Color.h"

namespace Caver {

class GradientColor;
class GUIAlertView;
class GUIButton;
class GUILabel;
class GUISlider;
class GUISwitch;
class GUIBubbleView;
class Texture;

class GameInterfaceBuilder {
public:
    static boost::shared_ptr<GUILabel> NormalLabel(const std::string& text,
                                                   Color color,
                                                   Color shadowColor);
    static boost::shared_ptr<GUILabel> SmallLabel(const std::string& text,
                                                  Color color,
                                                  Color shadowColor);
    static boost::shared_ptr<GUIButton> ControlButtonWithImage(
        const boost::intrusive_ptr<Texture>& image);
    static boost::shared_ptr<GUIButton> PlainButton(
        const boost::intrusive_ptr<Texture>& texture,
        const boost::intrusive_ptr<Texture>& pressedTexture);
    static boost::shared_ptr<GUIButton> IconButton(
        const boost::intrusive_ptr<Texture>& texture,
        const boost::intrusive_ptr<Texture>& pressedTexture);
    static boost::shared_ptr<GUIButton> FramedButtonWithTitle(
        const std::string& title, bool light);
    static boost::shared_ptr<GUIButton> MainMenuButtonWithTitle(
        const std::string& title);
    static boost::shared_ptr<GUIAlertView> AlertView(
        const std::string& title, const std::string& message, int buttonCount,
        const std::string* buttons, int defaultButtonIndex);
    static boost::shared_ptr<GUISlider> Slider(float minValue, float maxValue);
    static boost::shared_ptr<GUISwitch> Switch();
    static boost::shared_ptr<GradientColor> TextGradient(float hue,
                                                         float saturation,
                                                         float lightness,
                                                         float value);
    static boost::shared_ptr<GUIBubbleView> InfoBubble(const std::string& text,
                                                       float scale);
};

} // namespace Caver

#endif
