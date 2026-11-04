// Recovered from libswordigo.so (1.4.13) evidence — see the world
// lane results for ObjectLinkControllerComponent. Pins its owner
// object (as a child) to a target SceneObject, optionally following
// one of the target model's skeleton bones. sizeof 0x140 (proven by
// Create()'s operator new(0x140)). Member offsets are proven from
// the ctor store map, InitWithComponent, Load/Save, LinkToObject,
// UpdateLink and the destructor teardown order.
//
// NOTE the golden's protobuf cross-mapping (bug-for-bug, proven in
// Load/SaveToProtobufMessage): the message's world_offset field is
// loaded into / saved from the +272 member (the local rotation used
// by UpdateLocalTransform and binding property 3 "localRotation"),
// and the message's local_rotation field into / from the +284 member
// (the world offset used by UpdateLink and binding property 4
// "worldOffset"). Do not "fix" this.
#ifndef CAVER_OBJECTLINKCONTROLLERCOMPONENT_H
#define CAVER_OBJECTLINKCONTROLLERCOMPONENT_H

#include "Component.h"
#include "../core/Vector3.h"
#include "../core/Matrix4.h"

namespace Caver {

class SceneObject;
class SkeletonInstance;
struct BoneInstanceData;

class ObjectLinkControllerComponent : public Component {
public:
    ObjectLinkControllerComponent();
    virtual ~ObjectLinkControllerComponent();

    static ObjectLinkControllerComponent* Create();
    static long Interface();

    virtual Component* Clone() const;                                       // returns ObjectLinkControllerComponent* (raw new)
    // NOT an override: the vtable slot 3 holds Component::InitWithComponent.
    // This is a non-virtual overload taking the narrowed type (its mangled
    // name uses EPKS0_ — the parameter is const ObjectLinkControllerComponent*).
    void InitWithComponent(const ObjectLinkControllerComponent* other);
    virtual void LoadFromProtobufMessage(const Proto::Component& message);
    virtual void SaveToProtobufMessage(Proto::Component* message) const;
    virtual bool ShouldSave();                                              // true
    virtual void GetBindings(std::vector<Binding>* bindings);
    virtual void GetEnumValuesForBindedProperty(int property, std::vector<BindingValue>* values);
    virtual BindingValue ValueForBindedProperty(int property);
    virtual void SetValueForBindedProperty(int property, const BindingValue& value);
    virtual void Prepare();
    void RegisterLibrary();
    virtual void PerformBindedAction(int action);                           // empty
    virtual bool ImplementsInterface(long interfaceId);
    virtual void RegisterInterfaces(ComponentManager* manager);
    virtual bool RequiresUpdate() const;                                    // true
    virtual void Update(float deltaTime);
    virtual std::string className() const;                                  // "ObjectLinkController" (20 chars, proven — NOT the full class name)

    std::string TargetObjectIdentifier() const;
    SkeletonInstance* TargetSkeletonInstance();
    void CreateLink();
    void UpdateLocalTransform();
    void LinkToObject(const boost::intrusive_ptr<SceneObject>& targetObject);
    void LinkToBone(const boost::intrusive_ptr<SceneObject>& targetObject, const std::string& boneName);
    void DestroyLink();
    void UpdateLink(float deltaTime);

    std::string m_targetObjectIdentifier;   // +104 — stored target id; CreateLink clears it after the lookup (golden behaviour)
    boost::intrusive_ptr<SceneObject> m_targetObject; // +128 — the linked target object
    bool m_field136;                        // +136 — set by LinkToObject once a link is made; gates CreateLink in Update (name UNRECOVERED)
    char _pad137[7];                        // +137
    boost::intrusive_ptr<Component> m_targetModelComponent; // +144 — the target's ModelComponent (pointee proven by its +144/+24 chain)
    std::string m_targetBoneIdentifier;   // +152
    BoneInstanceData* m_targetBone;         // +176 — raw pointer into the target SkeletonInstance's bone array (220-byte stride)
    boost::intrusive_ptr<Component> m_targetTransformComponent; // +184 — the target's TransformComponent (fallback when no bone)
    Matrix4 m_localTransform;               // +192 — built by UpdateLocalTransform; NOT initialised by the ctor in the golden
    bool m_hasLocalTransform;               // +256 — true when UpdateLocalTransform found a rotation axis > 0.001
    char _pad257[3];                        // +257
    Vector3 m_localOffset;                  // +260 — binding property 2 / proto local_offset
    Vector3 m_localRotation;                // +272 — radians; binding property 3 / proto world_offset (cross-mapped, see header note)
    Vector3 m_worldOffset;                  // +284 — binding property 4 / proto local_rotation (cross-mapped, see header note)
    char _pad296[16];                       // +296 — never read by any body; only byte +296 is zeroed (ctor's 37-byte store tail); content UNRECOVERED
    boost::intrusive_ptr<Component> m_ownTransformComponent; // +312 — m_object's TransformComponent, cached by Prepare/LinkToObject
};

} // namespace Caver

#endif
