// Recovered from libswordigo.so (1.4.13) evidence.
//
// Caver::SoundLibrary — the engine's registry of sound effects and of
// already-decoded audio buffers, and the "SoundLibrary" Lua library
// (single function: PlayEffect).
//
// Layout (48 bytes), established from the constructor stores
// (0x510EE4), sharedLibrary's operator new(0x30), Clear (0x511240) and
// the two tree-destroy helpers inlined into SetSharedLibrary
// (0x510E90): two libc++ std::map-family trees back to back, each with
// the standard __tree layout (begin node, end node, size):
//   +0   std::multimap<std::string, boost::intrusive_ptr<SoundEffect> >
//        m_effects  (AddEffect uses __emplace_multi; EffectForName uses
//        __count_multi / __equal_range_multi; the value's key string is
//        SoundEffect's name at SoundEffect+40)
//   +24  std::map<std::string, boost::weak_ptr<AudioBuffer> >
//        m_buffers  (BufferForResourceWithName uses
//        __emplace_unique_key_args == operator[], and lock()s the
//        weak_ptr; node value: string at +32, weak px at +56, weak
//        control block at +64)
// The class has no vtable and no out-of-line destructor in the golden
// binary: SetSharedLibrary destroys the two maps inline (buffers first,
// i.e. reverse declaration order) and calls operator delete itself.
#ifndef CAVER_SOUNDLIBRARY_H
#define CAVER_SOUNDLIBRARY_H

#include <map>
#include <string>

#include <boost/intrusive_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

namespace Caver {

class AudioBuffer;
class ProgramState;
class SoundEffect;

namespace Proto {
class SoundLibrary;
}

class SoundLibrary {
public:
    SoundLibrary();

    static SoundLibrary* sharedLibrary();
    static void SetSharedLibrary(SoundLibrary* library);

    void LoadFromProtobufMessage(const Proto::SoundLibrary& message);
    void SaveToProtobufMessage(Proto::SoundLibrary* message);

    void AddEffect(const boost::intrusive_ptr<SoundEffect>& effect);
    void Clear();

    boost::intrusive_ptr<SoundEffect> EffectForName(const std::string& name);
    boost::shared_ptr<AudioBuffer> BufferForResourceWithName(const std::string& name);

    void RegisterProgramLibrary(ProgramState* state);

private:
    std::multimap<std::string, boost::intrusive_ptr<SoundEffect> > m_effects; // +0
    std::map<std::string, boost::weak_ptr<AudioBuffer> > m_buffers;           // +24
};

} // namespace Caver

#endif
