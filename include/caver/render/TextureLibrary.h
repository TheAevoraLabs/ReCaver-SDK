#ifndef CAVER_TEXTURELIBRARY_H
#define CAVER_TEXTURELIBRARY_H

#include <string>
#include <map>
#include <vector>
#include <boost/intrusive_ptr.hpp>
#include "Texture.h"

namespace Caver {

namespace Proto {
class Texture;
}

class TextureLibrary : public TextureDelegate {
public:
    static TextureLibrary* sharedLibrary();
    static void SetSharedLibrary(TextureLibrary* library);

    TextureLibrary();
    virtual ~TextureLibrary();

    boost::intrusive_ptr<Texture> TextureForName(const std::string& name, bool flag);
    void SetTextureForName(const std::string& name, const boost::intrusive_ptr<Texture>& texture);
    boost::intrusive_ptr<Texture> TextureFromProtobufMessage(const Proto::Texture& message, bool flag);
    void LoadTextureAtlasWithName(const std::string& name);

    void ReloadTextures(bool flag);
    void Clear();
    void PurgeTexturesIfNecessary();

    void TextureLoaded(Texture* texture) override;
    void TextureUnloaded(Texture* texture) override;

    void RefreshUnusedTexturesList();
    std::vector<boost::intrusive_ptr<Texture>> GetAllTextures() const;
    int TotalByteSize();
    void PrintMemoryUsageStats();

private:
    static TextureLibrary* s_sharedLibrary;
    std::map<std::string, boost::intrusive_ptr<Texture>> m_textures; // +8..+31

public:
    bool _field32;    // +32
    bool _field33;    // +33
    char _pad34[2];   // +34..+35
    int _field36;     // +36..+39 — running byte total of the resident top-level
                      // textures: TextureLoaded adds Texture::m_field104 (+104)
                      // and TextureUnloaded subtracts it, both only when the
                      // texture has no parent (0x5517C4 / 0x5517E4)
    int _field40;     // +40..+43
    char _pad44[4];   // +44..+47

private:
    std::vector<Texture*> m_unusedTextures; // +48..+71
};

} // namespace Caver

#endif // CAVER_TEXTURELIBRARY_H
