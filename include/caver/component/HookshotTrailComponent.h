#ifndef CAVER_HOOKSHOTTRAILCOMPONENT_H
#define CAVER_HOOKSHOTTRAILCOMPONENT_H

#include "GlowComponent.h"
#include "../core/Color.h"
#include "../core/FloatColor.h"
#include "../core/Vector2.h"
#include "../core/Vector3.h"
#include <cstddef>
#include <cstdint>

namespace Caver {

class RenderingContext;
struct Matrix4;

struct HookshotTrailVertex {
    Vector3 position; // +0
    Color color;      // +12
};
static_assert(sizeof(HookshotTrailVertex) == 16, "sizeof HookshotTrailVertex must be 16");

class HookshotTrailComponent : public GlowComponent {
public:
    HookshotTrailComponent();
    virtual ~HookshotTrailComponent();

    static long Interface();

    using GlowComponent::InitWithComponent;
    void InitWithComponent(const HookshotTrailComponent* other);

    virtual void LoadFromProtobufMessage(const Proto::Component& message);
    virtual void SaveToProtobufMessage(Proto::Component* message) const;
    virtual bool ShouldSave();
    virtual void Prepare();
    virtual void GetBindings(std::vector<Binding>* bindings);
    virtual void GetEnumValuesForBindedProperty(int property, std::vector<BindingValue>* values);
    virtual BindingValue ValueForBindedProperty(int property);
    virtual void SetValueForBindedProperty(int property, const BindingValue& value);
    virtual void PerformBindedAction(int action);
    virtual bool ImplementsInterface(long interfaceId);
    virtual void RegisterInterfaces(ComponentManager* manager);
    virtual bool RequiresUpdate() const;
    virtual void Update(float deltaTime);
    virtual Component* Clone() const;
    virtual std::string className() const;
    virtual void Draw(RenderingContext* context, const Matrix4& matrix, const Vector3& position, const Vector3& rotation);

    void CreateVertices();
    void UpdateVertices();

    HookshotTrailVertex* m_vertices;    // +128
    int m_vertexCount;                  // +136
    uint16_t* m_indices;                // +144
    int m_triangleCount;                // +152
    Vector2 m_startPoint;               // +156
    Vector2 m_endPoint;                 // +164
    FloatColor m_color;                 // +172
};

static_assert(sizeof(HookshotTrailComponent) == 0xC0, "sizeof HookshotTrailComponent must be 0xC0");

} // namespace Caver

#endif // CAVER_HOOKSHOTTRAILCOMPONENT_H
