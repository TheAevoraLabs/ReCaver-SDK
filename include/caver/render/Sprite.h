// Recovered from libswordigo.so (1.4.13) evidence — FULL layout.
// An 88-byte immediate-mode sprite batch: a texture, a bounds rectangle,
// and two raw CPU buffers (an unsigned-short index buffer of triangle
// faces and an interleaved vertex buffer) with derived interior
// pointers for the colour and texcoord streams.
//
// Layout (byte offsets proven against the golden binary):
//   +0  boost::intrusive_ptr<Texture>
//       (~Sprite @0x37CF6C releases it after the buffer deletes;
//       Init @0x54B618 and InitWithTexture @0x54C694 assign it first)
//   +8  Rectangle bounds — origin +8/+12, size (width/height) +16/+20.
//       The "float pair at +16" the BackgroundComponent::Draw row on the
//       modelmesh board reads as the sprite size is bounds.width/height.
//       AddRectangle/AddTriangleStrip/AddTransformedRectangle seed it
//       from the first geometry (when width < 0.001) and grow it with
//       Rectangle::ExpandToIncludePoint / ExpandToincludeRectangle.
//   +24 int index capacity, in FACES (3 indices = 6 bytes per face);
//       ResizeIndexBuffer @0x54B814 stores it, AddFaces @0x54CB44 grows
//       to it. (FontText's storage notes call +24 the second count.)
//   +28 int index count, in faces; Clear @0x54C688 zeroes it.
//   +32 unsigned short* index buffer (operator delete[] in ~Sprite,
//       after the +56 buffer); DrawTriangles @0x54CA54 hands it to
//       RenderingContext::DrawElements (0 when absent -> DrawArrays).
//   +40 int vertex capacity (ResizeVertexBuffer @0x54B774 stores it).
//   +44 int vertex count; Clear zeroes it.
//   +48 int vertex size (stride): 24 when the colour stream is present,
//       20 otherwise (Init @0x54B618 selects on its bool argument;
//       CreateVertexData @0x54C7B0 always writes 20).
//       Vertex layout inside the buffer: position (x, y, z=0.0f)
//       at +0, colour (4 bytes) at +12 when present, texcoords at
//       +12 (no colour) or +16 (colour) — proven by AddVertex
//       @0x54C098 and CreateVertexData.
//   +56 char* vertex buffer (operator delete[] FIRST in ~Sprite).
//   +64 void* colour data: vertex buffer + 12, refreshed by every
//       vertex-buffer (re)allocation, and only when the colour stream
//       is present (Init/ResizeVertexBuffer/AddVertex/
//       AddTrianglesFromSprite). DrawTriangles passes it as the
//       32886 (colour) attribute pointer. NOTE: the earlier stub read
//       this slot as a std::string name — that was a misread of
//       BackgroundComponent::ValueForBindedProperty, which dereferences
//       sprite->texture and reads the TEXTURE's +64 string. ~Sprite
//       proves there is no string member: it destroys no string.
//   +72 void* texcoord data: vertex buffer + 12 (no colour) or + 16
//       (colour); DrawTriangles passes it as the 32888 attribute ptr.
//   +80 bool colour-stream flag (Init's 4th argument); member NAME
//       unrecovered.
//   +81..87 tail padding.
//
// There is NO out-of-line constructor in the census: owners
// value-initialise the storage (BackgroundComponent's ctor resets a
// shared_ptr to a fresh zeroed 88-byte Sprite; GUIRoundedRect /
// FontText embed zero-initialised storage), so the class keeps an
// implicit default constructor. ~Sprite is weak in the golden
// (header-inline originally) and is defined inline below, so owners
// such as ~HealthComponent inline its teardown exactly as the
// golden does.
#ifndef CAVER_SPRITE_H
#define CAVER_SPRITE_H

#include "../core/Color.h"
#include "../core/Rectangle.h"
#include "../core/Vector2.h"
#include <boost/intrusive_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <cstddef>

