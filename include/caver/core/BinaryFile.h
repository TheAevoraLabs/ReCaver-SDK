// Recovered from libswordigo.so (1.4.13) evidence — IDA exports in
// ~/workspace/analysis/swordigo/functions/Caver/BinaryFile/ checked
// against the raw arm64 disassembly; the binary is the spec.
//
// BinaryFile: a file opened either as a plain FILE* (fopen) or as a
// gzip stream over an Android asset descriptor (gzdopen). Layout
// (sizeof 24, proven by the ctor store map @0x555190 and every body):
//   +0  int m_type — 0 = not open, 1 = plain file, 2 = gz/asset
//   +8  void* m_file — FILE* when m_type == 1, gzFile when == 2
//   +16 int m_size — ftell size for plain files; 0 for asset files
// Mode enumerator names UNRECOVERED; the values are proven by Open:
// 1 selects the write strings ("wb" / "wb9f"), anything else reads.
#ifndef CAVER_BINARYFILE_H
#define CAVER_BINARYFILE_H

#include <string>

namespace Caver {

class BinaryFile {
public:
    enum Mode {
        ModeRead = 0,  // UNRECOVERED name; value proven (not 1 = read)
        ModeWrite = 1, // UNRECOVERED name; value proven (1 = write)
    };

    BinaryFile(const std::string& path, Mode mode, bool useAssetManager);
    ~BinaryFile();

    bool Open(const std::string& path, Mode mode, bool useAssetManager);
    int ReadInt32();
    unsigned long WriteInt32(int value);
    // Argument/result types per the golden manglings: the byte-count
    // parameter is int (w-register) in both directions; ReadData's
    // result is the fread/gzread return widened, WriteData returns
    // the fwrite/gzwrite result directly.
    unsigned long ReadData(unsigned char* data, int size);
    unsigned long WriteData(const unsigned char* data, int size);
    std::string SHA1Hash();

    // The handle test every golden caller performs directly on the +8 field
    // (`if (m_file)` — e.g. Image::LoadFromTEXFile @0x55051C,
    // Image::SaveToTEXFile @0x5506C4), regardless of m_type.
    bool IsOpen() const { return m_file != 0; }

private:
    // The golden bodies repeatedly materialise the plain-file pointer
    // as (m_type == 1 ? m_file : 0) for the stdio calls (csel in the
    // disassembly); the gz calls test (m_type == 2 && m_file).
    void* FilePointer() const { return m_type == 1 ? m_file : 0; }

    int m_type;   // +0
    void* m_file; // +8
    int m_size;   // +16
};

} // namespace Caver

#endif
