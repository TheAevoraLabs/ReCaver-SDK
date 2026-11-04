// Recovered from libswordigo.so (1.4.13) evidence — see the world
// lane's TransformControllerComponent results file for per-function
// parity status. Animates its owner's position / rotation / scale
// over time (the "TransformController" Lua library), optionally about
// a pivot origin, and can follow another object.
// sizeof 0xB8 (proven by Create()'s operator new(0xB8) and the Lua
// get-or-create wrappers' operator new(0xB8)). First own member sits
// at +104 (sizeof(Component) == 104). Member offsets are proven from
// the ctor store map, Update, RotateBy/To, TranslateBy/To, ScaleBy/To,
// FollowObject and the file-static Lua wrappers at 0x348274..0x348894.
#ifndef CAVER_TRANSFORMCONTROLLERCOMPONENT_H
#define CAVER_TRANSFORMCONTROLLERCOMPONENT_H

#include "Component.h"
#include "../core/Vector3.h"

namespace Caver {

class TransformControllerComponent : public Component {
public:
    TransformControllerComponent();
    virtual ~TransformControllerComponent();

    static TransformControllerComponent* Create();
    static long Interface();

    virtual Component* Clone() const;
    // NOT an override: the vtable slot 3 keeps the BASE function; this
    // is a non-virtual overload taking the narrowed type (EPKS0_).
    void InitWithComponent(const TransformControllerComponent* other);
    virtual void LoadFromProtobufMessage(const Proto::Component& message);
    virtual void SaveToProtobufMessage(Proto::Component* message) const;
    virtual bool ShouldSave();                                              // true
    virtual void GetBindings(std::vector<Binding>* bindings);
    virtual void GetEnumValuesForBindedProperty(int property, std::vector<BindingValue>* values);
    virtual BindingValue ValueForBindedProperty(int property);
    virtual void SetValueForBindedProperty(int property, const BindingValue& value);
    virtual void PerformBindedAction(int action);                           // empty
    virtual void Prepare();
    void RegisterLibrary();
    virtual bool ImplementsInterface(long interfaceId);
    virtual void RegisterInterfaces(ComponentManager* manager);
    virtual bool RequiresUpdate() const;                                    // true
    virtual bool UpdateWhenPaused() const;                                  // true
    virtual void Update(float deltaTime);
    virtual std::string className() const;                                  // "TransformController"

    float RotateBy(float rotation, float time);   // golden body returns the new target rotation
    void RotateTo(float rotation, float time);
    void TranslateBy(const Vector3& translation, float time);
    void TranslateTo(const Vector3& translation, float time);
    void ScaleBy(float scale, float time);
    void ScaleTo(float scale, float time);
    void FollowObject(const boost::intrusive_ptr<SceneObject>& object);

    Vector3 m_origin;                   // +104 — pivot for rotate/scale (Lua SetOrigin); zero = no pivot
    bool m_rotating;                    // +116
    float m_rotationTarget;             // +120
    float m_rotationDelta;              // +124 — rotation still to apply
    float m_rotationTime;               // +128 — time remaining
    bool m_translating;                 // +132
    Vector3 m_translationTarget;      // +136
    Vector3 m_translationDelta;         // +148 — translation still to apply
    float m_translationTime;            // +160 — time remaining
    bool m_scaling;                     // +164
    float m_scaleTarget;                // +168 — 1.0f in ctor
    float m_scaleTime;                  // +172 — time remaining
    boost::intrusive_ptr<SceneObject> m_followObject; // +176
};

} // namespace Caver

#endif
