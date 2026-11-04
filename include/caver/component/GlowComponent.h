#ifndef CAVER_GLOWCOMPONENT_H
#define CAVER_GLOWCOMPONENT_H

#include "Component.h"
#include "../core/FloatColor.h"

namespace Caver {

class RenderingContext;
struct Matrix4;
struct Vector3;

// Recovered from libswordigo.so (1.4.13) evidence — vtable _ZTVN5Caver13GlowComponentE
// at 0x622560 (sizeof 0x80 = 128 bytes). Base class for visual glow effects.
// Primary vtable slots:
//   0..32  Component virtual overrides
//   33     RequiresTexturing() (returns false in base @0x37A8E4)
//   34     Draw(RenderingContext*, const Matrix4&, const Vector3&, const Vector3&) = 0
class GlowComponent : public Component {
public:
    GlowComponent();
    virtual ~GlowComponent();

    static long Interface();

    // Narrowed non-virtual overload (golden EPKS0_), not a vtable override.
    void InitWithComponent(const GlowComponent* other);

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
    virtual bool ShouldBeUpdatedInEditor() const;
    virtual bool IsVisualComponent() const;
    virtual std::string className() const;

    virtual bool RequiresTexturing();
    virtual void Draw(RenderingContext* context, const Matrix4& matrix, const Vector3& position, const Vector3& rotation) = 0;

    bool m_field104;        // +104 (1 in Glow ctor)
    FloatColor m_field108;  // +108 (1, 1, 1, 1 in Glow ctor)
};

} // namespace Caver

#endif // CAVER_GLOWCOMPONENT_H
