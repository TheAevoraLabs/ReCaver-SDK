// Recovered from libswordigo.so (1.4.13) evidence.
//
// GUIBubbleView: a speech-bubble popup hosting a GUITextBubble.
// Vtable 320 bytes @0x634588 — the 38 GUIView slots with slot 0
// (TouchBegan), slot 7/8 (~GUIBubbleView), slot 9 (Update) and slot 10
// (DrawRect) overridden (ABS64 relocation dump).
//
// Layout: GUIView (256 bytes), then
//   +256 boost::shared_ptr<GUITextBubble> textBubble (created in the ctor)
//   +272 Vector2 anchorPoint (set by PresentFromRectInView)
//   +280 boost::shared_ptr<GUIAnimation> animation (the scale-in
//        animation created by PresentFromRectInView)
// The destructor is implicit in the original: the golden D2 @0x51DCD8 is
// exactly the two shared_ptr teardowns (animation first) plus ~GUIView.
#ifndef CAVER_GUIBUBBLEVIEW_H
#define CAVER_GUIBUBBLEVIEW_H

#include <boost/shared_ptr.hpp>

#include "../core/Vector2.h"
#include "GUIAnimation.h"
#include "GUIView.h"

namespace Caver {

class GUITextBubble;

class GUIBubbleView : public GUIView {
public:
    GUIBubbleView();

    void TouchBegan(const FWTouch& touch) override;  // slot 0
    void Update(float deltaTime) override;           // slot 9
    void DrawRect(RenderingContext* context, const Rectangle& rect,
                  const Matrix4& transform) override;  // slot 10

    void PresentFromRectInView(const Rectangle& rect, GUIView* view);

    boost::shared_ptr<GUITextBubble> textBubble;  // +256
    Vector2 anchorPoint;                          // +272
    boost::shared_ptr<GUIAnimation> animation;    // +280
};

} // namespace Caver

#endif
