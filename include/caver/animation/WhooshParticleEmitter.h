// Recovered from libswordigo.so (1.4.13) evidence —
// Caver::WhooshParticleEmitter, the whoosh particle emitter
// (ParticleEmitter type 4, 4 parameters).
//
// The class itself is declared in ParticleEmitter.h alongside the
// base emitter and the Blast/Spark/Trail emitters (the original
// source grouping, per the proven header): ParticleEmitter base
// (216 bytes) plus one own member — the emission-interval
// accumulator at +216, zeroed by the ctor (0x4EB530) — sizeof 224.
// The class declares no destructor of its own in the golden binary:
// its vtable's D1 slot points straight at
// ParticleEmitter::~ParticleEmitter (D2), and only a D0 (0x4EBA0C,
// the base teardown inlined + operator delete) is emitted.
//
// The four small virtuals are WEAK symbols in the golden binary
// (type 0x4EBAAC, NumParameters 0x4EBAB4,
// DefaultValueForParameter 0x4EBABC with its defaults table at
// 0x64FA40, TitleForParameter 0x4EBACC with its titles at
// 0x651650), which pins them as definitions inside the class body
// in ParticleEmitter.h; the ctor (0x4EB530) and Update (0x4EB564)
// are GLOBAL (the .cpp).
//
// This header is the per-class include entry point, mirroring
// FountainParticleEmitter.h's file layout; the declaration it
// provides is the one in ParticleEmitter.h.
#ifndef CAVER_WHOOSHPARTICLEEMITTER_H
#define CAVER_WHOOSHPARTICLEEMITTER_H

#include "ParticleEmitter.h"

#endif
