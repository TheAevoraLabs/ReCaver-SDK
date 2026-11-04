// Android NDK MediaCodec video decoder for CaverEngine / ReCaver.
// Credit: Quantum Creeper / Aevora Labs.
// Adapted from sw-glb by raijin.

#ifndef CAVER_ANDROIDVIDEODECODER_H
#define CAVER_ANDROIDVIDEODECODER_H

#if defined(__ANDROID__)

#include "VideoDecoder.h"
#include <cstdint>
#include <vector>
#include <string>

#include <media/NdkMediaCodec.h>
#include <media/NdkMediaExtractor.h>
#include <media/NdkMediaFormat.h>

namespace Caver {

class Texture;

class AndroidVideoDecoder : public IVideoDecoder {
public:
    AndroidVideoDecoder();
    ~AndroidVideoDecoder() override;

    bool Open(Texture* texture, const std::string& path, float scale) override;
    void Close() override;
    void Update(float dt) override;

    bool IsValid() const override { return m_initialized; }
    int GetWidth() const override { return m_width; }
    int GetHeight() const override { return m_height; }
    float GetFPS() const override { return m_fps; }
    unsigned int GetTextureId() const override { return m_textureId; }

private:
    bool OpenDataSource();
    bool DecodeFrame();
    void ConvertYUVtoRGBA(const uint8_t* yuvData, size_t yuvSize);

    Texture* m_texture;
    std::string m_path;
    float m_scale;

    AMediaExtractor* m_extractor;
    AMediaCodec* m_codec;
    int m_videoTrack;
    int m_fd;

    int32_t m_width;
    int32_t m_height;
    int32_t m_potW;
    int32_t m_potH;
    int32_t m_stride;
    int32_t m_sliceHeight;
    int32_t m_colorFormat;

    float m_fps;
    double m_frameInterval;
    double m_timeAccumulator;

    unsigned int m_textureId;
    std::vector<uint8_t> m_potRgba;
    bool m_hasNewFrame;
    bool m_initialized;
    bool m_sawInputEOS;
};

} // namespace Caver

#endif // __ANDROID__

#endif // CAVER_ANDROIDVIDEODECODER_H
