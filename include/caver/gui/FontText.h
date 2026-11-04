// Recovered from libswordigo.so (1.4.13) evidence — IDA exports in
// ~/workspace/analysis/swordigo/functions/Caver/FontText/, the golden
// GUILabel ctor @0x5202CC (which inlines this class's ctor for its
// `new FontText`), GUITextBubble::SetText @0x531A6C (same), and every
// FontText method body.
//
// FontText: the GUI text-layout engine. It owns an embedded Sprite
// holding one quad per rendered glyph, a per-character record for
// every laid-out character, and a per-line index range table used by
// the alignment / bounds / translate helpers. GUILabel and
// GUITextBubble each own one via boost::shared_ptr<FontText>.
//
// NON-POLYMORPHIC: the golden exports no _ZTV/_ZTI/_ZTS for FontText
// (verified by symbol-table scan). The only out-of-line own symbols
// are the 11 graded in FontText.cpp (Clear, SetColor, AddText,
// Translate, AlignHorizontally, BoundsForLine, TranslateLine,
// BoundsForCharacterAtIndex, Draw, DrawSubstring, ~FontText); the
// constructor has NO out-of-line symbol — it is header-inline in the
// original and inlines into the GUILabel / GUITextBubble allocation
// sites (see the ctor note below).
//
// Layout (sizeof 240 = 0xF0, proven by the operator new(0xF0) at both
// allocation sites and by every method body's accesses):
//   +0   boost::shared_ptr<Font> font
//   +16  Color color (ctor: rgb 0, alpha byte at +19 = 255)
//   +24  boost::shared_ptr<GradientColor> gradient
//   +40  bool shadowEnabled
//   +41  Color shadowColor (UNALIGNED; ctor alpha byte at +44 = 255)
//   +48  Vector2 shadowOffset
//   +56  int field56 — a line-count limit consulted by AddText's wrap
//        path (it wraps/truncates once lineCount reaches it, when
//        >= 1); GUILabel::UpdateText copies its field348 here.
//        Member name UNRECOVERED.
//   +60  int lineCount — number of entries in the lines table as built
//        by the current AddText sequence (AddText resets it to 1, then
//        increments per wrapped line; AlignHorizontally only runs
//        when it is >= 2). Member name UNRECOVERED.
//   +64  Rectangle textBounds — the laid-out text rectangle: origin at
//        +64/+68 (called the text origin by GUITextBubble) and size at
//        +72/+76 (called the text size by GUILabel). Name UNRECOVERED.
//   +80  int faceCount — faces added to the sprite so far; each
//        CharacterInfo records the value at its creation as its first
//        face index. Member name UNRECOVERED.
//   +88  Sprite sprite (88 bytes, embedded by value)
//   +176 std::vector<CharacterInfo> characters
//   +200 std::vector<LineRange> lines
//   +224 Vector2 penPosition — the pen position the next glyph is laid
//        out at; AddText continues from the previous text's last
//        character when the characters vector is non-empty. Name
//        UNRECOVERED.
//   +232 int previousGlyphIndex — glyph index of the last processed
//        character (kerning input for the next one); -1 = none
//        (ctor and Clear). Name UNRECOVERED.
#ifndef CAVER_FONTTEXT_H
#define CAVER_FONTTEXT_H

#include <boost/intrusive_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <cstring>
#include <string>
#include <vector>

#include "../core/Color.h"
#include "../core/Rectangle.h"
#include "../core/Vector2.h"
#include "../render/Texture.h"
#include "GUIEventTypes.h"

namespace Caver {

class Font;
class GradientColor;
class RenderingContext;
class Texture;

// Storage for the embedded Sprite (the real Caver::Sprite layout is
// the render slice's; only the fields FontText's own bodies and the
// Sprite entry points touch are modelled, at their proven offsets —
// the same idiom as GUIRoundedRect's RoundedRectSpriteStorage).
// Sprite facts proven by Sprite::Init @0x54B618, Sprite::Clear
// @0x54C688 (zeroes +28 and +44), Sprite::ResizeVertexBuffer
// @0x54B774 (capacity at +40) and ~Sprite @0x37CF6C (frees the +56
// buffer, then the +32 buffer, then releases the texture):
struct FontTextSpriteStorage {
    boost::intrusive_ptr<Texture> texture;  // +0
    char _pad8[24];                         // +8 (a Rectangle at +8,
                                            // counts at +24/+28)
    void* indexBuffer;                      // +32
    int vertexCapacity;                     // +40
    int vertexCount;                        // +44
    char _pad48[8];                         // +48 (vertex stride at +48)
    void* vertexBuffer;                     // +56
    char _pad64[24];                        // +64 (derived buffer
                                            // pointers at +64/+72,
                                            // a bool at +80)

