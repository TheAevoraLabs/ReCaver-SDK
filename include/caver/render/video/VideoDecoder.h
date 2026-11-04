// Video playback abstraction layer for CaverEngine / ReCaver.
// Platform-agnostic video texture stream architecture.
// Credit: Quantum Creeper / Aevora Labs.
// Inspired by sw-glb / sw_asset_plus video architecture by raijin.

#ifndef CAVER_VIDEODECODER_H
#define CAVER_VIDEODECODER_H

#include <string>
#include <map>
#include <mutex>
#include <boost/shared_ptr.hpp>

namespace Caver {

class Texture;

class IVideoDecoder {
public:
    virtual ~IVideoDecoder() = default;

    virtual bool Open(Texture* texture, const std::string& path, float scale) = 0;
    virtual void Close() = 0;
    virtual void Update(float dt) = 0;

    virtual bool IsValid() const = 0;
    virtual int GetWidth() const = 0;
    virtual int GetHeight() const = 0;
    virtual float GetFPS() const = 0;
    virtual unsigned int GetTextureId() const = 0;
};

class VideoManager {
public:
    static VideoManager& Instance();

    void RegisterDecoder(Texture* texture, boost::shared_ptr<IVideoDecoder> decoder);
    void UnregisterDecoder(Texture* texture);
    boost::shared_ptr<IVideoDecoder> GetDecoder(Texture* texture);
    void UpdateAll(float dt);
    void Clear();

private:
    VideoManager() = default;
    ~VideoManager();
    VideoManager(const VideoManager&) = delete;
    VideoManager& operator=(const VideoManager&) = delete;

    std::mutex m_mutex;
    std::map<Texture*, boost::shared_ptr<IVideoDecoder> > m_decoders;
};

class VideoDecoderFactory {
public:
    static boost::shared_ptr<IVideoDecoder> Create(Texture* texture, const std::string& path, float scale);
};

} // namespace Caver

#endif // CAVER_VIDEODECODER_H
