#ifndef CAVER_GUISPACER_H
#define CAVER_GUISPACER_H

#include "GUIView.h"

namespace Caver {

class GUISpacer : public GUIView {
public:
    GUISpacer(float minSize = 0.0f, float flex = 1.0f);
    ~GUISpacer() override;

    void DrawRect(RenderingContext* context, const Rectangle& rect,
                  const Matrix4& transform) override;

    float m_minSize;
    float m_flex;
};

} // namespace Caver

#endif // CAVER_GUISPACER_H
