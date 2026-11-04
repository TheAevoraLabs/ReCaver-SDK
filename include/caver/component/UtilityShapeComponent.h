// Recovered from libswordigo.so (1.4.13) evidence — Caver::UtilityShapeComponent.
// A ShapeComponent with no members of its own: sizeof 0x80 (128) is proven by
// the operator new(0x80u) in both Create() @0x350598 and Clone() @0x3515D0,
// and by the base ShapeComponent's own proven 128-byte layout.
//
// Base: ShapeComponent (RTTI name is __si_class_type_info, base _ZTI
// ShapeComponent). The primary vtable address point 0x61C3C8 overrides only
// Slot 2 (Clone, covariant), slot 3 (vtable holds
// ShapeComponent::InitWithComponent — this class adds no narrowed overload),
// slot 33 EditColor, and the usual Component metadata slots (Implements-
// Interface, RegisterInterfaces, className). Load / Save / GetBindings /
// the other binding methods / Prepare / Update are all inherited unchanged.
#ifndef CAVER_UTILITYSHAPECOMPONENT_H
#define CAVER_UTILITYSHAPECOMPONENT_H

#include "ShapeComponent.h"

namespace Caver {

class UtilityShapeComponent : public ShapeComponent {
public:
    static UtilityShapeComponent* Create();
    static long Interface();

    virtual ~UtilityShapeComponent();

    virtual UtilityShapeComponent* Clone() const;
    virtual bool ImplementsInterface(long interfaceId);
    virtual void RegisterInterfaces(ComponentManager* manager);
    virtual Color EditColor() const;
    virtual std::string className() const;
};

static_assert(sizeof(UtilityShapeComponent) == 0x80,
              "UtilityShapeComponent sizeof must be 128 bytes");

} // namespace Caver

#endif // CAVER_UTILITYSHAPECOMPONENT_H
