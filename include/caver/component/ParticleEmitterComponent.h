// Recovered from libswordigo.so (1.4.13) evidence — Caver::ParticleEmitterComponent,
// the scene-side particle emitter component. sizeof 0x1D0 (464), proven by
// Create()'s operator new(0x1D0). It wraps the render-side
// Caver::ParticleSystem (owned, created in the ctor) and a
// Caver::ParticleEmitter (shared_ptr, set via bindings / SetEmitter).
//
// Layout (offsets proven from the ctor 0x396520, the destructor 0x396AC4,
// InitWithComponent 0x396D7C, Load/Save 0x396F64 / 0x3971F4, Prepare
// 0x399264, Update 0x399798 and Draw 0x399FB0):
//   +104 int m_particleTypeCount — number of live particle-type outlets
//        (Load sets it from the proto particle_id size; SetValue
//        recounts it by compacting the outlet identifiers)
//   +112 ComponentOutlet<ParticleComponent> m_particleOutlets[5]
//        (bound in the ctor with ids 7..11, names "particleType1".."5")
//   +232 .. +251: 20 bytes never written or read by any function of
//        this class in the golden binary (UNRECOVERED — possibly a
//        reserved sixth outlet slot that was never constructed)
//   +252 bool m_prepared — set at the end of Prepare(); SetEmitter and
//        SetValueForBindedProperty(12) consult it before PrepareEmitter
//   +256 ComponentOutlet<ParticleEmitterComponent> m_parentEmitterOutlet
//        (ctor BindOutlet id 0, "parentEmitter")
//   +280 ComponentOutlet<ModelComponent> m_modelOutlet
//        (ctor BindOutlet id 2, "modelBinding")
//   +304 boost::intrusive_ptr<TransformComponent> m_transform — cached
//        by Prepare() via ComponentWithInterface
//   +312 bool m_localSystem (proto field 8)
//   +316 int m_maxParticles (ctor: 30; proto field 4)
//   +320 boost::shared_ptr<ParticleSystem> m_particleSystem
//   +336 boost::shared_ptr<ParticleEmitter> m_emitter
//   +352 bool m_emitting (ctor: true)
//   +353 bool m_destroyWhenFinished (proto field 6)
//   +356 Vector3 m_gravity (proto field 9; ctor (0,0,0))
//   +368 FloatColor m_color (ctor (1,1,1,1); pushed onto the emitter's
//        VaryingColor by SetColor / binding property 14)
//   +384 Vector3 m_rotation (proto field 10; radians; ctor (0,0,0))
//   +396 Matrix4 m_localMatrix (built by UpdateLocalTransform; NOT
//        initialised by the ctor in the golden binary)
//   +460 bool m_useLocalMatrix (set by UpdateLocalTransform when any
//        rotation component exceeds 0.001 in magnitude)
//
// The primary vtable (0x622BC0) matches the Component slot map with
// InitWithComponent's slot holding the BASE function: this class's
// InitWithComponent(const ParticleEmitterComponent*) is a non-virtual
// overload (same pattern as HealthComponent).
#ifndef CAVER_PARTICLEEMITTERCOMPONENT_H
#define CAVER_PARTICLEEMITTERCOMPONENT_H

#include "Component.h"
#include "ComponentOutlet.h"
#include "ParticleComponent.h"
#include "ModelComponent.h"
#include "../core/Vector3.h"
#include "../core/FloatColor.h"
#include "../core/Matrix4.h"
#include "../animation/ParticleSystem.h"
#include "../animation/ParticleEmitter.h"
#include <boost/shared_ptr.hpp>
#include <boost/intrusive_ptr.hpp>

#include "TransformComponent.h"

namespace Caver {

class RenderingContext;

class ParticleEmitterComponent : public Component {
public:
    ParticleEmitterComponent();
    virtual ~ParticleEmitterComponent();

    static ParticleEmitterComponent* Create();
    static long Interface();

    virtual Component* Clone() const;
    // NOT an override: the vtable slot keeps Component::InitWithComponent.
    void InitWithComponent(const ParticleEmitterComponent* other);
    virtual void LoadFromProtobufMessage(const Proto::Component& message);
    virtual void SaveToProtobufMessage(Proto::Component* message) const;
    virtual bool ShouldSave();                                              // true
    virtual void GetBindings(std::vector<Binding>* bindings);
    virtual void GetEnumValuesForBindedProperty(int property, std::vector<BindingValue>* values);
    virtual BindingValue ValueForBindedProperty(int property);
    virtual void SetValueForBindedProperty(int property, const BindingValue& value);
    virtual void PerformBindedAction(int action);                           // empty
    virtual bool ImplementsInterface(long interfaceId);
    virtual void RegisterInterfaces(ComponentManager* manager);
    virtual bool ShouldBeUpdatedInEditor() const;                           // true
    virtual bool RequiresUpdate() const;                                    // true
    virtual bool IsVisualComponent() const;                                 // true
    virtual std::string className() const;                                  // "ParticleEmitter"

    virtual void Prepare();
    virtual void Update(float deltaTime);
    void RegisterLibrary();
    void Draw(RenderingContext* context, const Matrix4& matrix,
              const Vector3& cameraPosition, const Vector3& cameraUp);

    void UpdateLocalTransform();
    void SetEmitter(const boost::shared_ptr<ParticleEmitter>& emitter);
    void SetColor(const FloatColor& color);
    void PrepareEmitter(const boost::shared_ptr<ParticleEmitter>& emitter);
    Particle* AddParticle(int type, const Vector3& position,
                          const Vector3& velocity, float rotation,
                          const Color& color, float life);
    Particle* AddParticle(const Particle& particle);
    int RandomParticleType();
    void StartEmitting();
    void StopEmitting();

    int m_particleTypeCount;                                    // +104
    ComponentOutlet<ParticleComponent> m_particleOutlets[5];    // +112
    char _pad232[20];                                           // +232 — never touched (UNRECOVERED)
    bool m_prepared;                                            // +252
    char _pad253[3];                                            // +253
    ComponentOutlet<ParticleEmitterComponent> m_parentEmitterOutlet; // +256
    ComponentOutlet<ModelComponent> m_modelOutlet;              // +280
    boost::intrusive_ptr<TransformComponent> m_transform;       // +304
    bool m_localSystem;                                         // +312
    char _pad313[3];                                            // +313
    int m_maxParticles;                                         // +316
    boost::shared_ptr<ParticleSystem> m_particleSystem;         // +320
    boost::shared_ptr<ParticleEmitter> m_emitter;               // +336
    bool m_emitting;                                            // +352
    bool m_destroyWhenFinished;                                 // +353
    char _pad354[2];                                            // +354
    Vector3 m_gravity;                                          // +356
    FloatColor m_color;                                         // +368
    Vector3 m_rotation;                                         // +384
    Matrix4 m_localMatrix;                                      // +396
    bool m_useLocalMatrix;                                      // +460
    char _pad461[3];                                            // +461 — sizeof 0x1D0
};

} // namespace Caver

#endif
