// Recovered from libswordigo.so (1.4.13) evidence — IDA/Hex-Rays exports in
// reference/ida-arm64/functions/Caver/Image/.
//
// Layout (sizeof 0x20; the offsets below are what every golden body in that
// directory reads and writes):
//   +0   bool  m_premultipliedAlpha  set once RGB has the alpha channel
//                                    multiplied into it (PremultiplyAlpha
//                                    tests and then sets it; the .tex loader
//                                    sets it unconditionally because the
//                                    shipped .tex payload is already
//                                    premultiplied, and LoadFromFile sets it
//                                    for flipped PNGs)
//   +4   int   m_width
//   +8   int   m_height
//   +12  int   m_pixelFormat         Caver::PixelFormat, the 1..8 codes
//   +16  int   m_bytesPerPixel       dword_26BD94[format - 1]
//   +20  int   m_dataSize            width * height * bytesPerPixel
//   +24  void* m_data                new[]-allocated, deleted with delete[]
//
// The four middle fields are deliberately NOT initialised by Image(): the
// golden default constructor @0x54F740 writes only +0, +20 and +24. Loading
// or creating always fills them before they are read.
#ifndef CAVER_IMAGE_H
#define CAVER_IMAGE_H

#include <string>

#include "../core/FloatColor.h"
#include "Texture.h"

namespace Caver {

class Image {
public:
    Image();
    Image(const Image& other);
    Image(int width, int height, PixelFormat pixelFormat);
    ~Image();

    // _ZN5Caver5Image6CreateEiiNS_11PixelFormatE @0x54F874
    void Create(int width, int height, PixelFormat pixelFormat);

    // _ZN5Caver5Image13InitWithImageERKS0_ @0x54F7A8 — copies every field and
    // deep-copies the payload, releasing the old payload first.
    void InitWithImage(const Image& other);

    // _ZN5Caver5Image5ClearEv @0x54F91C
    void Clear();

    // _ZN5Caver5Image4BlitEiiRKS0_ @0x54F930
    void Blit(int x, int y, const Image& source);
    // _ZN5Caver5Image4BlitEiiRKS0_iiii @0x54F9C8
    void Blit(int x, int y, const Image& source, int sourceX, int sourceY,
              int width, int height);

    // _ZN5Caver5Image6ResizeEii @0x54FF64
    void Resize(int width, int height);

    // _ZN5Caver5Image20ConvertToPixelFormatENS_11PixelFormatEb @0x5500C0
    void ConvertToPixelFormat(PixelFormat pixelFormat, bool dither);

    // _ZN5Caver5Image12LoadFromFileERKNSt6__ndk112basic_stringIcNS1_
    // 11char_traitsIcEENS1_9allocatorIcEEEENS_9ImageTypeEb @0x5504AC
    bool LoadFromFile(const std::string& path, ImageType imageType,
                      bool flipped);
    // _ZN5Caver5Image15LoadFromTEXFileERKNSt6__ndk112basic_stringIcNS1_
    // 11char_traitsIcEENS1_9allocatorIcEEEEb @0x550510
    bool LoadFromTEXFile(const std::string& path, bool flipped);
    // _ZNK5Caver5Image10SaveToFileERKNSt6__ndk112basic_stringIcNS1_
    // 11char_traitsIcEENS1_9allocatorIcEEEENS_9ImageTypeE @0x550680
    bool SaveToFile(const std::string& path, ImageType imageType) const;
    // _ZNK5Caver5Image13SaveToTEXFileERKNSt6__ndk112basic_stringIcNS1_
    // 11char_traitsIcEENS1_9allocatorIcEEEEb @0x5506A4
    bool SaveToTEXFile(const std::string& path, bool flipped) const;

    // @0x550928 and @0x550930: both are `return 0` in the golden — PNG support
    // is compiled out of the shipped engine.
    bool LoadFromPNGFile(const std::string& path, bool flipped);
    bool SaveToPNGFile(const std::string& path) const;

    // _ZN5Caver5Image12CopyDataFromEPKv @0x550814
    void CopyDataFrom(const void* source);

    // _ZN5Caver5Image16PremultiplyAlphaEv @0x550824
    void PremultiplyAlpha();

    // _ZNK5Caver5Image8GetPixelEii @0x54FA80 and
    // _ZN5Caver5Image8SetPixelEiiRKNS_10FloatColorE @0x54FC78.
    // GetPixel returns an untouched FloatColor for format 3 (and for formats
    // outside 1..8), exactly like the golden's switch fallthrough.
    FloatColor GetPixel(int x, int y) const;
    void SetPixel(int x, int y, const FloatColor& color);

    bool m_premultipliedAlpha; // +0
    int m_width;               // +4
    int m_height;              // +8
    int m_pixelFormat;         // +12
    int m_bytesPerPixel;       // +16
    int m_dataSize;            // +20
    void* m_data;              // +24
};

static_assert(sizeof(Image) == 0x20, "sizeof must be 0x20");

} // namespace Caver

#endif
