// Recovered from libswordigo.so (1.4.13) evidence — Caver::ParticleEmitter
// (the base emitter) plus Blast/Spark/Trail/Whoosh, the derived
// emitters that share this header in the original source (their small
// virtuals are WEAK symbols in the golden binary, i.e. defined in the
// class bodies; Fountain lives in its own header, made by an earlier
// slice). The base ctor/dtor/small virtuals/TitleForType/
// AddParticleType are likewise WEAK (header-defined); Clone,
// Save/LoadFromProtobufMessage, CreateFromType and
// ConvertLocalParticleToWorld are GLOBAL (ParticleEmitter.cpp).
//
// Layout (sizeof 216), from the ctor (0x491550), the destructor
// (0x4912B4), Save/Load (0x4E8270 / 0x4E84B8),
// ConvertLocalParticleToWorld (0x4E8DB8) and
// ParticleEmitterComponent::Update / PrepareEmitter (0x399798 /
// 0x3995F8), which write most fields directly:
//   +0   vptr
//   +8   boost::shared_ptr<ParticleSystem> (px +8, control block +16)
//   +24  bool finished flag: the one-shot emitters (Blast, Spark) set
//        it when they fire; ParticleEmitterComponent::Update only
//        updates the emitter while it is clear
//   +32  std::vector<int> particle type indices (AddParticleType
//        appends; the Updates pick a random entry, or a random type
//        of the system when the vector is empty)
//   +56  Vector3 originOffset (serialised as proto field 2)
//   +68  Vector3 world position (cached transform; set externally)
//   +80  Vector3 world velocity (cached; read by the timed emitters
//        with their velocityTrackFactor)
//   +92  float world rotation (added to particle rotation; mirrored
//        when flipped)
//   +96  float world scale (ctor: 1.0)
//   +100 bool flipped (ctor: false)
//   +101 bool useWorldMatrix (ctor: false; selects the Matrix4 path
//        in ConvertLocalParticleToWorld)
//   +104 Matrix4 world matrix (64 bytes, untouched by the ctor)
//   +168 VaryingColor colour (h/s/l/a + variances; the ctor converts
//        a (1,1,1,0) FloatColor to HSL into +168..+180)
//   +200 float* parameters (owned array of NumParameters() floats,
//        allocated lazily by LoadFromProtobufMessage; delete[]d in
//        the destructor; null means "use DefaultValueForParameter")
//   +208 float rotation sin (ctor: 0.0)
//   +212 float rotation cos (ctor: 1.0)
//
// Vtable order (golden-exact): ~ParticleEmitter, type, NumParameters,
// DefaultValueForParameter, TitleForParameter, Update.
#ifndef CAVER_PARTICLEEMITTER_H
#define CAVER_PARTICLEEMITTER_H

#include <string>
#include <vector>

#include <boost/shared_ptr.hpp>

// Vector3.h before ParticleSystem.h: Matrix4.h (pulled in by
// ParticleSystem.h) names Vector3 without including its header.
#include "../core/Vector3.h"
#include "../core/Matrix4.h"
#include "ParticleSystem.h"
#include "VaryingColor.h"

namespace Caver {

namespace Proto {
class ParticleEmitter;
}

class ParticleEmitter {
public:
    // Values proven by the LoadFromProtobufMessage factory switch and
    // the derived type() returns (Fountain = 5). Enumerator NAMES are
    // unrecovered; only the numbering is evidence.
    enum Type {
        TypeNone = 0,
        TypeBlast = 1,
        TypeSpark = 2,
        TypeTrail = 3,
        TypeWhoosh = 4,
        TypeFountain = 5
    };

    // The ctor is WEAK in the golden binary but is called
    // out-of-line by every derived ctor (52-byte derived ctors in
    // their own TUs), so it is defined once in ParticleEmitter.cpp.
    // The dtor is header-defined: the derived D0 bodies in the
    // golden binary have its teardown inlined.
    ParticleEmitter();
    virtual ~ParticleEmitter() { delete[] m_parameters; }

    virtual Type type() { return TypeNone; }

// Golden ParticleEmitter::NumParameters @0x4916FC returns 0.
    virtual unsigned int NumParameters() { return 0; }

// Golden ParticleEmitter::DefaultValueForParameter @0x491704 returns 0.0.
    virtual float DefaultValueForParameter(unsigned int index) {
        return 0.0f;
    }

    virtual std::string TitleForParameter(unsigned int index) {
        return "unknown";
    }

// Golden ParticleEmitter::Update @0x491734: empty body.
    virtual void Update(float dt) {}

    static std::string TitleForType(Type type) {
        std::string titles[6] = { "none", "blast", "spark",
                                  "trail", "whoosh", "fountain" };
        return titles[type];
    }

    static boost::shared_ptr<ParticleEmitter> CreateFromType(Type type);
    static boost::shared_ptr<ParticleEmitter> LoadFromProtobufMessage(
        const Proto::ParticleEmitter& message);

    boost::shared_ptr<ParticleEmitter> Clone();
    void SaveToProtobufMessage(Proto::ParticleEmitter* message);

    void AddParticleType(int particleType) {
        m_particleTypeIndices.push_back(particleType);
    }

