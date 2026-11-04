#ifndef CAVER_GUIVECTORSLIDER_H
#define CAVER_GUIVECTORSLIDER_H

#include "GUIControl.h"

namespace Caver {

class GUIVectorSlider : public GUIControl {
public:
    explicit GUIVectorSlider(float minVal = 0.0f, float maxVal = 1.0f);
    ~GUIVectorSlider() override;

    void TouchBegan(const FWTouch& touch) override;
    void TouchMoved(const FWTouch& touch) override;
    void TouchEnded(const FWTouch& touch) override;
    void TouchCancelled(const FWTouch& touch) override;

    void DrawRect(RenderingContext* context, const Rectangle& rect,
                  const Matrix4& transform) override;
    Vector2 SizeThatFits(const Vector2& size);

    float Value() const { return m_value; }
    void SetValue(float val);

    float MinValue() const { return m_minValue; }
    void SetMinValue(float val) { m_minValue = val; }

    float MaxValue() const { return m_maxValue; }
    void SetMaxValue(float val) { m_maxValue = val; }

private:
    void UpdateFromTouch(const FWTouch& touch);

    float m_value;
    float m_minValue;
    float m_maxValue;
    bool m_tracking;
    unsigned long long m_touchId;
};

} // namespace Caver

#endif // CAVER_GUIVECTORSLIDER_H
