#ifndef CAVER_SHAPECOMPONENT_H
#define CAVER_SHAPECOMPONENT_H

#include "Component.h"
#include "../core/Color.h"
#include "../core/Vector2.h"
#include "../physics/CollisionShape.h"

namespace Caver {

class RenderingContext;
struct Matrix4;

// Recovered from libswordigo.so (1.4.13) evidence — vtable _ZTVN5Caver14ShapeComponentE
// at 0x61AFD0 (sizeof 0x80 = 128 bytes). Base class for collision and utility shapes.
// Primary vtable slots:
//   0..32  Component virtual overrides
//   33     EditColor (returns Color)
//   34     Draw(RenderingContext*, const Matrix4&)
//   35     SetShape(const Shape&)
//   36     SetDefaultShapeOfType(Shape::Type)
class ShapeComponent : public Component {
public:
    ShapeComponent();
    virtual ~ShapeComponent();

    static long Interface();

    void InitWithComponent(const ShapeComponent* other);
    virtual void LoadFromProtobufMessage(const Proto::Component& message);
    virtual void SaveToProtobufMessage(Proto::Component* message) const;
    virtual bool ShouldSave();
    virtual void GetBindings(std::vector<Binding>* bindings);
    virtual void GetEnumValuesForBindedProperty(int property, std::vector<BindingValue>* values);
    virtual BindingValue ValueForBindedProperty(int property);
    virtual void SetValueForBindedProperty(int property, const BindingValue& value);
    virtual void PerformBindedAction(int action);
    virtual int componentCategories() const;  // @0x327198 (returns 4)
    virtual bool ImplementsInterface(long interfaceId);
    virtual void RegisterInterfaces(ComponentManager* manager);
    virtual std::string className() const;

    virtual Color EditColor() const;
    virtual void Draw(RenderingContext* context, const Matrix4& matrix);
    virtual void SetShape(const Shape& shape);
    virtual void SetDefaultShapeOfType(Shape::Type type);

    bool ContainsWorldPoint(const Vector2& point);
    Matrix4 WorldMatrix() const;
    void DrawShape(RenderingContext* context, const Shape& shape, const Matrix4& matrix);

    const Shape& shape() const { return m_shape; }
    Shape& shape() { return m_shape; }

    Shape m_shape;  // +104
};

} // namespace Caver

#endif // CAVER_SHAPECOMPONENT_H
