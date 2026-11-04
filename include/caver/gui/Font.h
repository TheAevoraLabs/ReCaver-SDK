// Recovered from libswordigo.so (1.4.13) evidence — layout proven by
// FontText's AddText @0x549E18, Font::AddGlyph @0x548374, Font::Scale
// @0x5486B8 and Font::KerningBetweenGlyphsAtIndices @0x5485D0 (the
// same evidence the FontText slice recorded in FontText.cpp, where a
// TU-local copy of this layout still lives — retire that copy at
// merge). sizeof 112, proven by the operator new(0x70) in
// FontLibrary::FontWithName @0x548CAC, whose body zeroes the whole
// object: the original Font default ctor value-initialises every
// member below.
//   +0   char[24] UNRECOVERED
//   +24  boost::intrusive_ptr<Texture> texture
//   +32  std::vector<Glyph> glyphs (Glyph stride 44)
//   +56  std::vector<int> glyphIndexForChar (name UNRECOVERED)
//   +80  char[8] UNRECOVERED (kerning table pointer lives here)
//   +88  float height
//   +92  Rectangle boundingBox
#ifndef CAVER_FONT_H
#define CAVER_FONT_H

#include <boost/intrusive_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <string>
#include <vector>

#include "../core/Rectangle.h"
#include "../render/Texture.h"

namespace Caver {

namespace Proto {
class Font;
}

class Font {
public:
    struct Glyph {
        int index;        // +0
        int character;    // +4
        Rectangle rect;   // +8
        int advance;      // +24
        Rectangle uvRect; // +28
    };

    Font() : height(0.0f) {
        for (int i = 0; i < 24; ++i) _pad0[i] = 0;
        for (int i = 0; i < 8; ++i) _pad80[i] = 0;
    }

    bool LoadFromFile(const std::string& path);
    void LoadFromProtobufMessage(const Proto::Font& message);
    bool SaveToFile(const std::string& path) const;
    void SaveToProtobufMessage(Proto::Font* message) const;
    void AddGlyph(const Glyph& glyph);
    int KerningBetweenGlyphsAtIndices(int a, int b);
    void SetKerningBetweenGlyphsAtIndices(int a, int b, int value);
    void Scale(float scale);

    // The Font:: static accessors forward to FontLibrary (proven by
    // Font::FontWithName @0x547BE0 et al.); declared here, defined by
    // the gui lane's Font TU.
    static boost::shared_ptr<Font> FontWithName(const std::string& name);
    static boost::shared_ptr<Font> SystemFont();
    static boost::shared_ptr<Font> SmallSystemFont();
    static boost::shared_ptr<Font> DefaultFont();
    static boost::shared_ptr<Font> SmallDefaultFont();
    static boost::shared_ptr<Font> DynamicFont(float pixelHeight, int styleFlags = 0);
    static boost::shared_ptr<Font> DynamicGoldFont(float pixelHeight);
    static boost::shared_ptr<Font> DynamicOutlinedFont(float pixelHeight);

    char _pad0[24];                            // +0 UNRECOVERED
    boost::intrusive_ptr<Texture> texture;     // +24
    std::vector<Glyph> glyphs;                 // +32
    std::vector<int> glyphIndexForChar;        // +56 UNRECOVERED name
    char _pad80[8];                            // +80 UNRECOVERED
    float height;                              // +88
    Rectangle boundingBox;                     // +92
};

} // namespace Caver

#endif
