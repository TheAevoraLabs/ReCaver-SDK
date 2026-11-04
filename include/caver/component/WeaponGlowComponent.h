// Recovered from libswordigo.so (1.4.13) evidence — see the combat
// slice board (~/workspace/.scratch/parity/combat/WeaponGlowComponent/
// RESULTS.md) for per-function parity status.
//
// Weapon glow component: drives a ParticleEmitterComponent through an
// outlet and draws a glow ribbon over the owner's weapon.
// sizeof 0xF0 (240) — proven by Create()'s operator new(0xF0).
// Member offsets are proven from the ctor, InitWithComponent,
// LoadFromProtobufMessage and the destructor bodies.
#ifndef CAVER_WEAPONGLOWCOMPONENT_H
#define CAVER_WEAPONGLOWCOMPONENT_H

#include "Component.h"
#include "GlowComponent.h"
#include "ComponentOutlet.h"
#include "../core/FloatColor.h"
#include "../core/Color.h"
#include <boost/intrusive_ptr.hpp>

#include "ParticleEmitterComponent.h"

namespace Caver {

class TransformComponent;
class ModelComponent;
class RenderingContext;
class Matrix4;
class Vector3;

typedef ComponentOutlet<ParticleEmitterComponent> WeaponGlowParticleEmitterOutlet;

// Owned array holder for the vertex/index buffers: CreateVertexData's
// golden sequence is store-new-then-delete-old and ~WeaponGlowComponent
// delete[]s them in member-teardown position — the same holder shape
// the modelmesh slice proved for Mesh's buffers (MeshOwnedArray).
// Original type name UNRECOVERED.
template <typename T>
struct WeaponGlowBuffer {
    T* m_ptr;
    WeaponGlowBuffer() : m_ptr(0) {}
    ~WeaponGlowBuffer() { delete[] m_ptr; }
    void Reset(T* ptr)
    {
        T* old = m_ptr;
        m_ptr = ptr;
        if (old)
            delete[] old;
    }
    T* Get() const { return m_ptr; }
private:
    WeaponGlowBuffer(const WeaponGlowBuffer&);
    WeaponGlowBuffer& operator=(const WeaponGlowBuffer&);
};

class WeaponGlowComponent : public GlowComponent {
public:
    WeaponGlowComponent();
    virtual ~WeaponGlowComponent();

    static WeaponGlowComponent* Create();
    static long Interface();

    virtual Component* Clone() const;
    // NOT an override: narrowed non-virtual overload (EPKS0_).
    void InitWithComponent(const WeaponGlowComponent* other);
    virtual void LoadFromProtobufMessage(const Proto::Component& message);
    virtual void SaveToProtobufMessage(Proto::Component* message) const;
    virtual bool ShouldSave();                                              // true
    virtual void GetBindings(std::vector<Binding>* bindings);
    virtual void GetEnumValuesForBindedProperty(int property, std::vector<BindingValue>* values);
    virtual BindingValue ValueForBindedProperty(int property);
    virtual void SetValueForBindedProperty(int property, const BindingValue& value);
    virtual void PerformBindedAction(int action);
    virtual void Prepare();
    virtual bool ImplementsInterface(long interfaceId);
    virtual void RegisterInterfaces(ComponentManager* manager);
    virtual bool ShouldBeUpdatedInEditor() const;                           // true
    virtual bool RequiresUpdate() const;                                    // true
    virtual void Update(float deltaTime);
    virtual std::string className() const;                                  // "WeaponGlow"

    void UpdateParticleEmitter();
    void SetColor(const FloatColor& color);
    void Draw(RenderingContext* context, const Matrix4& matrix, const Vector3& position, const Vector3& rotation);
    void CreateVertexData();
    float CreateVertexData(int scale, int vertexOffset, int faceOffset, int count, float* positions, float* widths);
    void SetFace(int face, int vertex0, int vertex1, int vertex2);

    WeaponGlowParticleEmitterOutlet m_particleEmitter;  // +128 — ComponentOutlet<ParticleEmitterComponent>: identifier @+136 (the proto particle_emitter_id), target @+144
    bool m_field152;                // +152 — set by SetColor / SetValueForBindedProperty(103); consumed and cleared by Update (name UNRECOVERED)
    FloatColor m_color;             // +156 — ctor default (0.5, 0.5, 0.5, 0.0), proven from the .rodata constant at 0x25A260
    Color m_particleColor;          // +172 — ctor default 0x00FFFFFF (r/g/b 255, a 0)
    WeaponGlowBuffer<float> m_vertexData;       // +176 — 17 vertices x 16 bytes in CreateVertexData()
    int m_field184;                 // +184 — set to 17 by CreateVertexData() (name UNRECOVERED)
    WeaponGlowBuffer<unsigned char> m_indexData; // +192 — 21 faces x 3 bytes in CreateVertexData()
    int m_field200;                 // +200 — set to 21 by CreateVertexData(); Draw draws 3 * m_field200 indices (name UNRECOVERED)
    boost::intrusive_ptr<TransformComponent> m_transformComponent;  // +208 — cached by Prepare
    boost::intrusive_ptr<ModelComponent> m_modelComponent;          // +216 — cached by Prepare
    int m_field224;                 // +224 — 0 in ctor; never read by any recovered body (name UNRECOVERED)
    float m_field228;               // +228 — 1.0f in ctor; scales m_color in UpdateParticleEmitter / CreateVertexData (name UNRECOVERED)
    float m_field232;               // +232 — 65.0f in ctor (name UNRECOVERED)
    float m_width;                  // +236 — 15.0f in ctor; the proto extension's width (field 4), binding property 103
};

} // namespace Caver

#endif
