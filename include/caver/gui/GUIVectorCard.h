#ifndef CAVER_GUIVECTORCARD_H
#define CAVER_GUIVECTORCARD_H

#include <string>
#include <boost/shared_ptr.hpp>

#include "GUIView.h"
#include "GUIStackView.h"

namespace Caver {

class GUILabel;

class GUIVectorCard : public GUIView {
public:
    explicit GUIVectorCard(const std::string& title = "", float spacing = 10.0f);
    ~GUIVectorCard() override;

    void DrawRect(RenderingContext* context, const Rectangle& rect,
                  const Matrix4& transform) override;
    void LayoutSubviews() override;
    Vector2 SizeThatFits(const Vector2& size);

    void SetTitle(const std::string& title);
    const std::string& Title() const { return m_title; }

    void AddArrangedSubview(const boost::shared_ptr<GUIView>& view,
                            float flex = 0.0f,
                            StackAlignment align = StackAlignment::Fill,
                            float minSize = 0.0f,
                            float maxSize = 0.0f);

    boost::shared_ptr<GUIStackView> ContentStack() const { return m_contentStack; }

private:
    std::string m_title;
    boost::shared_ptr<GUILabel> m_titleLabel;
    boost::shared_ptr<GUIStackView> m_contentStack;
};

} // namespace Caver

#endif // CAVER_GUIVECTORCARD_H
