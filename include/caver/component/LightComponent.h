// Recovered from libswordigo.so (1.4.13) evidence — vtable
// _ZTVN5Caver14LightComponentE at 0x619B18 (0x178 bytes: primary address
// point 0x619B20, IBindable secondary 0x619C40, IUpdateable secondary
// 0x619C80), _ZTI at 0x619CC0 (a plain __si_class_type_info naming
// Caver::Component as the single base).
//
// sizeof 0xA8 (168) is proven three times by the golden: operator new(0xA8)
// in Create() @0x350848, in Clone() @0x332340, and in the
// "SetOverlayIntensity" Lua wrapper @0x3321BC.
//
// A fixed-function GLES1 light (ambient / directional / point) plus the
// screen-wide "overlay" light (type 4) that game scripts animate through
// the Lua library "Light" ("OverlayIntensity" / "SetOverlayIntensity").
// Only RequiresUpdate() is true for the overlay type: it is the one kind
// that animates its intensity in Update().
#ifndef CAVER_LIGHTCOMPONENT_H
#define CAVER_LIGHTCOMPONENT_H

#include <string>

#include "Component.h"
#include "../core/FloatColor.h"
#include "../core/Rectangle.h"
#include "../core/Vector3.h"

namespace Caver {

class RenderingContext;

// Golden: _ZN5Caver11typeStringsE — .bss at 0x64FD38, 144 bytes = six
// std::strings, initialised by the module ctor at 0x33169C from this TU.
// Entry 5 is empty and terminates every scan, so the type enum has five
// values. The spellings are read straight out of that initialiser:
// "unknown", "ambient", "directional", "point", "overlay", "".
extern std::string typeStrings[6];

class LightComponent : public Component {
public:
    // Proto::LightComponent::Type (Scene.proto) — Type_V0..Type_V4.
    enum Type {
        TypeUnknown = 0,
        TypeAmbient = 1,
        TypeDirectional = 2,
        TypePoint = 3,
        TypeOverlay = 4
    };

    LightComponent();
    virtual ~LightComponent();

    static LightComponent* Create();
    static long Interface();

    virtual LightComponent* Clone() const override;
    // NOT an override: vtable slot 3 keeps the BASE function; this is the
    // narrowed non-virtual overload (golden EPKS0_ @0x330E08).
    void InitWithComponent(const LightComponent* other);

    virtual void LoadFromProtobufMessage(const Proto::Component& message) override;
    virtual void SaveToProtobufMessage(Proto::Component* message) const override;
    virtual bool ShouldSave() override;
    virtual void Prepare() override;
    void RegisterLibrary();

    virtual void GetBindings(std::vector<Binding>* bindings) override;
    virtual void GetEnumValuesForBindedProperty(int property, std::vector<BindingValue>* values) override;
    virtual BindingValue ValueForBindedProperty(int property) override;
    virtual void SetValueForBindedProperty(int property, const BindingValue& value) override;

    virtual bool ImplementsInterface(long interfaceId) override;
    virtual void RegisterInterfaces(ComponentManager* manager) override;
    virtual bool RequiresUpdate() const override;
    virtual void Update(float deltaTime) override;
    virtual bool HasBounds() const override;
    virtual Rectangle localAABB() const override;
    virtual std::string className() const override;              // "Light"

    // Non-virtual (absent from the vtable — no slots beyond className).
    Vector3 Direction() const;                                   // @0x331CA4
    void AnimateToIntensity(float intensity, float duration);     // @0x331D94
    void Enable(RenderingContext* context, int lightIndex);       // @0x331DD0
    void Disable(RenderingContext* context);                      // @0x331FCC

    int m_type;                        // +104 (0x68) proto field 1
    float m_intensity;                 // +108 (0x6C) proto field 2
    FloatColor m_color;                // +112 (0x70) proto field 3
    // +128/+132 are only meaningful while m_animating: the ctor does NOT
    // touch them (verified store map of the golden ctor).
    float m_targetIntensity;           // +128 (0x80) — animation destination
    float m_intensitySpeed;            // +132 (0x84) — intensity units per second
    bool m_animating;                  // +136 (0x88)
    char _pad137[3];                   // +137
    float m_linearAttenuation;         // +140 (0x8C) proto field 4, GL_LINEAR_ATTENUATION
    float m_quadraticAttenuation;      // +144 (0x90) proto field 5, GL_QUADRATIC_ATTENUATION
    float m_radius;                    // +148 (0x94) proto field 7 (declared before field 6)
    Vector3 m_offset;                  // +152 (0x98) proto field 6
    int m_lightIndex;                  // +164 (0xA4) GL_LIGHT0 + index, -1 while disabled
};

static_assert(sizeof(LightComponent) == 168, "LightComponent size must be 168 bytes");

} // namespace Caver

#endif // CAVER_LIGHTCOMPONENT_H
