// Recovered from libswordigo.so (1.4.13) evidence.
//
// GUIFrameView: a GUIView with a nine-patch/textured frame backdrop.
// Vtable 320 bytes @0x634B48 — the 38 GUIView slots with only
// slot 7/8 (~GUIFrameView), slot 10 (DrawRect) and slot 11
// (LayoutSubviews) overridden (ABS64 relocation dump).
//
// Layout: GUIView (256 bytes), then
//   +256 boost::scoped_ptr<GUITexturedRect> texturedRect
//   +264 boost::scoped_ptr<GUIRoundedRect> roundedRect
//   +272 boost::intrusive_ptr<Texture> frameTexture
//   +280 bool centered (DrawRect wraps the frame draw in a
//        translate-to-centre / translate-back pair when set)
// The destructor is implicit in the original: the golden D2 @0x44605C is
// exactly the teardown of these three members (in reverse order) plus
// ~GUIView. The frame colour lives in the GUIView tail (dword at +249),
// written by the ctor (-1) and by SetFrameColor.
#ifndef CAVER_GUIFRAMEVIEW_H
#define CAVER_GUIFRAMEVIEW_H

#include <boost/intrusive_ptr.hpp>
#include <boost/scoped_ptr.hpp>

#include "../core/Color.h"
#include "../render/Texture.h"
#include "GUITexturedRect.h"
#include "GUIView.h"

namespace Caver {

class GUIRoundedRect;

class GUIFrameView : public GUIView {
public:
    GUIFrameView();

    void LayoutSubviews() override;  // slot 11
    void DrawRect(RenderingContext* context, const Rectangle& rect,
                  const Matrix4& transform) override;  // slot 10

    void SetFrameTexture(const boost::intrusive_ptr<Texture>& texture,
                         bool tiled);
    void SetFrameColor(const Color& color);

    boost::scoped_ptr<GUITexturedRect> texturedRect;  // +256
    boost::scoped_ptr<GUIRoundedRect> roundedRect;    // +264
    boost::intrusive_ptr<Texture> frameTexture;       // +272
    bool centered;                                    // +280
};

} // namespace Caver

#endif
