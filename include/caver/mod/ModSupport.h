// Caver::Mod compile-time gate. Credit: Quantum Creeper / Aevora Labs.
//
// The mod subsystem is strictly opt-in. It is enabled only when a build
// defines CAVER_MOD_SUPPORT and does NOT define CAVER_ORIGINAL:
//
//   CAVER_MOD_SUPPORT + (no CAVER_ORIGINAL)  -> mod code compiles in
//   CAVER_ORIGINAL                           -> mod code is preprocessed out
//   neither                                  -> mod code is preprocessed out
//
// Every mod translation unit and every engine-side call site wraps its
// body in `#if CAVER_MOD_ENABLED`, so a parity build against the golden
// libswordigo.so contains exactly zero mod code and zero extra fields.
#ifndef CAVER_MODSUPPORT_H
#define CAVER_MODSUPPORT_H

#if defined(CAVER_MOD_SUPPORT) && !defined(CAVER_ORIGINAL)
#define CAVER_MOD_ENABLED 1
#else
#define CAVER_MOD_ENABLED 0
#endif

#endif
