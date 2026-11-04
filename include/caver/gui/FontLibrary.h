// Recovered from libswordigo.so (1.4.13) evidence — IDA exports in
// ~/workspace/analysis/swordigo/functions/Caver/FontLibrary/ checked
// against the raw arm64 disassembly; the binary is the spec.
//
// FontLibrary: a name -> weak_ptr<Font> cache plus a singleton.
// Layout (sizeof 32, proven by sharedLibrary @0x548BEC's
// operator new(0x20) and its init store map, Clear @0x548C78 and the
// map accesses in FontWithName @0x548CAC):
//   +0  std::map<std::string, boost::weak_ptr<Font>> m_fonts
//   +24 bool m_field24 — gates FontWithName's "font_<name>_2x" first
//        attempt (name UNRECOVERED; TextureLibrary has the same flag
//        at its +33). The EngineSystems.h stub models this same byte.
// This header supersedes the FontLibrary stub in core/EngineSystems.h
// at merge time (the stub's +24 bool agrees with this layout).
#ifndef CAVER_FONTLIBRARY_H
#define CAVER_FONTLIBRARY_H

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <map>
#include <string>

#include "Font.h"

namespace Caver {

class FontLibrary {
public:
    FontLibrary() : m_field24(false) {}
    ~FontLibrary() {}

    static FontLibrary* sharedLibrary();
    static void SetSharedLibrary(FontLibrary* library);
    void Clear();

    boost::shared_ptr<Font> FontWithName(const std::string& name);
    boost::shared_ptr<Font> SystemFont();
    boost::shared_ptr<Font> SmallSystemFont();
    boost::shared_ptr<Font> DefaultFont();
    boost::shared_ptr<Font> SmallDefaultFont();
    boost::shared_ptr<Font> MediumSmallDefaultFont();
    boost::shared_ptr<Font> LargeDefaultFont();
    boost::shared_ptr<Font> FontFromTTF(const std::string& fontPath, float pixelHeight, int styleFlags = 0);
    boost::shared_ptr<Font> DynamicFont(float pixelHeight, int styleFlags = 0);
    boost::shared_ptr<Font> DynamicGoldFont(float pixelHeight);
    boost::shared_ptr<Font> DynamicOutlinedFont(float pixelHeight);

    std::map<std::string, boost::weak_ptr<Font> > m_fonts; // +0
    bool m_field24; // +24 UNRECOVERED name
};

} // namespace Caver

#endif