    FontTextSpriteStorage()
        : texture(nullptr), indexBuffer(nullptr), vertexCapacity(0),
          vertexCount(0), vertexBuffer(nullptr) {
        std::memset(_pad8, 0, sizeof(_pad8));
        std::memset(_pad48, 0, sizeof(_pad48));
        std::memset(_pad64, 0, sizeof(_pad64));
    }
    ~FontTextSpriteStorage() {
        delete[] (char*)vertexBuffer;
        delete[] (char*)indexBuffer;
    }
};

class FontText {
public:
    // Proven nested type name (the golden vector symbols
    // std::vector<Caver::FontText::CharacterInfo>). Layout proven by
    // AddText @0x549E18 (stride 36), Translate @0x54AB84,
    // TranslateLine @0x54AEAC and DrawSubstring @0x54B020:
    //   +0  unsigned char character (the source byte)
    //   +4  Rectangle bounds (glyph rect translated to the pen, with
    //       y/height replaced by the font bounding box's y/height)
    //   +20 Vector2 position (the pen position at layout time)
    //   +28 unsigned short faceIndex (value of FontText's face
    //       counter when this character's faces were added)
    //   +32 int glyphIndex (-1 when the character has no glyph)
    // Field names UNRECOVERED.
    struct CharacterInfo {
        unsigned char character;   // +0
        char _pad1[3];             // +1
        Rectangle bounds;          // +4
        Vector2 position;          // +20
        unsigned short faceIndex;  // +28
        char _pad30[2];            // +30
        int glyphIndex;            // +32
    };

    // Element of the lines table: an 8-byte {first character index,
    // character count} pair — proven by AlignHorizontally @0x54ABF4
    // and BoundsForLine @0x54ADC8 reading the int pair at 8*line, and
    // by AddText pushing new entries as one qword (index, then a
    // count it fills in as characters are laid out). The original
    // type/field names are UNRECOVERED (no vector symbol for it was
    // ever emitted out-of-line).
    struct LineRange {
        int firstCharacter;   // +0
        int characterCount;   // +4
    };

    // Header-inline in the golden: the GUILabel ctor @0x5202CC and
    // GUITextBubble::SetText @0x531A6C show the inlined initialisation
    // byte-for-byte — every member zero except the two colour alphas
    // (bytes +19 and +44 = 0xFF) and the dword at +232 = -1; the
    // padding at +45..47/+84..87/+140..143/+169..175/+236..239 is
    // never written by the golden.
    FontText()
        : shadowEnabled(false), field56(0), lineCount(0), faceCount(0),
          previousGlyphIndex(-1) {
        color.r = 0;
        color.g = 0;
        color.b = 0;
        color.a = 255;
        shadowColor.r = 0;
        shadowColor.g = 0;
        shadowColor.b = 0;
        shadowColor.a = 255;
        shadowOffset.x = 0.0f;
        shadowOffset.y = 0.0f;
        textBounds.x = 0.0f;
        textBounds.y = 0.0f;
        textBounds.width = 0.0f;
        textBounds.height = 0.0f;
        penPosition.x = 0.0f;
        penPosition.y = 0.0f;
    }

    // WEAK in the golden (@0x33AB80, 300 bytes): the teardown is
    // exactly the reverse-declaration member teardown — lines vector,
    // characters vector, the sprite's buffers and texture, the
    // gradient shared_ptr, then the font shared_ptr — which is what
    // GUILabel's / GUITextBubble's stub FontText classes connected to
    // through this same mangled name. Defined in FontText.cpp.
    ~FontText();

    void Clear();
    void SetColor(const Color& color);
    void AddText(const std::string& text, float maxWidth,
                 const Vector2& position);
    void Translate(const Vector2& delta);
    void AlignHorizontally(GUITextAlignment alignment);
    Rectangle BoundsForLine(int line);
    void TranslateLine(int line, const Vector2& delta);
    Rectangle BoundsForCharacterAtIndex(int index);
    void Draw(RenderingContext* context) const;
    void DrawSubstring(RenderingContext* context, unsigned long start,
                       unsigned long count) const;

    boost::shared_ptr<Font> font;                // +0
    Color color;                                 // +16
    char _pad20[4];                              // +20
    boost::shared_ptr<GradientColor> gradient;   // +24
    bool shadowEnabled;                          // +40
    Color shadowColor;                           // +41
    char _pad45[3];                              // +45
    Vector2 shadowOffset;                        // +48
    int field56;                                 // +56 UNRECOVERED name
    int lineCount;                               // +60 UNRECOVERED name
    Rectangle textBounds;                        // +64 UNRECOVERED name
    int faceCount;                               // +80 UNRECOVERED name
    char _pad84[4];                              // +84
    FontTextSpriteStorage sprite;                // +88
    std::vector<CharacterInfo> characters;       // +176
    std::vector<LineRange> lines;                // +200
    Vector2 penPosition;                         // +224 UNRECOVERED name
    int previousGlyphIndex;                      // +232 UNRECOVERED name
    char _pad236[4];                             // +236
};

} // namespace Caver

#endif
