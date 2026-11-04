// Recovered from libswordigo.so (1.4.13) evidence — IDA Pro / Hex-Rays
// exports (~/workspace/analysis/swordigo/functions/Caver/AudioSystem/)
// arbitrated against the golden binary's disassembly and symbol sizes.
//
// Caver::AudioSystem — the OpenAL hub of the audio stack: owns the AL
// device/context, the delayed and active SoundEffectSource lists, the
// embedded AudioListener and the MusicPlayer, and brokers playback of
// SoundEffects (with the per-effect minimum play interval enforced
// against the active sources' elapsed times).
//
// Object size is 0x88 (136) bytes, verified from the operator new(0x88)
// in sharedSystem (0x50C6B8). Layout from the constructor stores
// (0x50C74C), the destructor / Shutdown release order and every
// accessor and call site:
//   +0   ALCdevice* m_device (opaque; alcOpenDevice result)
//   +8   ALCcontext* m_context
//   +16  std::vector<boost::intrusive_ptr<SoundEffectSource> >
//        m_delayedSources (delay > 0.005 sources, ticked by Update)
//   +40  std::vector<boost::intrusive_ptr<SoundEffectSource> >
//        m_activeSources (playing sources)
//   +64  AudioListener m_listener (40 bytes: gain +0, position +4,
//        direction +16, up vector +28 — its own slice's layout,
//        declared minimally below; AudioSystem only constructs it)
//   +104 boost::shared_ptr<MusicPlayer> m_musicPlayer (created by
//        Setup; coordinator-proven)
//   +120 bool m_soundEnabled (init true)
//   +124 float m_soundVolume (init 1.0f)
//   +128 bool m_audioInterruptionEnded (init false; set by
//        AudioInterruptionDidEnd, consumed/cleared by
//        EndAudioInterruptionIfNecessary)
//   +129 bool m_canEndAudioInterruption (init true)
// The class has no vtable in the golden binary (no typeinfo symbol).
#ifndef CAVER_AUDIOSYSTEM_H
#define CAVER_AUDIOSYSTEM_H

#include <string>
#include <vector>

#include <boost/intrusive_ptr.hpp>
#include <boost/shared_ptr.hpp>

#include "caver/core/Vector3.h"

// The listener is embedded by value at +64 (40 bytes, exactly filling
// AudioSystem +64..+103); its full declaration lives in its own header.
#include "caver/audio/AudioListener.h"

namespace Caver {

class MusicPlayer;
class SoundEffect;
class SoundEffectSource;

class AudioSystem {
public:
    AudioSystem();
    ~AudioSystem();

    static bool hasSharedSystem();
    static AudioSystem* sharedSystem();
    static void SetSharedSystem(AudioSystem* system);

    void Shutdown();

    void AudioInterruptionDidBegin();
    void AudioInterruptionDidEnd();
    void EndAudioInterruptionIfNecessary();
    void SetCanEndAudioInterruption(bool canEnd);

    bool CreateDevice();
    bool Setup();

    void SetMusicEnabled(bool enabled);
    void SetSoundEnabled(bool enabled);
    void SetMusicVolume(float volume);
    void SetSoundVolume(float volume);

    // All three Play* functions return the dequeued source by value
    // (an sret intrusive_ptr in the golden binary, proven by the X8
    // hidden return pointer at every call site; SoundLibrary's
    // PlayEffect Lua binding discards it and the golden releases it
    // through the refcount at +8 / vtable slot +8, exactly like a
    // SoundEffectSource intrusive_ptr). A null (empty) intrusive_ptr
    // is returned when playback is gated off (no context, sound
    // disabled) or when the effect's minimum play interval has not
    // elapsed on an active source.
    boost::intrusive_ptr<SoundEffectSource> PlaySoundEffectWithName(
        const std::string& name, float volume);
    boost::intrusive_ptr<SoundEffectSource> PlaySoundEffect(
        const boost::intrusive_ptr<SoundEffect>& effect, float volume);
    boost::intrusive_ptr<SoundEffectSource> PlaySoundEffectAfterDelay(
        const boost::intrusive_ptr<SoundEffect>& effect, float delay,
        float volume);

    bool CanPlaySoundEffect(
        const boost::intrusive_ptr<SoundEffect>& effect);
    void AddSoundEffectSource(
        const boost::intrusive_ptr<SoundEffectSource>& source);
    void CancelDelayedSoundEffectSource(
        const boost::intrusive_ptr<SoundEffectSource>& source);

    void Update(float deltaTime);

private:
    void* m_device;  // +0 (ALCdevice*)
    void* m_context; // +8 (ALCcontext*)
    std::vector<boost::intrusive_ptr<SoundEffectSource> > m_delayedSources; // +16
    std::vector<boost::intrusive_ptr<SoundEffectSource> > m_activeSources;  // +40
    AudioListener m_listener;                                               // +64
    boost::shared_ptr<MusicPlayer> m_musicPlayer;                           // +104
    bool m_soundEnabled;                                                    // +120
    float m_soundVolume;                                                    // +124
    bool m_audioInterruptionEnded;                                          // +128
    bool m_canEndAudioInterruption;                                         // +129
};

} // namespace Caver

#endif
