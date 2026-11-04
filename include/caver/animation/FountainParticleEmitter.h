// Recovered from libswordigo.so (1.4.13) evidence —
// Caver::FountainParticleEmitter, the fountain particle emitter
// (ParticleEmitter type 5, 15 parameters).
//
// Layout (sizeof 224): ParticleEmitter base (216 bytes) plus one own
// member — the emission-interval accumulator at +216, zeroed by the
// ctor (0x4E74F4) and advanced by Update. The class declares no
// destructor of its own in the golden binary: its vtable's D1 slot
// points straight at ParticleEmitter::~ParticleEmitter (D2), and only
// a D0 (0x4E7D64, the base teardown inlined + operator delete) is
// emitted — the faithful source shape is therefore no declared dtor.
//
// The four small virtuals are WEAK symbols in the golden binary (and
// DefaultValueForParameter's table is a weak GOT-reached object),
// which pins them as definitions inside the class body in the
// original header; the ctor and Update are GLOBAL (a .cpp).
#ifndef CAVER_FOUNTAINPARTICLEEMITTER_H
#define CAVER_FOUNTAINPARTICLEEMITTER_H

#include "ParticleEmitter.h"

namespace Caver {

class FountainParticleEmitter : public ParticleEmitter {
public:
    FountainParticleEmitter();

    virtual Type type() { return TypeFountain; }

    virtual unsigned int NumParameters() { return 15; }

    virtual float DefaultValueForParameter(unsigned int index) {
        // The 15 defaults, read from the golden table at 0x64F9D8.
        // The golden object is a writable weak symbol in .data — the
        // original table is not const.
        static float values[15] = {
            0.0f,   // 0 directionAngle
            90.0f,  // 1 directionSpread
            0.0f,   // 2 positionSpreadDir
            0.0f,   // 3 positionSpreadX
            0.0f,   // 4 positionSpreadY
            100.0f, // 5 speed
            0.0f,   // 6 speedVariance
            0.0f,   // 7 velocityTrackFactor
            1.0f,   // 8 particleScale
            0.0f,   // 9 initialParticleSize
            1.0f,   // 10 finalParticleSize
            0.05f,  // 11 particleInterval
            0.2f,   // 12 particleAge
            0.0f,   // 13 ageVariance
            0.0f    // 14 scaleVariance
        };
        return values[index];
    }

    virtual std::string TitleForParameter(unsigned int index) {
        // Titles from the string literals in the golden guard block
        // (0x4E7E24), in parameter order.
        static std::string titles[15] = {
            "directionAngle",
            "directionSpread",
            "positionSpreadDir",
            "positionSpreadX",
            "positionSpreadY",
            "speed",
            "speedVariance",
            "velocityTrackFactor",
            "particleScale",
            "initialParticleSize",
            "finalParticleSize",
            "particleInterval",
            "particleAge",
            "ageVariance",
            "scaleVariance"
        };
        return titles[index];
    }

    virtual void Update(float dt);

    float m_emissionTimer; // +216
};

} // namespace Caver

#endif
