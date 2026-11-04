// Recovered from libswordigo.so (1.4.13) evidence.
//
// Caver::AudioBuffer — one decoded sound effect held in an OpenAL buffer:
// the AL buffer id, its sample format/frequency, and the raw PCM data the
// buffer was filled from (owned; freed by FreeDataImpl).
//
// Layout (32 bytes), established from the constructor stores (0x512510),
// InitWithStaticData (0x512568), FreeDataImpl (0x512848), the destructor
// (0x512524, which passes `this` itself to alDeleteBuffers — the buffer id
// is the first member) and the operator new(0x20) in
// SoundLibrary::BufferForResourceWithName (0x51165C):
//   +0   unsigned int m_buffer (ALuint)
//   +4   BufferFormat m_format
//   +8   int m_frequency (sample rate, Hz)
//   +16  unsigned char* m_data
//   +24  int m_dataSize (bytes)
// BufferFormat values are proven by GetAudioFileData's WAV-header decode
// (0x512630): bits-per-sample 8 -> 1/3 (mono/stereo), 16 -> 2/4, and by
// ALFormatForBufferFormat mapping 1..4 onto AL_FORMAT_* 4352..4355.
#ifndef CAVER_AUDIOBUFFER_H
#define CAVER_AUDIOBUFFER_H

#include <string>

namespace Caver {

class SoundEffectSource;

class AudioBuffer {
    // SoundEffectSource::SetEffect dereferences the buffer object to
    // read the AL buffer id at +0 (binary-proven at 0x5102A4).
    friend class SoundEffectSource;

public:
    enum BufferFormat {
        BufferFormatUnknown = 0,
        BufferFormatMono8 = 1,
        BufferFormatMono16 = 2,
        BufferFormatStereo8 = 3,
        BufferFormatStereo16 = 4
    };

    AudioBuffer();
    ~AudioBuffer();

    bool LoadFromFile(const std::string& fileName);
    bool InitWithStaticData(BufferFormat format, int frequency, int dataSize,
                            unsigned char* data);

    static int ALFormatForBufferFormat(BufferFormat format);
    static BufferFormat BufferFormatForALFormat(int alFormat);

private:
    bool LoadFromFileImpl(const std::string& fileName);
    void FreeDataImpl();

    unsigned int m_buffer;   // +0
    BufferFormat m_format;   // +4
    int m_frequency;         // +8
    unsigned char* m_data;   // +16
    int m_dataSize;          // +24
};

// Reads a canonical 44-byte-header WAV file from the Android asset
// manager and decodes it into a malloc'd data block (caller owns it).
// On success returns true and fills every output except that, for a
// bits-per-sample other than 8 or 16, *format is left untouched
// (binary-proven quirk at 0x512714-0x51277C).
bool GetAudioFileData(const std::string& fileName,
                      AudioBuffer::BufferFormat* format, void** data,
                      int* dataSize, int* frequency);

} // namespace Caver

#endif
