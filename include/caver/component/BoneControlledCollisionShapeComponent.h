#ifndef CAVER_BONECONTROLLEDCOLLISIONSHAPECOMPONENT_H
#define CAVER_BONECONTROLLEDCOLLISIONSHAPECOMPONENT_H

#include "CollisionShapeComponent.h"
#include "ComponentOutlet.h"
#include "ModelComponent.h"
#include <string>

namespace Caver {

struct BoneInstanceData;

class BoneControlledCollisionShapeComponent : public CollisionShapeComponent {
public:
    BoneControlledCollisionShapeComponent();
    virtual ~BoneControlledCollisionShapeComponent();
    static BoneControlledCollisionShapeComponent* Create();
    static long Interface();
    virtual BoneControlledCollisionShapeComponent* Clone() const;
    void InitWithComponent(const BoneControlledCollisionShapeComponent* other);
    virtual void LoadFromProtobufMessage(const Proto::Component& message);
    virtual void SaveToProtobufMessage(Proto::Component* message) const;
    virtual bool ShouldSave();
    virtual void GetBindings(std::vector<Binding>* bindings);
    virtual void GetEnumValuesForBindedProperty(int property, std::vector<BindingValue>* values);
    virtual BindingValue ValueForBindedProperty(int property);
    virtual void SetValueForBindedProperty(int property, const BindingValue& value);
    virtual bool ImplementsInterface(long interfaceId);
    virtual void RegisterInterfaces(ComponentManager* manager);
    virtual bool RequiresUpdate() const;
    virtual void Update(float deltaTime);
    virtual void UpdateCollisionShape();
    virtual std::string className() const;

    BoneInstanceData* BoneForName(const std::string& name);

    ComponentOutlet<ModelComponent> m_modelOutlet; // +504
    std::string m_boneName;                        // +528
    BoneInstanceData* m_bone;                      // +552
    float m_boneDepth;                             // +560
    char _pad564[4];
};

// Golden aarch64 NDK-libc++ layout anchor; MSVC STL member sizes differ,
// so this assert is Android/Linux-only (the Windows build is self-consistent
// under MSVC STL and does not claim golden byte-layout parity).
#ifndef _MSC_VER
static_assert(sizeof(BoneControlledCollisionShapeComponent) == 0x238,
              "BoneControlledCollisionShapeComponent size must be 568 bytes");
#endif

} // namespace Caver

#endif
