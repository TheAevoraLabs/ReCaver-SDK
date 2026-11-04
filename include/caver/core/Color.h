// Recovered from libswordigo.so (1.4.13) evidence — see docs/CORE_HEADERS.md.
// Layout: four RGBA bytes (verified against proven call sites).
#ifndef CAVER_COLOR_H
#define CAVER_COLOR_H

namespace Caver {

struct FloatColor;

struct Color {
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;

    Color() {}
    Color(unsigned char r_, unsigned char g_, unsigned char b_,
          unsigned char a_) {
        *(unsigned int*)this = (unsigned int)r_ | ((unsigned int)g_ << 8) |
                               ((unsigned int)b_ << 16) |
                               ((unsigned int)a_ << 24);
    }
    Color(const Color& other) : r(other.r), g(other.g), b(other.b), a(other.a) {}

    // Proven from the golden symbol
    // _ZN5Caver5Color14FromFloatColorERKNS_10FloatColorE.
    static Color FromFloatColor(const FloatColor& color);
    Color ColorByReversingAlphaPremultiplication();
};

inline bool operator==(const Color& lhs, const Color& rhs) {
    return lhs.r == rhs.r && lhs.g == rhs.g && lhs.b == rhs.b &&
           lhs.a == rhs.a;
}
inline bool operator!=(const Color& lhs, const Color& rhs) {
    return !(lhs == rhs);
}

} // namespace Caver

#endif
