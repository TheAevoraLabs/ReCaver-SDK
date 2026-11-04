// Recovered from libswordigo.so (1.4.13) evidence — see
// ~/workspace/.scratch/parity/combat/SwingableWeaponComponent/RESULTS.md
// for per-function parity status and the evidence behind the layout.
// Swingable weapon component; sizeof 0x1C0 (proven by Create()'s
// operator new(0x1C0)). Member offsets are proven from the ctor,
// InitWithComponent, LoadFromProtobufMessage and the gameplay bodies.
#ifndef CAVER_SWINGABLEWEAPONCOMPONENT_H
#define CAVER_SWINGABLEWEAPONCOMPONENT_H

#include "Component.h"
#include "ComponentOutlet.h"
#include "CollisionShapeComponent.h"
#include "WeaponGlowComponent.h"
#include "SoundEffectComponent.h"
#include "ModelComponent.h"
#include "WeaponTrailComponent.h"
#include "../core/Vector3.h"
#include "../core/FloatColor.h"
#include "../core/Rectangle.h"
#include "../core/Matrix4.h"

namespace Caver {

class TransformComponent;
class DamageComponent;



class SwingableWeaponControllerComponent;

template <typename T>
using SwingableOutlet = ComponentOutlet<T>;

class SwingableWeaponComponent : public Component {
public:
    SwingableWeaponComponent();
    virtual ~SwingableWeaponComponent();

    static SwingableWeaponComponent* Create();
    static long Interface();

    virtual Component* Clone() const;
    // NOT an override: narrowed non-virtual overload (mangling EPKS0_);
    // the vtable slot 3 holds Component::InitWithComponent.
    void InitWithComponent(const SwingableWeaponComponent* other);
    virtual void LoadFromProtobufMessage(const Proto::Component& message);
    virtual void SaveToProtobufMessage(Proto::Component* message) const;
    virtual bool ShouldSave();                                              // true
    virtual void Prepare();
    virtual void GetBindings(std::vector<Binding>* bindings);
    virtual void GetEnumValuesForBindedProperty(int property, std::vector<BindingValue>* values);
    virtual BindingValue ValueForBindedProperty(int property);
    virtual void SetValueForBindedProperty(int property, const BindingValue& value);
    virtual int componentCategories() const;                                // 2
    virtual bool ImplementsInterface(long interfaceId);
    virtual void RegisterInterfaces(ComponentManager* manager);
    virtual std::string className() const;                                  // "SwingableWeapon" (golden, sic)

    virtual void Update(float deltaTime);
    virtual bool HandleMessage(int message, void* data);

    void SetController(SwingableWeaponControllerComponent* controller);
    void StartSwing(int direction, float delay, float duration);
    void StopSwing();
    void SetDamageEnabled(bool enabled);
    void SetGlowColor(FloatColor color);   // golden takes FloatColor BY VALUE (mangling ENS_10FloatColorE)
    void SetGlowIntensity(float intensity);
    void CreateGlowObject();
    void UpdateTransform(const Matrix4& matrix);
    // 16-byte plane (normal xyz, distance w) returned through sret in
    // the golden; modelled as FloatColor for its non-trivial ABI.
    // UNRECOVERED: the original return type's name.
    FloatColor CurrentSwingPlane();

    SwingableWeaponControllerComponent* m_controller;   // +104 — raw pointer (SetController; no refcount in the golden)
    SwingableOutlet<ModelComponent> m_model;            // +112 — outlet 0 "model"
    SwingableOutlet<CollisionShapeComponent> m_collisionShape; // +136 — outlet 1 "collisionShape"
    Rectangle m_field160;                               // +160 — swing bounds (current)
    Rectangle m_field176;                               // +176 — swing bounds (previous)
    SwingableOutlet<WeaponTrailComponent> m_trail;      // +192 — outlet 7 "trail"
    SwingableOutlet<WeaponTrailComponent> m_glowTrail;  // +216 — outlet 8 "glowTrail"
    SwingableOutlet<WeaponGlowComponent> m_glow;        // +240 — outlet 9 "glow"
    SwingableOutlet<ParticleEmitterComponent> m_impactParticleEmitter; // +264 — outlet 10 "impactParticleEmitter"
    SwingableOutlet<SoundEffectComponent> m_swingSound; // +288 — outlet 11 "swingSound"
    SwingableOutlet<SoundEffectComponent> m_damageImpactSound; // +312 — outlet 12 "damageImpactSound"
    boost::intrusive_ptr<TransformComponent> m_transformComponent; // +336 — cached by Prepare (raw assign pattern in the golden)
    boost::intrusive_ptr<DamageComponent> m_damageComponent;       // +344 — cached by Prepare
    boost::intrusive_ptr<WeaponGlowComponent> m_glowComponent;     // +352 — cached glow object
    bool m_swinging;                                    // +360
    bool m_swingPending;                                // +361 — delay countdown active
    char _pad362[2];                                    // +362
    float m_swingDelay;                                 // +364
    float m_swingDuration;                              // +368
    float m_swingElapsed;                               // +372
    int m_swingDirection;                               // +376
    Vector3 m_field380;                                 // +380 — previous swing point
    Vector3 m_field392;                                 // +392 — current swing point
    float m_baseLength;                                 // +404 — ctor 50.0
    float m_glowLength;                                 // +408 — ctor 65.0
    float m_glowIntensity;                              // +412 — ctor 0.0
    FloatColor m_glowColor;                             // +416 — ctor (0.5, 0.5, 0.5, 0)
    float m_field432;                                   // +432 — current length, lerp(base, glow, intensity) in SetGlowIntensity
    float m_width;                                      // +436 — ctor 10.0
    int m_field440;                                     // +440 — bounds-update counter (name UNRECOVERED)
    bool m_field444;                                    // +444 — impact/sound latch (name UNRECOVERED)
    char _pad445[3];                                    // +445 — sizeof 0x1C0
};

} // namespace Caver

#endif
