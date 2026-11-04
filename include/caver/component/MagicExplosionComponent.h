// Recovered from libswordigo.so (1.4.13) evidence — march 2, combat
// lane. Magic explosion component; sizeof 0x180 (384), proven by
// Create()'s operator new(0x180). Member offsets are proven from the
// ctor (0x37CCAC), InitWithComponent, Load/SaveToProtobufMessage,
// SetDamage, Update, Draw and the destructor bodies.
//
// Layout (Component base ends at +104):
//   +104 ComponentOutlet<ParticleEmitterComponent> — ctor BindOutlet
//        (0, "particleEmitter"); its identifier (+112) is the proto
//        extension's particle_emitter_id and its target (+120) is the
//        resolved emitter component (Load clears it via the outlet's
//        setIdentifier semantics when the id changes).
//   +128 ComponentOutlet<SoundEffectComponent> — ctor BindOutlet
//        (4, "sound"); identifier (+136) = proto sound_id, target
//        (+144) is played by Update once the explosion starts.
//   +152 intrusive_ptr<DamageComponent> — created by SetDamage.
//   +160 intrusive_ptr<CollisionShapeComponent> — created by
//        SetDamage (circle shape, radius 150, initially disabled).
//   +168 FloatColor color (ctor (1,1,1,1); proto color, binding 1).
//   +184 FloatColor — read by Draw as a component-wise multiplier of
//        +168; never written by any function of this class in the
//        golden binary and NOT initialised by the ctor (UNRECOVERED).
//   +200 Vector2 direction (set by ExplodeToDirection; Update's
//        starting branch resets it to (1, 0)).
//   +208 float — radius / duration ratio, computed by
//        ExplodeToDirection and by Update's starting branch.
//   +212 float — ExplodeToDirection's second argument; Update's
//        starting branch resets it to 2*pi (UNRECOVERED name).
//   +216 float radius (ctor 100; proto radius, binding 2).
//   +220 float duration (ctor 0.3; proto duration, binding 3).
//   +224 float — radius * 1.5 (the expansion target; ctor 150).
//   +228 float — elapsed timer while active.
//   +232 bool — shockwave phase flag (cleared once the timer
//        passes 0.5).
//   +233 bool — started flag (shape enabled / sound played / blast
//        object spawned exactly once).
//   +234 bool — active (exploding) flag.
//   +236/+240 float — first star polygon rotation angle / scale.
//   +244/+248 float — second star polygon rotation angle / scale.
//   +256 Sprite (88 bytes, embedded) — the "$particle_explosion"
//        sprite, initialised by Prepare.
//   +344 float — blast growth progress (sprite scale in Draw; when
//        it passes the expansion target the owner is flagged +248).
//   +352 StarVertex* — star polygon vertices (8 x 16 bytes,
//        operator new[](0x80) in CreateStarPolygon).
//   +360 unsigned char* — star polygon indices (operator
//        new[](0x12) = 18 indices in CreateStarPolygon).
//   +368 int — star polygon triangle count (6 after creation).
//   +376 SceneObject* caster (raw pointer, manually reference
//        counted exactly as the golden SetCasterObject /
//        CasterObject / destructor bodies do it).
//
// The Proto extension field number is 551 (proven from the golden
// protobuf static initialiser: "MagicExplosionComponent::extension
// = 551" in protobuf_AddDesc_Scene's TU init, IDA dump line
// 290004; HealthComponent's was 123, GroundMeshComponent's 111).
#ifndef CAVER_MAGICEXPLOSIONCOMPONENT_H
#define CAVER_MAGICEXPLOSIONCOMPONENT_H

#include "Component.h"
#include "ComponentOutlet.h"
#include "ParticleEmitterComponent.h"
#include "DamageComponent.h"
#include "CollisionShapeComponent.h"
#include "SoundEffectComponent.h"
#include "../core/Vector2.h"
#include "../core/FloatColor.h"
#include "../render/Sprite.h"

