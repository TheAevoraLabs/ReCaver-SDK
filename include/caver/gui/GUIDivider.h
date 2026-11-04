#ifndef CAVER_GUIDIVIDER_H
#define CAVER_GUIDIVIDER_H

#include "../core/Color.h"
#include "GUIView.h"

namespace Caver {

class GUIDivider : public GUIView {
public:
    GUIDivider(const Color& color = Color(85, 75, 65, 180));
    ~GUIDivider() override;

    void DrawRect(RenderingContext* context, const Rectangle& rect,
                  const Matrix4& transform) override;

    Color m_color;
};

} // namespace Caver

#endif // CAVER_GUIDIVIDER_H
