#ifndef CAVER_PORTALEFFECTCOMPONENT_H
#define CAVER_PORTALEFFECTCOMPONENT_H

#include "GlowComponent.h"
#include "GroundPolygonComponent.h"
#include "ComponentOutlet.h"
#include "TextureMappingComponent.h"
#include "../core/Vector2.h"
#include "../core/Vector3.h"
#include <vector>

namespace Caver {

class RenderingContext;

class PortalEffectComponent : public GlowComponent {
public:
    // Golden Vertex layout (CreateVertices 0x39C7FC, UpdateVertices
    // 0x39D1F4, Draw 0x39D2C0), stride 32. NOTE: an earlier revision
    // had color at +12 and texcoord at +16/+20 — swapped relative to
    // the golden — which fed the texcoord slot color bits and the
    // color slot UVs. Position is a Vector3 (z always 0.0f): the
    // golden writes 0 at +8 as the position z, and Draw binds the
    // position array with size 3.
    struct Vertex {
        Vector3 position;              // +0, +4, +8 (z always 0.0f)
        Vector2 textureCoordinate;     // +12, +16
        unsigned int color;            // +20
        float angle;                   // +24
        float distance;                // +28
    };

    static_assert(sizeof(Vertex) == 32,
                  "PortalEffectComponent::Vertex must be 32 bytes");

    PortalEffectComponent();
    virtual ~PortalEffectComponent();
    static PortalEffectComponent* Create();
    static long Interface();
    virtual PortalEffectComponent* Clone() const;
    void InitWithComponent(const PortalEffectComponent* other);
    virtual void LoadFromProtobufMessage(const Proto::Component& message);
    virtual void SaveToProtobufMessage(Proto::Component* message) const;
    virtual bool ShouldSave();
    virtual void GetBindings(std::vector<Binding>* bindings);
    virtual void GetEnumValuesForBindedProperty(int property, std::vector<BindingValue>* values);
    virtual BindingValue ValueForBindedProperty(int property);
    virtual void SetValueForBindedProperty(int property, const BindingValue& value);
    // 0x39D0F0: thunk to GlowComponent::PerformBindedAction.
    virtual void PerformBindedAction(int action);
    virtual void Prepare();
    virtual bool RequiresUpdate() const;
    virtual bool RequiresTexturing();
    virtual bool ImplementsInterface(long interfaceId);
    virtual void RegisterInterfaces(ComponentManager* manager);
    virtual std::string className() const;
    virtual void Update(float deltaTime);
    virtual void Draw(RenderingContext* context, const Matrix4& matrix,
                      const Vector3& cameraPosition, const Vector3& cameraUp);

    void CreateVertices();
    void UpdateVertices();

    ComponentOutlet<GroundPolygonComponent> m_polygon; // +128
    ComponentOutlet<TextureMappingComponent> m_textureMapping; // +152
    FloatColor m_color; // +176
    Vector3 m_speed; // +192
    int m_vertexPadding; // +204
    int m_vertexCount; // +208
    int m_vertexCapacity; // +212
    Vertex* m_vertices; // +216
    int m_indexCount; // +224
    int m_indexCapacity; // +228
    unsigned short* m_indices; // +232
    Vector2 m_scroll; // +240
};

static_assert(sizeof(PortalEffectComponent) == 0xF8,
              "PortalEffectComponent size must be 248 bytes");

} // namespace Caver

#endif
