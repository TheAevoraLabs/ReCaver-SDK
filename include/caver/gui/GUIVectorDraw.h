#ifndef CAVER_GUIVECTORDRAW_H
#define CAVER_GUIVECTORDRAW_H

#include "../core/Color.h"
#include "../core/Rectangle.h"
#include "../core/Vector2.h"
#include "../render/RenderingContext.h"
#include <string>

namespace Caver {

class Font;

class GUIVectorDraw {
public:
    // Core primitives
    static void FillGradientV(RenderingContext* context, const Rectangle& rect,
                              const Color& topColor, const Color& bottomColor);

    static void FillGradientH(RenderingContext* context, const Rectangle& rect,
                              const Color& leftColor, const Color& rightColor);

    static void StrokeRect(RenderingContext* context, const Rectangle& rect,
                           const Color& color, float width = 1.0f);

    static void FillRoundedRect(RenderingContext* context, const Rectangle& rect,
                                float radius, const Color& fillColor);

    static void FillRoundedRectGradient(RenderingContext* context, const Rectangle& rect,
                                        float radius, const Color& topColor, const Color& bottomColor);

    static void StrokeRoundedRect(RenderingContext* context, const Rectangle& rect,
                                  float radius, const Color& color, float width = 1.0f);

    static void FillCircle(RenderingContext* context, const Vector2& center,
                           float radius, const Color& color, int segments = 24);

    static void StrokeCircle(RenderingContext* context, const Vector2& center,
                             float radius, const Color& color, float width = 1.5f, int segments = 24);

    // Iconic Swordigo / Caver themed components
    static void DrawBannerStripe(RenderingContext* context, const Rectangle& rect,
                                const std::string& title, Font* font = nullptr);

    static void DrawCardFrame(RenderingContext* context, const Rectangle& rect,
                             float cornerRadius = 6.0f);

    static void DrawFramedButton(RenderingContext* context, const Rectangle& rect,
                                 bool isPressed, bool isHighlighted,
                                 const std::string& title, Font* font = nullptr);

    static void DrawCapsuleSwitch(RenderingContext* context, const Rectangle& rect,
                                  bool isOn, bool isPressed);

    static void DrawSliderTrack(RenderingContext* context, const Rectangle& rect,
                                float progress01, bool isDragging);

    static void DrawStatusBadge(RenderingContext* context, const Rectangle& rect,
                                const std::string& text, const Color& badgeColor,
                                Font* font = nullptr);
};

} // namespace Caver

#endif // CAVER_GUIVECTORDRAW_H
