// Recovered from libswordigo.so (1.4.13) evidence.
//
// GUITexturedRect is a plain (non-polymorphic — no vtable/typeinfo symbol
// exists in the golden) textured quad: an intrusive_ptr<Texture>, a bounds
// rectangle, a tint colour and four interleaved vertices
// (position xyz + uv, stride 20 bytes) built by CreateVertexData().
//
// Layout (112 bytes, from IDA 0x53228C ctor / 0x5322A8 Reset /
// 0x5322FC+0x53257C InitWithTexture / 0x532410 CreateVertexData /
// 0x532734 Draw):
//   +0   boost::intrusive_ptr<Texture> texture
//   +8   bool tiled (uv computed as position / texture size when set)
//   +12  Rectangle bounds (origin +12, size +20)
//   +28  Color color (ctor writes dword -1 = white)
//   +32  vertex data: 4 x { Vector3 position, Vector2 uv }
#ifndef CAVER_GUITEXTUREDRECT_H
#define CAVER_GUITEXTUREDRECT_H

#include <boost/intrusive_ptr.hpp>

#include "../core/Color.h"
#include "../core/Rectangle.h"
#include "../core/Vector2.h"
#include "../render/Texture.h"

namespace Caver {

class RenderingContext;

class GUITexturedRect {
public:
    GUITexturedRect();

    void Reset();
    void InitWithTexture(const boost::intrusive_ptr<Texture>& texture,
                         const Vector2& anchor);
    void InitWithTexture(const boost::intrusive_ptr<Texture>& texture,
                         const Rectangle& bounds, bool tiled);
    void InitWithTextureName(const std::string& name, const Vector2& anchor);
    void CreateWithBounds(const Rectangle& bounds, bool tiled);
    void CreateVertexData();
    void Draw(RenderingContext* context) const;

    boost::intrusive_ptr<Texture> texture;  // +0
    bool tiled;                             // +8
    Rectangle bounds;                       // +12
    Color color;                            // +28
    // +32: 4 vertices x (Vector3 position + Vector2 uv), stride 20 bytes.
    float vertexData[20];
};

} // namespace Caver

#endif
