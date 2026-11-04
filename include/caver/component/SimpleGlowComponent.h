#ifndef CAVER_SIMPLEGLOWCOMPONENT_H
#define CAVER_SIMPLEGLOWCOMPONENT_H

#include "GlowComponent.h"
#include "../core/Vector2.h"
#include "../core/Vector3.h"

namespace Caver {

class Camera;
class RenderingContext;

class SimpleGlowComponent : public GlowComponent {
public:
    SimpleGlowComponent();
    virtual ~SimpleGlowComponent();

    static SimpleGlowComponent* Create();
    static long Interface();
    virtual Component* Clone() const;
    using GlowComponent::InitWithComponent;
    void InitWithComponent(const SimpleGlowComponent* other);
    virtual void LoadFromProtobufMessage(const Proto::Component& message);
    virtual void SaveToProtobufMessage(Proto::Component* message) const;
    virtual bool ShouldSave();
    virtual void GetBindings(std::vector<Binding>* bindings);
    virtual void GetEnumValuesForBindedProperty(int property,
                                                std::vector<BindingValue>* values);
    virtual BindingValue ValueForBindedProperty(int property);
    virtual void SetValueForBindedProperty(int property,
                                           const BindingValue& value);
    virtual void PerformBindedAction(int action);
    virtual void Prepare();
    virtual bool RequiresUpdate() const;
    virtual bool ImplementsInterface(long interfaceId);
    virtual void RegisterInterfaces(ComponentManager* manager);
    virtual bool HasBounds() const;
    virtual Rectangle localAABB() const;
    virtual std::string className() const;
    virtual void Update(float deltaTime);
    virtual void Draw(RenderingContext* context, const Matrix4& matrix,
                      const Vector3& cameraPosition, const Vector3& cameraUp);

    void CreateVertexData();
    void AnimateToScale(float scale, float duration);

    void* m_vertexData;       // +128, vertex xyz followed by RGBA bytes
    int m_vertexCount;        // +136
    int m_vertexPadding;      // +140
    unsigned short* m_indices; // +144
    int m_indexCount;         // +152, three times the segment count
    FloatColor m_color;       // +156
    float m_size;             // +172
    int m_numSegments;        // +176
    Vector2 m_offset;         // +180
    float m_depth;            // +188
    float m_scale;            // +192
    float m_pulseAmount;      // +196
    float m_pulseTime;        // +200
    float m_pulseTimer;       // +204
    float m_targetScale;      // +208
    float m_scaleTimeRemaining; // +212
};

static_assert(sizeof(SimpleGlowComponent) == 0xD8,
              "SimpleGlowComponent size must be 216 bytes");

} // namespace Caver

#endif
