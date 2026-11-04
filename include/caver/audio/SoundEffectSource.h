// Recovered from libswordigo.so (1.4.13) evidence — IDA Pro / Hex-Rays
// exports (~/workspace/analysis/swordigo/functions/Caver/SoundEffectSource/)
// arbitrated against the golden binary's disassembly, relocations and
// vtables. Graded with tools/parity (systems slice).
//
// Caver::SoundEffectSource — one playing instance of a SoundEffect: an
// OpenAL source bound to the effect's AudioBuffer, with a delegate that
// is notified once when playback finishes. Idle sources are kept in a
// file-static cache list (see the .cpp) and recycled by
// DequeueSourceForEffect.
//
// Object size is 0x50 (80) bytes, verified from the operator new(0x50)
// in DequeueSourceForEffect (0x50FF9C). Layout from the constructor
// stores (0x510510), Reset (0x510228), the accessors and the AudioSystem
// call sites (PlaySoundEffectAfterDelay writes the delay at +68 and the
// owning system at +32; AudioSystem::Update reads the elapsed time at
// +72 and the finished flag at +24):
//   +0   vptr (ReferenceCountedObject)
//   +8   int m_referenceCount (base class)
//   +16  ISoundEffectSourceDelegate* m_delegate
//   +24  bool m_finishedPlaying (delegate-notified latch)
//   +32  AudioSystem* m_audioSystem (owning system, raw pointer)
//   +40  boost::intrusive_ptr<SoundEffect> m_effect
//   +48  unsigned int m_source (ALuint)
//   +52  float m_gain (init 1.0f)
//   +56  Vector3 m_position
//   +68  float m_delay (remaining delay before Play, set by AudioSystem)
//   +72  float m_elapsedTime (accumulated by Update; reset by Stop/
//        Rewind/Reset)
// The vtable (0x634160, address point 0x634170) carries only the two
// destructor slots over the ReferenceCountedObject base, and the
// typeinfo is a 24-byte __si_class_type_info, matching that sole base.
#ifndef CAVER_SOUNDEFFECTSOURCE_H
#define CAVER_SOUNDEFFECTSOURCE_H

#include <boost/intrusive_ptr.hpp>

#include "caver/component/ReferenceCountedObject.h"
#include "caver/core/Vector3.h"

namespace Caver {

class AudioSystem;
class SoundEffect;
class SoundEffectSource;

// The delegate interface for playback completion. Its golden typeinfo
// (_ZTIN5Caver26ISoundEffectSourceDelegateE) is a plain 16-byte
// __class_type_info with no bases, and SoundEffectSource::Update calls
// slot 0 of the delegate's vtable, so the interface's first (and only
// recovered) virtual is SoundEffectSourceDidFinishPlaying and it has
// no virtual destructor.
class ISoundEffectSourceDelegate {
public:
    virtual void SoundEffectSourceDidFinishPlaying(SoundEffectSource* source) = 0;
};

class SoundEffectSource : public ReferenceCountedObject {
public:
    SoundEffectSource();
    virtual ~SoundEffectSource();

    // Takes a source from the cache (reset and re-bound to `effect`),
    // or creates and initialises a new one when the cache is empty.
    static boost::intrusive_ptr<SoundEffectSource> DequeueSourceForEffect(
        const boost::intrusive_ptr<SoundEffect>& effect);
    // Detaches the source's effect and returns it to the cache.
    static void EnqueueSource(const boost::intrusive_ptr<SoundEffectSource>& source);
    static void ClearSourceCache();

    // Generates the AL source and binds the effect. Returns false when
    // alGenSources reported an error (the effect is not bound then).
    bool InitWithEffect(const boost::intrusive_ptr<SoundEffect>& effect);
    void SetEffect(const boost::intrusive_ptr<SoundEffect>& effect);
    void SetDefaultParameters();
    // Clears delegate, owning system, the finished latch and the
    // elapsed time (NOT the effect, gain, position or delay).
    void Reset();

    void Update(float deltaTime);

    void Play();
    void Pause();
    void Stop();
    void Rewind();

    bool isActive() const;
    bool isPlaying() const;
    bool isPaused() const;
    bool isLooping() const;
    void setLooping(bool looping);

    void setGain(float gain);
    void setPosition(const Vector3& position);
    void setReferenceDistance(float distance);
    float referenceDistance() const;
    void setRolloffFactor(float factor);
    float rolloffFactor() const;

private:
    ISoundEffectSourceDelegate* m_delegate;          // +16
    bool m_finishedPlaying;                          // +24
    AudioSystem* m_audioSystem;                      // +32
    boost::intrusive_ptr<SoundEffect> m_effect;      // +40
    unsigned int m_source;                           // +48
    float m_gain;                                    // +52
    Vector3 m_position;                              // +56
    float m_delay;                                   // +68
    float m_elapsedTime;                             // +72
};

} // namespace Caver

#endif
