#ifndef CAVER_WEAPONTRAILCOMPONENT_H
#define CAVER_WEAPONTRAILCOMPONENT_H

#include "Component.h"
#include "../animation/WeaponTrail.h"
#include "../core/FloatColor.h"

namespace Caver {

class RenderingContext;
struct Matrix4;
struct Vector3;

class WeaponTrailComponent : public Component {
public:
    WeaponTrailComponent();
    virtual ~WeaponTrailComponent();

    static WeaponTrailComponent* Create();
    static long Interface();

    using Component::InitWithComponent;
    void InitWithComponent(const WeaponTrailComponent* other);

    virtual void LoadFromProtobufMessage(const Proto::Component& message);
    virtual void SaveToProtobufMessage(Proto::Component* message) const;
    virtual bool ShouldSave();
    virtual void GetBindings(std::vector<Binding>* bindings);
    virtual void GetEnumValuesForBindedProperty(int property, std::vector<BindingValue>* values);
    virtual BindingValue ValueForBindedProperty(int property);
    virtual void SetValueForBindedProperty(int property, const BindingValue& value);
    virtual bool ImplementsInterface(long interfaceId);
    virtual void RegisterInterfaces(ComponentManager* manager);
    virtual void Update(float deltaTime);
    virtual Component* Clone() const;
    virtual std::string className() const;

    void ResetTrail();
    void AddSegment(const Vector3& point1, const Vector3& point2, float t);
    void Draw(RenderingContext* context, const Matrix4& matrix) const;
    void Draw(RenderingContext* context, const Matrix4& matrix);

    WeaponTrail m_trail;        // +104 (sizeof 64)
    FloatColor m_color;         // +168
    FloatColor m_tintColor;     // +184
};

static_assert(sizeof(WeaponTrailComponent) == 0xC8, "sizeof WeaponTrailComponent must be 0xC8");

} // namespace Caver

#endif // CAVER_WEAPONTRAILCOMPONENT_H