    // Non-virtual; defined out-of-line in the golden binary
    // (0x4E8DB8). Returns the scaled end size it stores.
    float ConvertLocalParticleToWorld(Particle* particle);

    // The parameter access pattern used by every derived Update: the
    // lazily allocated parameter array when present, the class
    // default (virtual) otherwise. (Golden derived Updates re-read
    // the array / re-call the virtual at each use site, which is this
    // helper's shape.)
    float ValueForParameter(unsigned int index) {
        return m_parameters ? m_parameters[index]
                            : DefaultValueForParameter(index);
    }

    boost::shared_ptr<ParticleSystem> m_particleSystem; // +8
    bool m_enabled;                                     // +24
    std::vector<int> m_particleTypeIndices;             // +32
    Vector3 m_originOffset;                             // +56
    Vector3 m_worldPosition;                            // +68
    Vector3 m_worldVelocity;                            // +80
    float m_worldRotation;                              // +92
    float m_worldScale;                                 // +96
    bool m_flipped;                                     // +100
    bool m_useWorldMatrix;                              // +101
    Matrix4 m_worldMatrix;                              // +104
    VaryingColor m_color;                               // +168
    float* m_parameters;                                // +200
    float m_rotationSin;                                // +208
    float m_rotationCos;                                // +212
};

// Blast (type 1) and Spark (type 2) declare no constructor or
// destructor of their own in the golden binary and add no members
// (sizeof 216): CreateFromType / LoadFromProtobufMessage
// value-initialise them (zeroed storage, base ctor, vtable store).
class BlastParticleEmitter : public ParticleEmitter {
public:
    virtual Type type() { return TypeBlast; }

    virtual unsigned int NumParameters() { return 4; }

    virtual float DefaultValueForParameter(unsigned int index) {
        // The 4 defaults, read from the golden table at 0x64F9C8.
        static float values[4] = {
            25.0f, // 0 blastSize
            0.2f,  // 1 blastDuration
            10.0f, // 2 particleWidth
            60.0f  // 3 particleHeight
        };
        return values[index];
    }

    virtual std::string TitleForParameter(unsigned int index) {
        static std::string titles[4] = { "blastSize", "blastDuration",
                                         "particleWidth",
                                         "particleHeight" };
        return titles[index];
    }

    virtual void Update(float dt);
};

class SparkParticleEmitter : public ParticleEmitter {
public:
    virtual Type type() { return TypeSpark; }

    virtual unsigned int NumParameters() { return 4; }

    virtual float DefaultValueForParameter(unsigned int index) {
        // The 4 defaults, read from the golden table at 0x64FA14.
        static float values[4] = {
            40.0f, // 0 blastSize
            0.3f,  // 1 blastDuration
            3.0f,  // 2 particleWidth
            30.0f  // 3 particleHeight
        };
        return values[index];
    }

    virtual std::string TitleForParameter(unsigned int index) {
        static std::string titles[4] = { "blastSize", "blastDuration",
                                         "particleWidth",
                                         "particleHeight" };
        return titles[index];
    }

    virtual void Update(float dt);
};

// Trail (type 3) and Whoosh (type 4) add the emission-interval
// accumulator at +216 (sizeof 224), zeroed by their out-of-line
// constructors; they declare no destructor (the vtable D1 slot
// points at the base D2, as with Fountain).
class TrailParticleEmitter : public ParticleEmitter {
public:
    TrailParticleEmitter();

    virtual Type type() { return TypeTrail; }

    virtual unsigned int NumParameters() { return 7; }

    virtual float DefaultValueForParameter(unsigned int index) {
        // The 7 defaults, read from the golden table at 0x64FA24.
        static float values[7] = {
            0.05f, // 0 particleInterval
            0.4f,  // 1 particleAge
            1.0f,  // 2 particleScale
            20.0f, // 3 radius
            0.3f,  // 4 velocityTrackFactor
            0.0f,  // 5 initialSize
            1.0f   // 6 finalSize
        };
        return values[index];
    }

    virtual std::string TitleForParameter(unsigned int index) {
        static std::string titles[7] = { "particleInterval",
                                         "particleAge",
                                         "particleScale",
                                         "radius",
                                         "velocityTrackFactor",
                                         "initialSize",
                                         "finalSize" };
        return titles[index];
    }

    virtual void Update(float dt);

    float m_emissionTimer; // +216
};

class WhooshParticleEmitter : public ParticleEmitter {
public:
    WhooshParticleEmitter();

    virtual Type type() { return TypeWhoosh; }

    virtual unsigned int NumParameters() { return 4; }

    virtual float DefaultValueForParameter(unsigned int index) {
        // The 4 defaults, read from the golden table at 0x64FA40.
        static float values[4] = {
            0.05f, // 0 particleInterval
            0.4f,  // 1 particleAge
            1.0f,  // 2 particleScale
            20.0f  // 3 radius
        };
        return values[index];
    }

    virtual std::string TitleForParameter(unsigned int index) {
        static std::string titles[4] = { "particleInterval",
                                         "particleAge", "particleScale",
                                         "radius" };
        return titles[index];
    }

    virtual void Update(float dt);

    float m_emissionTimer; // +216
};

} // namespace Caver

#endif
