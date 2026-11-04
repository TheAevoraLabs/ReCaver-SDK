// Recovered from libswordigo.so (1.4.13) evidence — see docs/CORE_HEADERS.md.
// Layout: four floats, r then g then b then a.
#ifndef CAVER_FLOATCOLOR_H
#define CAVER_FLOATCOLOR_H

namespace Caver {

struct FloatColor {
    float r;
    float g;
    float b;
    float a;
    FloatColor() {}
    FloatColor(const FloatColor& o) : r(o.r), g(o.g), b(o.b), a(o.a) {}
    // The golden copies a FloatColor as a raw 16-byte vector store; the
    // explicit assignment keeps that shape without the deprecated implicit
    // operator the user-provided copy constructor would otherwise leave.
    FloatColor& operator=(const FloatColor& o) {
        r = o.r; g = o.g; b = o.b; a = o.a;
        return *this;
    }

    // Proven from the golden symbol _ZN5Caver10FloatColor8FromHSLAEffff
    // and its call sites in HealthComponent::TakeDamageWithImpact
    // (static; result returned through the sret pointer).
    static FloatColor FromHSLA(float h, float s, float l, float a);

    // Golden symbol _ZNK5Caver10FloatColor16GetHSLComponentsEPf (const);
    // writes h, s, l into out[0..2]. Declared here for the systems
    // slice's ParticleEmitter reconstruction (2026-10-23); the
    // definition belongs to the core slice.
    void GetHSLComponents(float* out) const;

    // _ZNK5Caver10FloatColor3HueEv @0x514AD0,
    // _ZNK5Caver10FloatColor10SaturationEv @0x514BB4 and
    // _ZNK5Caver10FloatColor9LightnessEv @0x514C3C: the same HSL math as
    // GetHSLComponents, each returning only the one component.
    float Hue() const;
    float Saturation() const;
    float Lightness() const;
};

} // namespace Caver

#endif
