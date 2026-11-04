// Recovered from libswordigo.so (1.4.13) evidence.
// A Component (104 bytes) plus its own members (proven by the C2
// ctor store map @0x3AC508, Create()'s operator new(0x150), and the
// field accesses in Load/Save/UpdateBounds/CreateMesh/Draw):
//   +104 ComponentOutlet<ShapeComponent> boundsShape outlet
//        (identifier at +112 = proto bounds_shape_id, target at +120;
//        the target's Shape sits at target+104)
//   +128 Rectangle local AABB (default assigned in the ctor body:
//        {-50, -50, 100, 100} from .rodata 0x25A0E0; refreshed from
//        Shape::Bounds by UpdateBounds)
//   +144 ComponentOutlet<TextureMappingComponent> textureMapping
//        outlet (identifier at +152 = proto texture_mapping_id,
//        target at +160; the target's TextureMapping sits at
//        target+128, its Texture* at target+128)
//   +168 FloatColor front color (binded property 2 "frontColor"),
//        initialised to {1, 1, 1, 1}
//   +184 FloatColor surface color (binded property 3
//        "surfaceColor"), initialised to {1, 1, 1, 1}
//   +200 FloatColor, initialised to {1, 1, 1, 1} — Draw multiplies
//        BOTH colours by this one; name UNRECOVERED
//   +216 VertexArrayObject (72 bytes)
//   +288 bool mesh-created latch (set by CreateMesh, cleared when
//        UpdateBounds detects a bounds change)
//   +296 void* cached copy of the VAO vertex buffer pointer
//   +304 void* cached copy of the VAO index buffer pointer
//   +312 bool mesh-dirty flag (set by Update/UpdateBounds, cleared
//        by UpdateMesh)
//   +316 int segment count (= llroundf(AABB width / 20))
//   +320 float segment step (= width / roundf(width / 20))
//   +324 int = 2 * segment count (surface half, in index triples)
//   +328 int = 2 * segment count (front half; Draw's gate)
//   +332 float time accumulator (advanced by Update)
// Object size 336 (see Create()'s operator new(0x150)).
#ifndef CAVER_WATERMESHCOMPONENT_H
#define CAVER_WATERMESHCOMPONENT_H

#include "Component.h"
#include "ComponentOutlet.h"
#include "ShapeComponent.h"
#include "TextureMappingComponent.h"
#include "../core/FloatColor.h"
#include "../core/Matrix4.h"
#include "../core/Rectangle.h"
#include "../core/Vector2.h"
#include "../core/Vector3.h"
#include "../physics/CollisionShape.h"
#include "../render/VertexArrayObject.h"

namespace Caver {

class RenderingContext;
class Texture;

class WaterMeshComponent : public Component {
public:
    WaterMeshComponent();
    virtual ~WaterMeshComponent();

    static WaterMeshComponent* Create();
    static long Interface();

    Component* Clone() const override;
    bool ShouldSave() override;
    void Prepare() override;
    bool RequiresUpdate() const override;
    bool UpdateWhenPaused() const override;
    void Update(float deltaTime) override;
    bool HasBounds() const override;
    Rectangle localAABB() const override;
    bool ImplementsInterface(long interfaceId) override;
    void RegisterInterfaces(ComponentManager* manager) override;
    std::string className() const override;

    // Non-virtual narrowed overload (the vtable slot keeps the
    // base function, same pattern as HealthComponent).
    void InitWithComponent(const WaterMeshComponent* other);

    void LoadFromProtobufMessage(const Proto::Component& message) override;
    void SaveToProtobufMessage(Proto::Component* message) const override;

    void GetBindings(std::vector<Binding>* bindings) override;
    void GetEnumValuesForBindedProperty(
        int property, std::vector<BindingValue>* values) override;
    BindingValue ValueForBindedProperty(int property) override;
    void SetValueForBindedProperty(int property,
                                   const BindingValue& value) override;
    void PerformBindedAction(int action) override;

    void CreateMesh();
    void UpdateMesh();
    void UpdateBounds();
    void Draw(RenderingContext* context, const Rectangle& rect,
              const Matrix4& matrix);

private:
    ComponentOutlet<ShapeComponent> m_boundsShape;           // +104
    Rectangle m_localAABB;                                   // +128
    ComponentOutlet<TextureMappingComponent> m_textureMapping; // +144
    FloatColor m_frontColor;                                 // +168
    FloatColor m_surfaceColor;                               // +184
    FloatColor m_field200;                                   // +200
    VertexArrayObject m_vertexArrayObject;                   // +216
    bool m_meshCreated;                                      // +288
    unsigned char m_pad289[7];
    void* m_vertexData;                                      // +296
    void* m_indexData;                                       // +304
    bool m_meshDirty;                                        // +312
    unsigned char m_pad313[3];
    int m_numSegments;                                       // +316
    float m_step;                                            // +320
    int m_field324;                                          // +324
    int m_field328;                                          // +328
    float m_time;                                            // +332
};

} // namespace Caver

#endif
