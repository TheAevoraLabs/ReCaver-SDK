// TrueType / OpenType font rasterizer and atlas generator for Caver Engine.
// Credit: Quantum Creeper / Aevora Labs.
#ifndef CAVER_TTFFONTLOADER_H
#define CAVER_TTFFONTLOADER_H

#include <boost/shared_ptr.hpp>
#include <string>
#include "Font.h"

namespace Caver {

enum TTFFontStyle {
    TTFFontStyleStandard     = 0,      // Pure crisp white glyphs with alpha mask
    TTFFontStyleOutlined     = 1 << 0, // Solid dark outer contour/halo
    TTFFontStyleGold         = 1 << 1, // Authentic Swordigo amber/gold vertical gradient
    TTFFontStyleGoldOutlined = TTFFontStyleOutlined | TTFFontStyleGold, // Gold with dark bronze contour
    TTFFontStyleShadowed     = 1 << 2  // Baked drop shadow
};

class TTFFontLoader {
public:
    // Load a font from a TTF/OTF file at the specified pixel point size.
    static boost::shared_ptr<Font> LoadFont(const std::string& fontPath,
                                            float pixelHeight,
                                            int styleFlags = TTFFontStyleStandard);

    // Convenience loader for MegalopolisExtra (Swordigo's signature font).
    static boost::shared_ptr<Font> LoadMegalopolis(float pixelHeight,
                                                   int styleFlags = TTFFontStyleStandard);
};

} // namespace Caver

#endif
