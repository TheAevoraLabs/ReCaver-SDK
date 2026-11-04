// Recovered from libswordigo.so (1.4.13) evidence — vtable
// _ZTVN5Caver22ParticleFieldComponentE at 0x6053D0.
//
// sizeof 0x70 (112 bytes) is proven by:
//   operator new(0x70u) in Create() @0x350F08 and Clone() @0x39B6C8.
//
// Primary vtable at 0x622E50, IBindable secondary at 0x622F68,
// IUpdateable secondary at 0x622FA8. These three addresses are
// written verbatim by the constructor @0x39B5CC and the destructor body
// @0x39B670.
//
// Field offsets confirmed by the constructor body @0x39B5CC:
//
//   +0    vptr (primary)
//   +8    refcount
//   +16   vptr (IBindable secondary) — *((_QWORD*)this+2) = &off_622F68
//   +24   vptr (IUpdateable secondary) — *((_QWORD*)this+3) = &off_622FA8
//   +32   int m_identifier (Component)
//   +36   int m_field36   (Component)
//   +40   SceneObject* m_object (Component)
//   +48   bool m_flag48   (Component)
//   +56   std::string m_label (Component)
//   +80   std::map m_outlets (Component)
//
// Constructor zeroes the fields at +72, +80, +88 (sentinel),
// +96, and writes the three vptrs in the pattern documented above.
//
// Reduce-to-minimal component: Update() @0x39B664 and Draw()
// @0x39B66C have empty bodies in the golden binary. The implemented
// virtuals are the standard stub set (Interface, Clone, InitWithComponent,
// LoadFromProtobufMessage thunk, SaveToProtobufMessage thunk,
// ShouldSave, ImplementsInterface, RegisterInterfaces, RequiresUpdate,
// className). GetBindings/ValueForBindedProperty/SetValueForBindedProperty
// are all thunks to Component, and PerformBindedAction is an empty body.
#ifndef CAVER_PARTICLEFIELDCOMPONENT_H
#define CAVER_PARTICLEFIELDCOMPONENT_H

#include <string>

#include "Component.h"
#include "../render/RenderingContext.h"
#include "../core/Matrix4.h"
#include "../core/Vector3.h"

namespace Caver {

// Particle field component. A minimal component whose Update and Draw
// bodies are empty in the golden binary. Functionally present to carry
// the protobuf extension 257 and to satisfy the component registration
// pipeline.
//
// Primary vtable: _ZTVN5Caver22ParticleFieldComponentE @0x6053D0
// IBindable secondary: @0x622F68
// IUpdateable secondary: @0x622FA8
class ParticleFieldComponent : public Component {
public:
    ParticleFieldComponent();
    virtual ~ParticleFieldComponent();

    static ParticleFieldComponent* Create();
    static long Interface();

    virtual ParticleFieldComponent* Clone() const;
    void InitWithComponent(const ParticleFieldComponent* other);

    virtual void LoadFromProtobufMessage(const Proto::Component& message);
    virtual void SaveToProtobufMessage(Proto::Component* message) const;
    virtual bool ShouldSave();

    virtual void GetBindings(std::vector<Binding>* bindings);
    virtual void GetEnumValuesForBindedProperty(int property, std::vector<BindingValue>* values);
    virtual BindingValue ValueForBindedProperty(int property);
    virtual void SetValueForBindedProperty(int property, const BindingValue& value);
    virtual void PerformBindedAction(int action);

    virtual bool ImplementsInterface(long interfaceId);
    virtual void RegisterInterfaces(ComponentManager* manager);
    virtual bool RequiresUpdate() const;
    virtual void Update(float deltaTime);
    virtual void Draw(RenderingContext* context,
                      const Matrix4& transform,
                      const Vector3& eye,
                      const Vector3& viewDir);
    virtual std::string className() const;

    char _pad104[8]; // +104..+111
};

static_assert(sizeof(ParticleFieldComponent) == 0x70,
              "ParticleFieldComponent sizeof must be 112 bytes");

} // namespace Caver

#endif // CAVER_PARTICLEFIELDCOMPONENT_H
