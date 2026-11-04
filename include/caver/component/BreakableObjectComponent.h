// Recovered from libswordigo.so (1.4.13) evidence — vtable
// _ZTVN5Caver24BreakableObjectComponentE at 0x61DF08 (0x108 bytes:
// primary address point 0x61DF08, IBindable secondary 0x61E020,
// IUpdateable secondary 0x61E060).
//
// sizeof 0xB8 (184 bytes) is proven by:
//   operator new(0xB8u) in Create() @0x350908 and Clone() @0x35679C.
#ifndef CAVER_BREAKABLEOBJECTCOMPONENT_H
#define CAVER_BREAKABLEOBJECTCOMPONENT_H

#include <string>
#include <vector>

#include "Component.h"
#include "DamageComponent.h"
#include "../core/Color.h"
#include "../scripting/Program.h"

namespace Caver {

class BreakableObjectComponent : public Component {
public:
    BreakableObjectComponent();
    virtual ~BreakableObjectComponent();

    static BreakableObjectComponent* Create();
    static long Interface();

    virtual BreakableObjectComponent* Clone() const;
    void InitWithComponent(const BreakableObjectComponent* other);

    virtual void LoadFromProtobufMessage(const Proto::Component& message);
    virtual void SaveToProtobufMessage(Proto::Component* message) const;
    virtual bool ShouldSave();

    virtual void GetBindings(std::vector<Binding>* bindings);
    virtual void GetEnumValuesForBindedProperty(int property, std::vector<BindingValue>* values);
    virtual BindingValue ValueForBindedProperty(int property);
    virtual void SetValueForBindedProperty(int property, const BindingValue& value);
    virtual void PerformBindedAction(int action);

    virtual int componentCategories() const;
    virtual bool ImplementsInterface(long interfaceId);
    virtual void RegisterInterfaces(ComponentManager* manager);
    virtual bool RequiresUpdate() const;
    virtual void Update(float deltaTime);
    virtual bool HandleMessage(int message, void* data);
    virtual std::string className() const;

    void Break();
    void FlashModel(const Color& color, float duration);

    bool m_broken;                          // +104 (ctor false; checked in Break)
    bool m_breaksOnImpact;                  // +105 (proto breaks_on_impact, binding 0)
    char _pad106[2];                        // +106
    int m_numHitsToBreak;                   // +108 (proto num_hits_to_break, binding 1)
    SpecialDamageType m_requiredDamageType; // +112 (proto required_damage_type, binding 2)
    char _pad116[4];                        // +116
    Program m_onBreak;                      // +120 (proto on_break, binding 3; sizeof 0x38 = 56)
    bool m_flashActive;                     // +176 (model flash active flag)
    char _pad177[3];                        // +177
    float m_flashTimer;                     // +180 (flash countdown timer)
};

// Golden aarch64 NDK-libc++ layout anchor; MSVC STL member sizes differ,
// so this assert is Android/Linux-only (the Windows build is self-consistent
// under MSVC STL and does not claim golden byte-layout parity).
#ifndef _MSC_VER
static_assert(sizeof(BreakableObjectComponent) == 0xB8,
              "BreakableObjectComponent sizeof must be 184 bytes");
#endif

} // namespace Caver

#endif // CAVER_BREAKABLEOBJECTCOMPONENT_H
