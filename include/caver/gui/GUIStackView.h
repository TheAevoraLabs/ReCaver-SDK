#ifndef CAVER_GUISTACKVIEW_H
#define CAVER_GUISTACKVIEW_H

#include <vector>
#include <boost/shared_ptr.hpp>

#include "GUIView.h"
#include "GUIDivider.h"
#include "GUISpacer.h"

namespace Caver {

enum class StackAxis {
    Horizontal,
    Vertical
};

enum class StackAlignment {
    Leading,   // Left (vertical) / Top (horizontal)
    Center,    // Center on cross axis
    Trailing,  // Right (vertical) / Bottom (horizontal)
    Fill       // Fill cross axis
};

enum class StackDistribution {
    Leading,
    Center,
    Trailing,
    Fill,
    SpaceBetween
};

struct StackItem {
    boost::shared_ptr<GUIView> view;
    float flex;
    StackAlignment alignment;
    float minSize;
    float maxSize;
    bool customSize;
    float explicitSize;
};

class GUIStackView : public GUIView {
public:
    GUIStackView(StackAxis axis = StackAxis::Vertical, float spacing = 8.0f);
    ~GUIStackView() override;

    void SetAxis(StackAxis axis) { m_axis = axis; }
    StackAxis Axis() const { return m_axis; }

    void SetSpacing(float spacing) { m_spacing = spacing; }
    float Spacing() const { return m_spacing; }

    void SetAlignment(StackAlignment alignment) { m_alignment = alignment; }
    StackAlignment Alignment() const { return m_alignment; }

    void SetDistribution(StackDistribution distribution) { m_distribution = distribution; }
    StackDistribution Distribution() const { return m_distribution; }

    void SetPadding(float top, float right, float bottom, float left);
    void SetPadding(float uniformPadding);

    void AddArrangedSubview(const boost::shared_ptr<GUIView>& view,
                            float flex = 0.0f,
                            StackAlignment alignment = StackAlignment::Center,
                            float minSize = 0.0f,
                            float maxSize = 1e6f);

    boost::shared_ptr<GUISpacer> AddSpacer(float minSize = 0.0f, float flex = 1.0f);
    void AddFixedSpacing(float size);
    boost::shared_ptr<GUIDivider> AddDivider(const Color& color = Color(85, 75, 65, 180),
                                            float thickness = 2.0f);

    void AddSubview(const boost::shared_ptr<GUIView>& subview) override;
    void RemoveAllArrangedSubviews();

    Vector2 ComputeFittingSize() const;
    void LayoutSubviews() override;

    static boost::shared_ptr<GUIStackView> HStack(float spacing = 8.0f);
    static boost::shared_ptr<GUIStackView> VStack(float spacing = 8.0f);

private:
    StackAxis m_axis;
    float m_spacing;
    StackAlignment m_alignment;
    StackDistribution m_distribution;

    float m_paddingTop;
    float m_paddingRight;
    float m_paddingBottom;
    float m_paddingLeft;

    std::vector<StackItem> m_items;

    Vector2 MeasureItem(GUIView* view, const StackItem& item) const;
};

} // namespace Caver

#endif // CAVER_GUISTACKVIEW_H
