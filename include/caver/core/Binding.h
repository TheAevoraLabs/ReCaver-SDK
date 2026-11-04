// Recovered from libswordigo.so (1.4.13) evidence — see docs/CORE_HEADERS.md.
// One bindable property/action descriptor, produced by
// IBindable::GetBindings into a std::vector<Binding> for the editor.
//
// Caver::Binding has NO out-of-line member functions in the arm64 binary
// (proven in ~/workspace/.scratch/parity/binding/recon/queue-binding.txt:
// zero census rows 'Caver::Binding::', no _ZN5Caver7Binding* / _ZTV /
// _ZTI symbol in the golden, no functions/Caver/Binding/ IDA directory).
// It is a plain aggregate struct, built by direct stack writes inside
// each component's GetBindings and moved by std::vector code.
// sizeof(Binding) = 48 is proven by
// std::vector<Binding>::__push_back_slow_path<Binding&&> @0x327508
// (operator new(48 * n), element stride 48, field copies at the offsets
// below, string destruction at element+8 in its cleanup loop).
#ifndef CAVER_BINDING_H
#define CAVER_BINDING_H

#include <string>

#include "BindingValue.h"

namespace Caver {

struct Binding {
    int m_property;                 // +0 — per-class property index (the same index later passed to ValueForBindedProperty / SetValueForBindedProperty / GetEnumValuesForBindedProperty). Evidence: DamageComponent::GetBindings @0x361E98 writes 0,1,2,... beside the names "minDamage","maxDamage","damageType"; CollisionShapeComponent::GetBindings @0x329654 writes 200..206; BoneControlledCollisionShapeComponent::GetBindings @0x3261E0 writes 301. Original member NAME is UNRECOVERED: m_property is descriptive only.
    char _pad4[4];                  // +4 — never written or read by any GetBindings body or by either vector<Binding> push_back path (@0x327508, @0x39B228)
    std::string m_name;             // +8 (24 bytes, +8..+31) — evidence: SSO string built at temp+8 in every GetBindings cited above (e.g. strcpy "minDamage" at temp+9 after the length byte at temp+8, @0x361E98); in-place vector writes copy it at element+8..+31 (@0x327508)
    int m_field32;                  // +32 — written as the low dword of the 8-byte store at temp+32 in every observed construction; the value is ALWAYS 1 (DamageComponent @0x361E98, CollisionShapeComponent @0x329654, SceneObject::GetBindings @0x501B48, ObjectTemplate::GetBindings @0x4F3BA4, Component::GetBindings @0x348E4C, Component::BindingForOutlet @0x349028). UNRECOVERED: name and meaning (no in-binary consumer reads a Binding back).
    BindingValueType m_valueType;   // +36 — the high dword of that same 8-byte store: 0x100000001 (String) for "controllingBoneName" @0x3261E0, 0x200000001 (Int) for the Component outlet bindings @0x348E4C, 0x400000001 (Bool) for "isGround" @0x329654, 0x500000001 (Float) for "minDepth" @0x329654, 0x600000001 (Program) for "onLoad" @0x4F3BA4, 0x700000001 (FloatColor) for "color" in LightComponent::GetBindings @0x33123C, 0x900000001 (Vector3) for "offset" @0x33123C — the high dword matches the BindingValueType of the property in every sampled component (3+ required: Damage, CollisionShape, BoneControlled, plus Light/SceneObject/ObjectTemplate/Component).
    int m_field40;                  // +40 — per-binding int written beside the pair above (e.g. 4 for "minDamage", 1 for "healthType"/"damageType", 2 for SceneObject "template" and ObjectTemplate "name", 5 for SceneObject "group", 0 in most bindings; Component::BindingForOutlet @0x349028 writes 1 when the outlet was found, 0 otherwise). UNRECOVERED: name and meaning (no in-binary consumer reads a Binding back).
    // +44: tail padding to the proven 48-byte size.
};

} // namespace Caver

#endif
