// Recovered from libswordigo.so (1.4.13) evidence — Caver::VaryingColor,
// an HSLA colour plus per-component random variances.
//
// Layout (32 bytes, 8 floats), from VaryingColor::RandomColor
// (0x514E84): components [0..3] are the base (h, s, l, a) handed to
// FloatColor::FromHSLA, and [4..7] are the matching variances — each
// component c with variance > 0.0001 is perturbed by
// (fastrandom()/FASTRANDOM_MAX - 0.5) * variance (the alpha result is
// additionally clamped to [0, 1]).
#ifndef CAVER_VARYINGCOLOR_H
#define CAVER_VARYINGCOLOR_H

#include "../core/FloatColor.h"

namespace Caver {

struct VaryingColor {
    float h;                    // +0
    float s;                    // +4
    float l;                    // +8
    float a;                    // +12
    float hueVariance;          // +16
    float saturationVariance;   // +20
    float lightnessVariance;    // +24
    float alphaVariance;        // +28

    FloatColor RandomColor() const;
};

} // namespace Caver

#endif
