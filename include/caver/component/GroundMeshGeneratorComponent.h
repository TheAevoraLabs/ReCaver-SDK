#ifndef CAVER_GROUNDMESHGENERATORCOMPONENT_H
#define CAVER_GROUNDMESHGENERATORCOMPONENT_H

#include "Component.h"
#include "ComponentOutlet.h"
#include "GroundPolygonComponent.h"
#include "GroundMeshComponent.h"
#include "TextureMappingComponent.h"

namespace Caver {

class GroundMeshGeneratorComponent : public Component {
public:
    enum MeshType { MeshTypePlain = 0, MeshTypeRoundedHat = 1 };

    GroundMeshGeneratorComponent();
    virtual ~GroundMeshGeneratorComponent();
    static GroundMeshGeneratorComponent* Create();
    static long Interface();
    virtual GroundMeshGeneratorComponent* Clone() const;
    virtual void InitWithComponent(const Component* other);
    virtual void LoadFromProtobufMessage(const Proto::Component& message);
    virtual void SaveToProtobufMessage(Proto::Component* message) const;
    virtual bool ShouldSave();
    virtual void ComponentValuesChanged(Component* component);
    virtual void GetBindings(std::vector<Binding>* bindings);
    virtual void GetEnumValuesForBindedProperty(int property, std::vector<BindingValue>* values);
    virtual BindingValue ValueForBindedProperty(int property);
    virtual void SetValueForBindedProperty(int property, const BindingValue& value);
    // 0x3A97A0: action 11 reseeds m_randomSeed (+208) from fastrandom(),
    // and both 11 and 12 then regenerate the mesh; any other action is a
    // no-op.
    virtual void PerformBindedAction(int action);
    virtual bool ImplementsInterface(long interfaceId);
    virtual void RegisterInterfaces(ComponentManager* manager);
    virtual std::string className() const;

    void GenerateMesh();

    ComponentOutlet<GroundPolygonComponent> m_groundPolygon; // +104
    ComponentOutlet<GroundMeshComponent> m_targetMesh;       // +128
    ComponentOutlet<TextureMappingComponent> m_frontTexture;  // +152
    ComponentOutlet<TextureMappingComponent> m_surfaceTexture;// +176
    int m_meshType;                                           // +200
    float m_surfaceWidth;                                     // +204
    int m_randomSeed;                                          // +208
    float m_horizNoise;                                        // +212
    float m_hatHeight;                                         // +216
    float m_hatWidthOffset1;                                   // +220
    float m_hatWidthOffset2;                                   // +224
    char _pad228[4];
};

static_assert(sizeof(GroundMeshGeneratorComponent) == 0xE8,
              "GroundMeshGeneratorComponent size must be 232 bytes");

} // namespace Caver

#endif
