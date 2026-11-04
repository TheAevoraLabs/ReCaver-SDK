// Recovered from libswordigo.so (1.4.13) evidence.
//
// GUIRoundedRect: a nine-patch rounded-rectangle backdrop: a texture,
// a tint colour, outer/inner rectangles, the texture's UV rect and the
// pixel insets derived from the texture, rendered through an embedded
// Sprite built by CreateSprite().
// NON-POLYMORPHIC: the golden exports no _ZTV/_ZTI/_ZTS for
// GUIRoundedRect (verified by symbol-table scan).
//
// Layout (168 bytes, proven by the ctor @0x531158, InitWithTexture
// @0x531268, CreateWithOuterBounds @0x53132C, CreateWithInnerBounds
// @0x5314D0, CreateSprite @0x531378 and the destructor @0x4852B4):
//   +0   boost::intrusive_ptr<Texture> texture
//   +8   Color color (ctor: dword -1, white)
//   +12  Rectangle outerRect (member name UNRECOVERED)
//   +28  Rectangle innerRect (member name UNRECOVERED)
//   +44  Rectangle uvRect (the texture's inner UV region, computed by
//        InitWithTexture; member name UNRECOVERED)
//   +60  float insets[4]: left (+60), right (+64), top (+68),
//        bottom (+72) — pixel insets computed by InitWithTexture
//        (floor/ceil of the texture's half size); NOT initialised by
//        the ctor
//   +80  Sprite sprite (88 bytes; its own texture at +80, its vertex
//        buffer at +112 and its index buffer at +136 are torn down by
//        the destructor, in that reverse order after the buffers)
#ifndef CAVER_GUIROUNDEDRECT_H
#define CAVER_GUIROUNDEDRECT_H

#include <boost/intrusive_ptr.hpp>
#include <string>

#include "../core/Color.h"
#include "../core/Rectangle.h"
#include "../render/Sprite.h"
#include "../render/Texture.h"

namespace Caver {

class RenderingContext;

class GUIRoundedRect {
public:
    GUIRoundedRect();
    ~GUIRoundedRect();

    void InitWithTextureName(const std::string& name);
    void InitWithTexture(const boost::intrusive_ptr<Texture>& texture);
    void CreateWithOuterBounds(const Rectangle& bounds);
    void CreateWithInnerBounds(const Rectangle& bounds, float inset);
    void CreateSprite();
    void SetColor(Color color);
    void Draw(RenderingContext* context) const;

    boost::intrusive_ptr<Texture> texture;  // +0
    Color color;                            // +8
    Rectangle outerRect;                    // +12 UNRECOVERED name
    Rectangle innerRect;                    // +28 UNRECOVERED name
    Rectangle uvRect;                       // +44 UNRECOVERED name
    float insets[4];                        // +60 left/right/top/bottom
    char _pad76[4];                         // +76
    Sprite sprite;                          // +80
};

static_assert(sizeof(GUIRoundedRect) == 168, "GUIRoundedRect must be 168 bytes");

} // namespace Caver

#endif
