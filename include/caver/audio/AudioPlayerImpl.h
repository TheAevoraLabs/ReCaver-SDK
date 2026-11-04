// Recovered from libswordigo.so (1.4.13) evidence.
//
// Caver::AudioPlayerImpl — the Android AudioPlayer: a thin forwarding
// layer over the global MusicPlayerJNI bridge (the Java music player).
// Every operation either forwards straight to MusicPlayerJNI or is a
// local stub (Update / duration / currentTime).
//
// Layout (24 bytes), established from NewAudioPlayerImpl's
// operator new(0x18), the constructor stores at 0x50FA20 and every
// accessor body:
//   +0   vptr (AudioPlayer)
//   +8   int m_state (0 = initial, 1 = playing, 2 = paused,
//        3 = stopped; written by Play/Pause/Stop, read by isPlaying)
//   +12  bool m_looping (init false)
//   +16  float m_volume (init 1.0f)
// The typeinfo is a 24-byte __si_class_type_info (single base:
// AudioPlayer) and the 128-byte vtable carries the two destructor
// slots plus the twelve AudioPlayer slots, in base-declaration order.
#ifndef CAVER_AUDIOPLAYERIMPL_H
#define CAVER_AUDIOPLAYERIMPL_H

#include <string>

#include "caver/audio/MusicPlayer.h"

namespace Caver {

class AudioPlayerImpl : public AudioPlayer {
public:
    AudioPlayerImpl();
    virtual ~AudioPlayerImpl();

    virtual bool InitWithFile(const std::string& path);
    virtual void Update(float deltaTime);
    virtual void Play();
    virtual void Pause();
    virtual void Stop();
    virtual bool isPlaying() const;
    virtual bool isLooping() const;
    virtual void setLooping(bool looping);
    virtual float duration() const;
    virtual float currentTime() const;
    virtual float volume() const;
    virtual void setVolume(float volume);

private:
    int m_state;    // +8
    bool m_looping; // +12
    float m_volume; // +16
};

AudioPlayerImpl* NewAudioPlayerImpl();

} // namespace Caver

#endif
