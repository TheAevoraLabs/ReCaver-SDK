#ifndef CAVER_SHADOWCOMPONENT_H
#define CAVER_SHADOWCOMPONENT_H

#include "Component.h"
#include "../core/Rectangle.h"
#include "../core/Vector2.h"
#include "../core/Vector3.h"
#include "../scene/FastVector.h"
#include "../scene/Scene.h"
#include <boost/intrusive_ptr.hpp>

namespace Caver {

class ModelComponent;
class CollisionShapeComponent;
class Texture;

// Recovered from libswordigo.so (1.4.13) evidence — vtable _ZTVN5Caver15ShadowComponentE
// at 0x61ACA0 (sizeof 0xD8 = 216 bytes).
class ShadowComponent : public Component {
public:
    ShadowComponent();
    virtual ~ShadowComponent();

    static ShadowComponent* Create();
    static long Interface();

    virtual ShadowComponent* Clone() const override;
    void InitWithComponent(const ShadowComponent* other);

    virtual void LoadFromProtobufMessage(const Proto::Component& message) override;
    virtual void SaveToProtobufMessage(Proto::Component* message) const override;
    virtual bool ShouldSave() override;
    virtual void Prepare() override;

    virtual void GetBindings(std::vector<Binding>* bindings) override;
    virtual void GetEnumValuesForBindedProperty(int property, std::vector<BindingValue>* values) override;
    virtual BindingValue ValueForBindedProperty(int property) override;
    virtual void SetValueForBindedProperty(int property, const BindingValue& value) override;
    virtual void PerformBindedAction(int action) override;

    virtual bool ImplementsInterface(long interfaceId) override;
    virtual void RegisterInterfaces(ComponentManager* manager) override;
    virtual std::string className() const override;

    void UpdateShadowBounds();
    // z1/z2 are the quad's depth extent (midDepth -/+ radius * depth
    // factor), NOT texture U; see AddShadowSegment in ShadowComponent.cpp.
    void AddShadowSegment(const Vector2& p1, const Vector2& p2, float z1, float z2,
                          FastVector<ShadowVertex>* vertices);
    void CreateShadowWithShapes(CollisionShapeComponent** shapes, int count,
                                FastVector<ShadowVertex>* vertices);

    boost::intrusive_ptr<ModelComponent> m_modelComponent; // +104 (0x68)
    bool m_field112;                                       // +112 (0x70, default 1)
    char _pad113[3];                                       // +113
    float m_widthRadius;                                   // +116 (0x74)
    float m_depthRadius;                                   // +120 (0x78)
    Vector3 m_offset;                                      // +124 (0x7C)
    Rectangle m_bounds;                                    // +136 (0x88)
    float m_minDepth;                                      // +152 (0x98)
    float m_maxDepth;                                      // +156 (0x9C)
    float m_field160;                                      // +160 (0xA0, default 150.0f)
    Rectangle m_shadowBounds;                              // +164 (0xA4)
    bool m_field180;                                       // +180 (0xB4, default 0)
    char _pad181[3];                                       // +181
    float m_field184;                                      // +184 (0xB8)
    float m_field188;                                      // +188 (0xBC)
    float m_field192;                                      // +192 (0xC0)
    float m_field196;                                      // +196 (0xC4)
    float m_field200;                                      // +200 (0xC8)
    float m_field204;                                      // +204 (0xCC)
    boost::intrusive_ptr<Texture> m_texture;               // +208 (0xD0)
};

static_assert(sizeof(ShadowComponent) == 216, "ShadowComponent size must be 216 bytes");

} // namespace Caver

#endif // CAVER_SHADOWCOMPONENT_H
