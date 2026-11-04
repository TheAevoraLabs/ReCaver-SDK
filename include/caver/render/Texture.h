// Recovered from libswordigo.so (1.4.13) evidence — see docs/CORE_HEADERS.md.
// A texture resource: either a root texture owning a GL texture, or a
// subtexture referencing a region of a parent (atlas) texture.
//
// Layout (byte offsets verified against the golden binary):
//   +0   primary vtable (refcounted primary base, name unconfirmed)
//   +8   int refcount (intrusive_ptr)
//   +16  secondary vtable (shared component binding interface, name
//        unconfirmed — the _ZThn16_ thunks adjust this by -16)
//   +24  delegate pointer (vtable slot 0 called after load-from-parent,
//        slot +8 after unload)
//   +32  std::string resource path
//   +56  int state
//   +64  std::string (second string)
//   +88  unsigned m_glTexture (GL name)
//   +92  int padded width
//   +96  int padded height
//   +100 PixelFormat
//   +108 Rectangle subtexture rect
//   +124 float[4] UV transform (offset +124, scale +132)
//   +140 float scale (init 1.0)
//   +144 Texture* parent (raw, not retained)
//   +152 std::vector<boost::intrusive_ptr<Texture>> subtextures
//   +176 TextureConversionInfo* (owned; operator delete in ~Texture)
//   +184 field copied from the parent by LoadFromParentTexture
//   +192 byte field
#ifndef CAVER_TEXTURE_H
#define CAVER_TEXTURE_H

#include "../core/Rectangle.h"
#include "../core/Vector2.h"
#include "../component/ReferenceCountedObject.h"
#include "../component/IBindable.h"
#include <boost/intrusive_ptr.hpp>
#include <cstdint>
#include <string>
#include <vector>

namespace Caver {

namespace Proto {
class Texture;
}

class Texture;
struct Binding;
class BindingValue;

// Golden values: the format codes are the proto PixelFormat order, 1..8. 0 is
// "unset" — Texture::Texture @0x54CC94 writes a zeroed _OWORD at +100, and
// ProtobufValueFromPixelFormat maps anything outside 1..8 to 0 as well.
// Bytes-per-pixel is dword_26BD94[format - 1] = {4,2,2,3,2,1,1,2}.
enum PixelFormat {
    PixelFormatNone = 0,
    PixelFormatRGBA8888 = 1,
    PixelFormatRGBA4444 = 2,
    PixelFormatRGBA5551 = 3,
    PixelFormatRGB888 = 4,
    PixelFormatRGB565 = 5,
    PixelFormatLuminance8 = 6,
    PixelFormatAlpha8 = 7,
    PixelFormatLuminanceAlpha88 = 8
};

enum ImageType {
    // Values proven by Texture::InitWithResource (session 3): the
    // state it stores is 1 for .png, 2 for .pvr, 3 for .tex; 0 is the
    // unset/default value (the enum converters clamp to 1..3).
    ImageTypePNG = 1,
    ImageTypePVR = 2,
    ImageTypeTEX = 3,
    ImageTypeVideo = 4
};

// Texture's bases are the shared ReferenceCountedObject (primary: vptr +0,
// refcount +8) and IBindable (secondary at +16, exactly the six binding
// virtuals and no destructor — which is why the golden exports no _ZThn16_
// destructor thunks for Texture). Recovered by the components slice in
// component/ReferenceCountedObject.h and component/IBindable.h.

// Delegate notified through the raw pointer at +24: vtable slot 0 after a
// load-from-parent, slot +8 after an unload (names unconfirmed).
class TextureConversionInfo;

class TextureDelegate {
public:
    virtual void TextureLoaded(Texture* texture) = 0;
    virtual void TextureUnloaded(Texture* texture) = 0;
    virtual ~TextureDelegate() {}
};

class Texture : public ReferenceCountedObject, public IBindable {
public:
    Texture();
    virtual ~Texture();

    boost::intrusive_ptr<Texture> SharedPtr();

    static void SetLoadPNGFilesOnly(bool value);
    static int Pow2FromDimension(int dimension);

    static bool s_loadPNGFilesOnly;

    void InitWithParentTexture(Texture* parent, const Rectangle& rect,
                               float scale);
    bool InitWithFile(const std::string& file, ImageType imageType,
                      float scale);
    bool InitWithResource(const std::string& resource, float scale);
    void InitWithData(const void* data, int width, int height,
                      PixelFormat format);
    void Create(int width, int height, PixelFormat format, float scale);
    void CreateWithData(const void* data, int width, int height,
                        PixelFormat format, float scale);
    boost::intrusive_ptr<Texture> CreateSubtexture(const Rectangle& rect,
                                                   float scale);

    bool Load();
    void Unload();
    void Reload(bool unload);
    void LoadFromParentTexture();

    void SetDefaultParameters(bool mipmaps);
    void SetNearestFiltering();

    int TextureRetainCount() const;

    Vector2 ConvertSubtextureCoordinatesToParentTextureCoordinates(
        const Vector2& coordinates);

    bool LoadFromProtobufMessage(const Proto::Texture& message);
    void SaveToProtobufMessage(Proto::Texture* message) const;

    // TextureBindingInterface overrides.
    void GetBindings(std::vector<Binding>* bindings) override;
    BindingValue ValueForBindedProperty(int property) override;
    void SetValueForBindedProperty(int property,
                                   const BindingValue& value) override;
    void GetEnumValuesForBindedProperty(
        int property, std::vector<BindingValue>* values) override;
    void PerformBindedAction(int action) override;
    void UpdateBindingDependencies() override;

    unsigned int GLTexture() const { return m_glTexture; }
    const std::string& Name() const { return m_string64; }
    const Rectangle& SubRect() const { return m_subRect; }
    // Normalized UV rect [u, v, du, dv] at Texture+124 — the four floats
    // AddRectangle expects.  Subtextures have these filled by
    // LoadFromParentTexture / CreateSubtexture; root textures have
    // [0, 0, w/pw, h/ph].  Cast to Rectangle for direct use as a srcRect.
    const float* UVTransform() const { return m_uvTransform; }
    float Width() const { return m_subRect.width; }
    float Height() const { return m_subRect.height; }
    const std::vector<boost::intrusive_ptr<Texture>>& Subtextures() const { return m_subtextures; }
    // Read by TextureLibrary::TextureLoaded/TextureUnloaded (0x5517C4 /
    // 0x5517E4), which add/subtract +104 for every texture that has no
    // parent; +104 is therefore the texture's byte total.
    Texture* Parent() const { return m_parent; }
    int ByteSize() const { return m_field104; }

    friend class IVideoDecoder;
    friend class AndroidVideoDecoder;
    friend class LinuxVideoDecoder;
    friend class WindowsVideoDecoder;

private:
    TextureDelegate* m_delegate;                            // +24
    std::string m_resourcePath;                             // +32
    int m_state;                                            // +56
    int m_field60;                                          // +60
    std::string m_string64;                                 // +64
    unsigned int m_glTexture;                               // +88
    int m_paddedWidth;                                      // +92
    int m_paddedHeight;                                     // +96
    PixelFormat m_pixelFormat;                              // +100
    int m_field104;                                         // +104
    Rectangle m_subRect;                                    // +108
    float m_uvTransform[4];                                 // +124
    float m_scale;                                          // +140
    Texture* m_parent;                                      // +144
    std::vector<boost::intrusive_ptr<Texture>> m_subtextures; // +152
    TextureConversionInfo* m_field176;                      // +176
    std::uint64_t m_field184;                               // +184
    unsigned char m_field192;                               // +192

};

} // namespace Caver

#endif
