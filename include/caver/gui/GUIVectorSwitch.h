#ifndef CAVER_GUIVECTORSWITCH_H
#define CAVER_GUIVECTORSWITCH_H

#include <string>
#include "GUIControl.h"

namespace Caver {

class GUIVectorSwitch : public GUIControl {
public:
    explicit GUIVectorSwitch(bool on = false);
    ~GUIVectorSwitch() override;

    void TouchBegan(const FWTouch& touch) override;
    void TouchMoved(const FWTouch& touch) override;
    void TouchEnded(const FWTouch& touch) override;
    void TouchCancelled(const FWTouch& touch) override;

    void DrawRect(RenderingContext* context, const Rectangle& rect,
                  const Matrix4& transform) override;
    Vector2 SizeThatFits(const Vector2& size);

    bool IsOn() const { return m_on; }
    void SetOn(bool on);

    std::string soundName;

private:
    bool m_on;
    bool m_pressed;
    unsigned long long m_touchId;
};

} // namespace Caver

#endif // CAVER_GUIVECTORSWITCH_H
