// Recovered from libswordigo.so (1.4.13) evidence.
// A Component (104 bytes) plus its own members:
//   +104 std::vector<boost::shared_ptr<MeshInstance>> (surface meshes)
//   +128 std::vector<boost::shared_ptr<MeshInstance>> (front meshes)
//   +152 Rectangle local AABB (returned verbatim by localAABB())
//   +168 FloatColor, initialised to {1, 1, 1, 1} (binded property 0)
//   +184 bool, initialised to false (binded property 1)
//   +188 FloatColor, initialised to {1, 1, 1, 1}
// Object size 208 (see Create()'s operator new(0xD0)).
#ifndef CAVER_GROUNDMESHCOMPONENT_H
#define CAVER_GROUNDMESHCOMPONENT_H

#include "Component.h"
#include "../core/FloatColor.h"
#include "../core/Matrix4.h"
#include "../core/Rectangle.h"
#include <boost/shared_ptr.hpp>
#include <vector>

namespace Caver {

class Mesh;
class MeshInstance;
class RenderingContext;

class GroundMeshComponent : public Component {
public:
    GroundMeshComponent();
    virtual ~GroundMeshComponent();

    static GroundMeshComponent* Create();
    static long Interface();

    Component* Clone() const override;
    bool ShouldSave() override;
    bool IsVisualComponent() const override;
    Rectangle localAABB() const override;
    bool ImplementsInterface(long interfaceId) override;
    void RegisterInterfaces(ComponentManager* manager) override;
    std::string className() const override;

    void InitWithComponent(const GroundMeshComponent* other);

    void LoadFromProtobufMessage(const Proto::Component& message) override;
    void SaveToProtobufMessage(Proto::Component* message) const override;

    void GetBindings(std::vector<Binding>* bindings) override;
    bool Process(
        std::vector<boost::intrusive_ptr<Component>>* components) override;

    void GetEnumValuesForBindedProperty(
        int property, std::vector<BindingValue>* values) override;
    BindingValue ValueForBindedProperty(int property) override;
    void SetValueForBindedProperty(int property,
                                   const BindingValue& value) override;
    void PerformBindedAction(int action) override;

    void AddSurfaceMesh(const boost::shared_ptr<Mesh>& mesh);
    void AddFrontMesh(const boost::shared_ptr<Mesh>& mesh);
    void RemoveAllMeshes();
    void GetAllMeshes(
        std::vector<boost::shared_ptr<MeshInstance>>* meshes);
    void DrawMesh(RenderingContext* context, MeshInstance* instance) const;
    void Draw(RenderingContext* context, const Rectangle& rect,
              const Matrix4& matrix) const;

private:
    std::vector<boost::shared_ptr<MeshInstance>> m_field104; // +104
    std::vector<boost::shared_ptr<MeshInstance>> m_field128; // +128
    Rectangle m_localAABB;                                   // +152
    FloatColor m_field168;                                   // +168
    bool m_field184;                                         // +184
    unsigned char m_pad185[3];
    FloatColor m_field188;                                   // +188
};

} // namespace Caver

#endif
