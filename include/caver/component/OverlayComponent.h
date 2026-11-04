#ifndef CAVER_OVERLAYCOMPONENT_H
#define CAVER_OVERLAYCOMPONENT_H

#include "Component.h"
#include "../core/Rectangle.h"

namespace Caver {

class Camera;
class RenderingContext;

// Recovered from libswordigo.so (1.4.13) evidence — vtable _ZTVN5Caver16OverlayComponentE
// at 0x61A228 (sizeof 0x68 = 104 bytes). Base class for visual overlay components.
// Primary vtable slots:
//   0..32  Component virtual overrides
//   33     Draw(RenderingContext*, const Camera*, const Rectangle&) = 0 (pure virtual @slot 35 in layout)
class OverlayComponent : public Component {
public:
    OverlayComponent();
    virtual ~OverlayComponent();

    static long Interface();

    // Narrowed non-virtual overload (golden EPKS0_), not a vtable override.
    void InitWithComponent(const OverlayComponent* other);

    virtual void LoadFromProtobufMessage(const Proto::Component& message);
    virtual void SaveToProtobufMessage(Proto::Component* message) const;
    virtual bool ShouldSave();
    virtual void GetBindings(std::vector<Binding>* bindings);
    virtual void GetEnumValuesForBindedProperty(int property, std::vector<BindingValue>* values);
    virtual BindingValue ValueForBindedProperty(int property);
    virtual void SetValueForBindedProperty(int property, const BindingValue& value);
    virtual void PerformBindedAction(int action);
    virtual int componentCategories() const;  // returns 8
    virtual bool ImplementsInterface(long interfaceId);
    virtual void RegisterInterfaces(ComponentManager* manager);
    virtual std::string className() const;

    virtual void Draw(RenderingContext* context, const Camera* camera, const Rectangle& rect) = 0;
};

} // namespace Caver

#endif // CAVER_OVERLAYCOMPONENT_H