namespace Caver {

class RenderingContext;

// One star-polygon vertex (16 bytes): position + packed colour at
// +12, the layout Draw's vertex attribute pointers prove (position:
// 3 floats stride 16 at +0; colour: 4 unsigned bytes stride 16 at
// +12) and CreateStarPolygon's store map fills.
struct StarVertex {
    float x;            // +0
    float y;            // +4
    float z;            // +8
    unsigned int color; // +12
};

// The embedded sprite at +256. The golden ~Sprite teardown deletes
// the heap arrays at sprite +56 (m_vertexBuffer) and +32
// (m_indexBuffer), in that order, before the texture pointer at +0
// is released. That teardown lives in Sprite::~Sprite and runs
// automatically as the base destructor: it must NOT be repeated
// here. A previous version of this struct deleted +56/+32 in its
// own destructor body, so every MagicExplosionComponent destruction
// freed both buffers twice (GDB-proven 2026-10-29: PurgeInactive
// erase -> ~MagicExplosionComponent -> ~Sprite freeing an
// already-freed m_vertexBuffer, "double free detected in tcache 2").
// This struct therefore declares no destructor body of its own.
struct MagicExplosionSprite : public Sprite {
};

class MagicExplosionComponent : public Component {
public:
    MagicExplosionComponent();
    virtual ~MagicExplosionComponent();

    static MagicExplosionComponent* Create();
    static long Interface();

    virtual Component* Clone() const;
    // NOT an override: the vtable slot 3 holds
    // Component::InitWithComponent. This is a non-virtual overload
    // taking the narrowed type (mangling EPKS0_).
    void InitWithComponent(const MagicExplosionComponent* other);
    virtual void LoadFromProtobufMessage(const Proto::Component& message);
    virtual void SaveToProtobufMessage(Proto::Component* message) const;
    virtual bool ShouldSave();                                              // true
    virtual void GetBindings(std::vector<Binding>* bindings);
    virtual void GetEnumValuesForBindedProperty(int property, std::vector<BindingValue>* values);
    virtual BindingValue ValueForBindedProperty(int property);
    virtual void SetValueForBindedProperty(int property, const BindingValue& value);
    virtual void PerformBindedAction(int action);                           // empty
    virtual void Prepare();
    virtual bool ImplementsInterface(long interfaceId);
    virtual void RegisterInterfaces(ComponentManager* manager);
    virtual bool RequiresUpdate() const;                                    // true
    virtual std::string className() const;                                  // "MagicExplosion" (golden, sic — not the full class name)

    virtual void Update(float deltaTime);
    virtual bool HandleMessage(int message, void* data);
    void Draw(RenderingContext* context, const Matrix4& matrix) const;
    void Draw(RenderingContext* context, const Matrix4& matrix);

    void CreateStarPolygon();
    void UpdateStarPolygonColor();
    void ExplodeToDirection(const Vector2& direction, float value);
    void SetDamage(DamageType damageType, SpecialDamageType specialDamageType, int damage, float radius);
    void SetCasterObject(const boost::intrusive_ptr<SceneObject>& object);
    boost::intrusive_ptr<SceneObject> CasterObject();

    ComponentOutlet<ParticleEmitterComponent> m_particleEmitterOutlet; // +104 (id +112, target +120)
    ComponentOutlet<SoundEffectComponent> m_soundOutlet;               // +128 (id +136, target +144)
    boost::intrusive_ptr<DamageComponent> m_damageComponent;           // +152
    boost::intrusive_ptr<CollisionShapeComponent> m_collisionComponent; // +160
    FloatColor m_color;             // +168 — ctor (1,1,1,1)
    FloatColor m_field184;          // +184 — UNRECOVERED (see top note)
    Vector2 m_direction;            // +200
    float m_field208;               // +208 — radius / duration
    float m_field212;               // +212 — UNRECOVERED (2*pi default in Update)
    float m_radius;                 // +216 — ctor 100
    float m_duration;               // +220 — ctor 0.3
    float m_maxRadius;              // +224 — radius * 1.5
    float m_elapsed;                // +228
    bool m_shockwavePhase;          // +232
    bool m_started;                 // +233
    bool m_active;                  // +234
    char _pad235;                   // +235
    float m_starAngle1;             // +236
    float m_starScale1;             // +240
    float m_starAngle2;             // +244
    float m_starScale2;             // +248
    char _pad252[4];                // +252
    MagicExplosionSprite m_sprite;  // +256 (88 bytes)
    float m_blastProgress;          // +344
    char _pad348[4];                // +348
    StarVertex* m_starVertices;     // +352
    unsigned char* m_starIndices;   // +360
    int m_starTriangleCount;        // +368
    char _pad372[4];                // +372
    SceneObject* m_casterObject;    // +376 — manually refcounted (see top note)
};

} // namespace Caver

#endif
