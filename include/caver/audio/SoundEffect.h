// Recovered from libswordigo.so (1.4.13) evidence.
//
// Caver::SoundEffect — a named sound effect resource: the protobuf-loaded
// name/resource-name pair, volume and minimum play interval, plus the
// decoded OpenAL audio buffer it loads through SoundLibrary.
//
// Layout (112 bytes), established from the ctor/dtor bodies, from
// SoundLibrary::AddEffect (map key = string at +40, refcount at +8),
// SoundEffectSource::InitWithEffect (buffer pointer at +88) and
// AudioSystem::CanPlaySoundEffect (float at +108):
//   +0   vptr (ReferenceCountedObject)
//   +8   int m_referenceCount (base class)
//   +16  std::string (name/purpose UNRECOVERED — untouched by the
//        protobuf Load/Save pair; destroyed last in the dtor)
//   +40  std::string m_name
//   +64  std::string m_resourceName
//   +88  boost::shared_ptr<AudioBuffer> m_buffer (ptr +88, ctrl +96)
//   +104 float m_volume (default 1.0f)
//   +108 float m_minPlayInterval (default 0.1f)
// The golden typeinfo is a 24-byte __si_class_type_info (single base) and
// the vtable carries only the two destructor slots, matching
// ReferenceCountedObject as the sole base.
#ifndef CAVER_SOUNDEFFECT_H
#define CAVER_SOUNDEFFECT_H

#include <string>

#include <boost/shared_ptr.hpp>

#include "caver/component/ReferenceCountedObject.h"

namespace Caver {

namespace Proto {
class SoundEffect;
}

class AudioBuffer;
class SoundEffectSource;

class SoundEffect : public ReferenceCountedObject {
    // SoundEffectSource::SetEffect reads m_buffer (+88) and its volume
    // directly (binary-proven at 0x510288 / 0x510348); no accessor
    // symbol exists in the golden binary.
    friend class SoundEffectSource;
public:
    SoundEffect();
    virtual ~SoundEffect();

    void LoadFromProtobufMessage(const Proto::SoundEffect& message);
    void SaveToProtobufMessage(Proto::SoundEffect* message);

    const std::string& Name() const { return m_name; }

    void LoadBuffer();
    void FreeBuffer();

private:
    std::string m_string16;                        // +16 (UNRECOVERED name)
    std::string m_name;                            // +40
    std::string m_resourceName;                    // +64
    boost::shared_ptr<AudioBuffer> m_buffer;       // +88
    float m_volume;                                // +104
    float m_minPlayInterval;                       // +108
};

} // namespace Caver

#endif
