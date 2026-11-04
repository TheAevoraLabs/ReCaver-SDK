// Linux dynamic GStreamer video decoder for CaverEngine / ReCaver.
// Zero compile-time link dependencies: loads libgstreamer and libgstapp dynamically at runtime.
// Credit: Quantum Creeper / Aevora Labs.

#ifndef CAVER_LINUXVIDEODECODER_H
#define CAVER_LINUXVIDEODECODER_H

#if defined(__linux__) && !defined(__ANDROID__)

#include "VideoDecoder.h"
#include <cstdint>
#include <string>
#include <vector>

namespace Caver {

class Texture;

class LinuxVideoDecoder : public IVideoDecoder {
public:
    LinuxVideoDecoder();
    ~LinuxVideoDecoder() override;

    bool Open(Texture* texture, const std::string& path, float scale) override;
    void Close() override;
    void Update(float dt) override;

    bool IsValid() const override { return m_initialized; }
    int GetWidth() const override { return m_width; }
    int GetHeight() const override { return m_height; }
    float GetFPS() const override { return m_fps; }
    unsigned int GetTextureId() const override { return m_textureId; }

private:
    static bool LoadGStreamerSymbols();

    Texture* m_texture;
    std::string m_path;
    float m_scale;

    void* m_pipeline; // GstElement*
    void* m_sink;     // GstElement* (appsink)

    int m_width;
    int m_height;
    int m_potW;
    int m_potH;
    float m_fps;
    double m_frameInterval;
    double m_timeAccumulator;

    unsigned int m_textureId;
    std::vector<uint8_t> m_potRgba;
    bool m_initialized;
    bool m_hasNewFrame;
};

} // namespace Caver

#endif // __linux__ && !__ANDROID__

#endif // CAVER_LINUXVIDEODECODER_H
