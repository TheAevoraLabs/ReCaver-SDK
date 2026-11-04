#ifndef CAVER_GUIVECTORBUTTON_H
#define CAVER_GUIVECTORBUTTON_H

#include <string>
#include "GUIButton.h"

namespace Caver {

class GUIVectorButton : public GUIButton {
public:
    explicit GUIVectorButton(const std::string& title = "", float width = 160.0f, float height = 32.0f);
    ~GUIVectorButton() override;

    void DrawRect(RenderingContext* context, const Rectangle& rect,
                  const Matrix4& transform) override;
    void LayoutSubviews() override;
    Vector2 SizeThatFits(const Vector2& size);

    void SetCustomTitle(const std::string& title);
    void SetSelected(bool selected) { m_selected = selected; }
    bool IsSelected() const { return m_selected; }

private:
    bool m_selected;
};

} // namespace Caver

#endif // CAVER_GUIVECTORBUTTON_H
