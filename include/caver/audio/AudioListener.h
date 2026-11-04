// Recovered from libswordigo.so (1.4.13) evidence — IDA Pro / Hex-Rays
// exports (~/workspace/analysis/swordigo/functions/Caver/AudioListener/)
// arbitrated against the golden binary's disassembly.
//
// Caver::AudioListener — the OpenAL listener state holder, embedded by
// value in AudioSystem at +64. It caches the listener gain, position,
// direction and up vector and pushes every change straight to OpenAL
// (alListenerf / alListener3f / alListenerfv), draining alGetError()
// after each push.
//
// Object size is 40 bytes, proven by the AudioSystem layout (the
// listener exactly fills AudioSystem +64..+103) and by every accessor's
// offsets:
//   +0   float m_gain (init 1.0f)
//   +4   Vector3 m_position (init 0, 0, 0)
//   +16  Vector3 m_direction (init 0, 0, -1)
//   +28  Vector3 m_upVector (init 0, 1, 0)
// The constructor (golden 0x50DCD0) stores gain 1.0f, zeroes the
// position and direction x/y, and loads direction z (-1) plus the up
// vector (0, 1, 0) from the 16-byte rodata constant at golden
// 0x25A4E0. The class has no vtable in the golden binary.
#ifndef CAVER_AUDIOLISTENER_H
#define CAVER_AUDIOLISTENER_H

#include "caver/core/Vector3.h"

namespace Caver {

class AudioListener {
public:
    AudioListener();

    void setGain(float gain);
    void setPosition(const Vector3& position);
    void setDirection(const Vector3& direction);
    void setUpVector(const Vector3& upVector);
    void UpdateOrientation();

    float m_gain;        // +0
    Vector3 m_position;  // +4
    Vector3 m_direction; // +16
    Vector3 m_upVector;  // +28
};

} // namespace Caver

#endif
