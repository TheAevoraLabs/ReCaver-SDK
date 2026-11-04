// Asm-label helper. Credit: Quantum Creeper / Aevora Labs.
//
// The recovered source names many functions with Itanium asm labels
// (CAVER_ASM_LABEL("_ZN...")) so the declaration matches the golden's symbol
// exactly. On Itanium builds the label coincides with natural mangling
// and the link works; on MSVC the definition mangles ?-style while the
// label reference stays Itanium, so every labeled reference is an
// undefined symbol. CAVER_ASM_LABEL keeps the label on non-MSVC builds
// (byte-parity anchor) and drops it on MSVC, where the natural
// ?-mangled declaration matches the ?-mangled definition in the same
// build.
//
// It must NOT be used for the shim declarations (functions whose name
// differs from the labeled symbol, e.g. GUIViewCtorShim): those get
// MSVC forwarding definitions instead, because dropping their label
// would leave a declared-but-undefined function. Nor for the volatile
// asm barrier in TransformControllerComponent.cpp, which is a real
// compiler barrier, not a label.
#ifndef CAVER_ASMLABEL_H
#define CAVER_ASMLABEL_H

#ifdef _MSC_VER
#define CAVER_ASM_LABEL(x)
#else
#define CAVER_ASM_LABEL(x) __asm__(x)
#endif

#endif
