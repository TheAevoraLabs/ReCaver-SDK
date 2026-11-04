// Recovered from libswordigo.so (1.4.13) evidence.
// 24-byte value object owned by Texture at +176: a bool at +0, width
// +4, height +8, image type +12, pixel format +16 and a float aspect
// (width / height) at +20, recomputed by LoadFromProtobufMessage.
// Texture::LoadFromProtobufMessage allocates it with defaults
// {true, 0, 0, ImageType 3, PixelFormat 0, NaN aspect}.
#ifndef CAVER_TEXTURECONVERSIONINFO_H
#define CAVER_TEXTURECONVERSIONINFO_H

#include "Texture.h"
#include "../proto/Common.pb.h"

namespace Caver {

namespace Proto {
class Texture_ConversionInfo;
}

// Enum converters between the engine and protobuf domains. All four
// are the same clamp in the golden: values 1..N pass through, anything
// else becomes 0 (N = 3 for ImageType, 8 for PixelFormat).
Proto::ImageType ProtobufValueFromImageType(ImageType value);
ImageType ImageTypeFromProtobufValue(Proto::ImageType value);
Proto::PixelFormat ProtobufValueFromPixelFormat(PixelFormat value);
PixelFormat PixelFormatFromProtobufValue(Proto::PixelFormat value);

class TextureConversionInfo {
public:
    float LoadFromProtobufMessage(const Proto::Texture_ConversionInfo& message);
    void SaveToProtobufMessage(Proto::Texture_ConversionInfo* message) const;
    // Golden 0x550A00 returns 0/1 (1 = image resized or converted, 0 =
    // already matching, except PVR sources which always report 1); the
    // return type is not encoded in the Itanium mangled name, so bool
    // keeps the golden symbol intact.
    bool ConvertTexture(Texture* texture, class Image* image);

    bool m_field0;          // +0
    int m_width;            // +4
    int m_height;           // +8
    ImageType m_imageType;  // +12
    PixelFormat m_pixelFormat; // +16
    float m_aspect;         // +20
};

} // namespace Caver

#endif
