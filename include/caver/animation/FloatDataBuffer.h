// Recovered from libswordigo.so (1.4.13) evidence.
//
// FloatDataBuffer is a flat, heap-allocated array of floats: the buffer
// every animation node writes its pose into. Layout — sizeof 16, proven
// by FloatDataBuffer's own golden functions (IDA exports,
// ~/workspace/analysis/swordigo/functions/Caver/FloatDataBuffer/) and by
// the 16-byte stride in boost::checked_array_deleter<FloatDataBuffer>
// (KeyframeAnimation holds one buffer per frame in a shared_array):
//   +0  int    m_count
//   +8  float* m_data
//
//   FloatDataBuffer(int)  0x325914: m_count = count;
//                                   m_data = new float[count]
//   ~FloatDataBuffer      0x325948: if (m_data) operator delete[](m_data)
//   Resize(int)           0x325958: delete[] the old array (even when the
//                                   size is unchanged), then allocate
//                                   exactly as the ctor does
//   FillZero()            0x32599C: memset(m_data, 0, 4 * (unsigned)m_count)
//   Multiply(float)       0x3259B0: every element *= factor
//   Accumulate(const&, float)              0x325A20: m_data[i] += other[i] * w
//   Accumulate(const&, float, const FloatDataTrack&)
//                                          0x325AD4: strided, by track type
//   AccumulateVector3(const&, float, uint, uint)     0x325C1C
//   AccumulateQuaternion(const&, float, uint, uint)  0x325C94
//
// The strided accumulate functions walk both buffers at the same float
// offset, starting at `start` and advancing by `stride`, for as long as
// the whole 3- or 4-float group still fits inside this buffer (the
// golden loop guard is a range check against m_data + m_count - width,
// not a trip count). Quaternion accumulation first negates this
// buffer's quaternion in place when dot(dst, src) < 0 (hemisphere
// flip), then adds src * weight component-wise. The tracked overload
// dispatches on FloatDataTrack::type: 4 = quaternion, 2 = vector3,
// anything else is a no-op.
//
// The default ctor is inline in this header (AnimKeysNode's ctor
// 0x325320 zeroes its embedded buffer in place with no call); every
// other member is GLOBAL in the golden binary and lives in
// FloatDataBuffer.cpp.
#ifndef CAVER_FLOATDATABUFFER_H
#define CAVER_FLOATDATABUFFER_H

namespace Caver {

struct FloatDataTrack;

class FloatDataBuffer {
public:
    FloatDataBuffer() : m_count(0), m_data(0) {}
    FloatDataBuffer(int count);
    ~FloatDataBuffer();

    void Resize(int count);
    void FillZero();
    void Multiply(float factor);
    void Accumulate(const FloatDataBuffer& other, float weight);
    void Accumulate(const FloatDataBuffer& other, float weight,
                    const FloatDataTrack& track);
    void AccumulateVector3(const FloatDataBuffer& other, float weight,
                           unsigned int start, unsigned int stride);
    void AccumulateQuaternion(const FloatDataBuffer& other, float weight,
                              unsigned int start, unsigned int stride);

    int m_count;    // +0
    float* m_data;  // +8
};

} // namespace Caver

#endif
