#ifndef CAVER_GUIVECTORBADGE_H
#define CAVER_GUIVECTORBADGE_H

#include <string>
#include <boost/shared_ptr.hpp>
#include "GUIView.h"
#include "../core/Color.h"

namespace Caver {

class GUILabel;

class GUIVectorBadge : public GUIView {
public:
    explicit GUIVectorBadge(const std::string& text = "", const Color& color = Color(110, 220, 140, 255));
    ~GUIVectorBadge() override;

    void DrawRect(RenderingContext* context, const Rectangle& rect,
                  const Matrix4& transform) override;
    void LayoutSubviews() override;

    void SetText(const std::string& text);
    void SetBadgeColor(const Color& color);

private:
    std::string m_text;
    Color m_color;
    boost::shared_ptr<GUILabel> m_label;
};

} // namespace Caver

#endif // CAVER_GUIVECTORBADGE_H