namespace Caver {

class GradientColor;
class Matrix4;
class RenderingContext;
class Texture;

class Sprite {
public:
    Sprite()
        : m_texture(nullptr),
          m_bounds{0.0f, 0.0f, 0.0f, 0.0f},
          m_indexCapacity(0),
          m_indexCount(0),
          m_indexBuffer(nullptr),
          m_vertexCapacity(0),
          m_vertexCount(0),
          m_vertexSize(0),
          m_vertexBuffer(nullptr),
          m_colorData(nullptr),
          m_texcoordData(nullptr),
          m_field80(false) {
        std::memset(m_pad81, 0, sizeof(m_pad81));
    }

    // Header-inline in the golden: owners inline this teardown
    // (~HealthComponent @0x36CEC4 deletes the +56 buffer, then the
    // +32 buffer, then releases the +0 texture, all inside its own
    // body), and ~Sprite @0x37CF6C itself deletes +56 first, then
    // +32. The +0 intrusive texture is released by its own
    // destructor after this body, exactly as in the golden.
    ~Sprite()
    {
        delete[] m_vertexBuffer;
        delete[] m_indexBuffer;
    }

    void Init(const boost::intrusive_ptr<Texture>& texture, int maxFaces,
              int maxVertices, bool hasColor);
    void InitWithMaxRectangles(const boost::intrusive_ptr<Texture>& texture,
                               int maxRectangles, bool hasColor);
    void InitWithTexture(const boost::intrusive_ptr<Texture>& texture,
                         const Rectangle& rect);
    void CreateVertexData(int vertexCount, const Vector2* positions,
                          int faceCount, unsigned short* indices);

    void ResizeVertexBuffer(int count);
    void ResizeIndexBuffer(int count);
    void Clear();

    void AddRectangle(const Rectangle& rect, const Rectangle& uvRect,
                      const Color& color);
    void AddQuad(const Vector2* positions, const Vector2* uvPositions,
                 const Color* colors);
    void AddTransformedRectangle(const Rectangle& rect,
                                 const Rectangle& uvRect,
                                 const Matrix4& matrix, const Color& color);
    int AddGradientRectangle(const Rectangle& rect, const Rectangle& uvRect,
                             const boost::shared_ptr<GradientColor>& gradient);
    void AddLine(const Vector2& from, const Vector2& to, float width,
                 const Rectangle& uvRect, const Color& color);
    void AddVertex(const Vector2& position, const Vector2& uvPosition,
                   const Color& color);
    void AddFace(int v0, int v1, int v2);
    void AddFaces(int faceCount, unsigned short* indices);
    void AddTriangleStrip(int vertexCount, const Vector2* positions,
                          const Vector2* uvPositions, const Color& color);
    void AddTrianglesFromSprite(Sprite* other);

    void Translate(const Vector2& offset);
    void TranslateFaces(int firstFace, int faceCount, const Vector2& offset);
    void Scale(float scale, const Vector2& origin);

    void Draw(RenderingContext* context);
    void Draw(RenderingContext* context) const;
    void DrawTriangles(RenderingContext* context, int firstFace,
                       int faceCount) const;

    Vector2 TexCoordForVertexLocation(const Vector2& location);

    const boost::intrusive_ptr<Texture>& GetTexture() const {
        return m_texture;
    }
    const Rectangle& GetBounds() const { return m_bounds; }

private:
    boost::intrusive_ptr<Texture> m_texture;  // +0
    Rectangle m_bounds;                       // +8
    int m_indexCapacity;                      // +24 (faces)
    int m_indexCount;                         // +28 (faces)
    unsigned short* m_indexBuffer;            // +32
    int m_vertexCapacity;                     // +40
    int m_vertexCount;                        // +44
    int m_vertexSize;                         // +48 (20 or 24)
    char* m_vertexBuffer;                     // +56
    void* m_colorData;                        // +64
    void* m_texcoordData;                     // +72
    bool m_field80;                           // +80 (colour stream flag;
                                              // name UNRECOVERED)
    unsigned char m_pad81[7];                 // +81

    // Member offsets verified against the golden layout by the
    // debts-sprite parity harness (texture +0, bounds +8, index buffer
    // +32, vertex buffer +56, colour flag +80).
};

static_assert(sizeof(Sprite) == 88, "Sprite must be 88 bytes");

} // namespace Caver

#endif
