// Windows video decoder stub / Media Foundation interface for CaverEngine / ReCaver.
// Credit: Quantum Creeper / Aevora Labs.

#ifndef CAVER_WINDOWSVIDEODECODER_H
#define CAVER_WINDOWSVIDEODECODER_H

#if defined(_WIN32)

#include "VideoDecoder.h"
#include <string>

namespace Caver {

class Texture;

class WindowsVideoDecoder : public IVideoDecoder {
public:
    WindowsVideoDecoder();
    ~WindowsVideoDecoder() override;

    bool Open(Texture* texture, const std::string& path, float scale) override;
    void Close() override;
    void Update(float dt) override;

    bool IsValid() const override { return m_initialized; }
    int GetWidth() const override { return m_width; }
    int GetHeight() const override { return m_height; }
    float GetFPS() const override { return m_fps; }
    unsigned int GetTextureId() const override { return m_textureId; }

private:
    Texture* m_texture;
    std::string m_path;
    float m_scale;
    int m_width;
    int m_height;
    float m_fps;
    unsigned int m_textureId;
    bool m_initialized;
};

} // namespace Caver

#endif // _WIN32

#endif // CAVER_WINDOWSVIDEODECODER_H
