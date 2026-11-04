// Recovered from libswordigo.so (1.4.13) evidence — vtable
// _ZTVN5Caver31MonsterDeathControllerComponentE at 0x605340.
//
// sizeof 0xC0 (192 bytes) is proven by:
//   operator new(0xC0u) in Create() @0x350C88 and Clone() @0x38DCD0.
//
// Primary vtable at 0x621790, IBindable secondary at 0x6218A8,
// IUpdateable secondary at 0x6218E8. These three addresses are
// written verbatim by the constructor and both destructor flavours.
//
// Field offsets confirmed by the constructor body @0x38C53C:
//
//   +0    vptr (primary)
//   +8    refcount
//   +16   vptr (IBindable secondary) — *((_QWORD*)this+2) = &off_6218A8
//   +24   vptr (IUpdateable secondary) — *((_QWORD*)this+3) = &off_6218E8
//   +32   int m_identifier (Component)
//   +36   int m_field36   (Component)
//   +40   SceneObject* m_object (Component)
//   +48   bool m_flag48   (Component)
//   +56   std::string m_label (Component)   — ctor reads *((_BYTE*)this+56)
//   +80   std::map m_outlets (Component)    — confirmed by ComponentOutletBinding sentinel at +88
//   +104  ComponentOutlet<ParticleEmitterComponent> m_particleEmitter
//         BindOutlet(id=0, "particleEmitter", this+104, true) proven by the ctor
//   +120  ParticleEmitterComponent* m_resolvedEmitter — *((_QWORD*)this+15)
//         read in Explode, cleared in destructor and LoadFromProtobufMessage
//   +128  float m_localOriginX   — *(float*)(this+128), written by FindOriginAndRadius
//   +132  float m_localOriginY   — *(float*)(this+132)
//   +136  float m_localOriginZ   — *(float*)(this+136)  (depth layer)
//   +140  float m_sliceCenterX   — *(float*)(this+140), collision-shape-local center
//   +144  float m_sliceCenterY   — *(float*)(this+144)
//   +148  float m_worldOriginX   — *(float*)(this+148), world-space origin
//   +152  float m_worldOriginY   — *(float*)(this+152)
//   +156  float m_worldOriginZ   — *(float*)(this+156)  (depth)
//   +160  float m_radius         — *(float*)(this+160), 0.45 * collisionShape.halfHeight * sceneObject.scale
//   +164  float m_elapsedTime    — *(float*)(this+164), accumulates in Update
//   +168  float m_dissolveAlpha  — *(float*)(this+168), written in Update (float 1060320051 = 0.5f)
//   +172  char  _pad172[4]       — explicit 0 in ctor: *((_BYTE*)this+172)=0
//   +176  SceneObject* m_killer  — *((_QWORD*)this+22), stored by KillMonster.
//         NOTE: the name is a misnomer kept for layout stability. Golden
//         HandleDamageImpact (0x374D90) passes the VICTIM monster as the
//         stored object, and SliceMonster/KillMonster slice and shatter
//         that victim's model; it is not the attacker or the sword.
//   +184  int   m_state          — *((_DWORD*)this+46), death FSM state:
//                                    0 = idle/tracking, 1 = exploding, 2 = dissolving
//   +188  int   m_particleEmitterId — *((_DWORD*)this+47)  == *((_DWORD*)this+28)*4? No:
//         proto field: *((_DWORD*)this+28) = extension field 1 (particle_emitter_id)
//         Clone copies *((_DWORD*)this+8) (=identifier at +32) and *((_DWORD*)this+28).
//         Wait — *((_DWORD*)this+28) is byte offset 112 which is inside the
//         ComponentOutlet at +104 (size 24: vptr 8 + m_identifier 4 + m_target 8 + pad).
//         Re-examine: *((_DWORD*)this+28) = offset 28*4 = 112 = outlet.m_identifier (int at +112)?
//         No: ComponentOutletBase has vptr(8) + m_identifier(4) = 12 bytes, so outlet at +104:
//           +104 vptr, +112 m_identifier (int), +116 pad, +120 m_target (ptr).
//         So *((_DWORD*)this+28) = *(int*)(this+112) = m_particleEmitter.m_identifier.
//         Confirmed: LoadFromProtobufMessage writes *((_DWORD*)this+28) from proto field 1.
//         SaveToProtobufMessage reads *((_DWORD*)this+28) into proto field.
//         Clone() copies *((_DWORD*)this+28) after constructing.
//
// Struct layout summary (192 bytes total):
//   0x00  Component base (104 bytes: vptr + RCO + IBindable vptr + IUpdateable vptr +
//         m_identifier + m_field36 + m_object + m_flag48 + m_label + m_outlets)
//   0x68  ComponentOutlet<ParticleEmitterComponent> m_particleEmitter  (24 bytes)
//   0x80  ParticleEmitterComponent* m_resolvedEmitter                  ( 8 bytes)
//   0x80  float m_localOriginX                                         ( 4 bytes, offset 128)
//   (... see full offset table above)
//   0xB8  SceneObject* m_killer                                        ( 8 bytes)
//   0xB8? int m_state
//   The exact tail must fit 192 bytes.
//
// Corrected from raw ctor evidence:
//   *((_DWORD*)this+2)  = 0   => offset 8  = refcount (Component's RCO base)
//   *((_QWORD*)this+7)  = 0   => offset 56 = m_label._M_dataplus._M_p (string internal)
//   *((_QWORD*)this+8)  = 0   => offset 64 = string continuation
//   *((_QWORD*)this+12) = 0   => offset 96 = map begin sentinel
//   *((_QWORD*)this+11) = 0   => offset 88 = map root
//   *((_QWORD*)this+4)  = 0   => offset 32 = m_identifier
//   *((_QWORD*)this+5)  = 0   => offset 40 = m_object
//   *((_BYTE*)this+48)  = 0   => m_flag48
//   *((_QWORD*)this+9)  = 0   => offset 72 = map size
//   *((_QWORD*)this+10) = (char*)this+88 => sentinel self-pointer in map
//   *((_DWORD*)this+28) = 0   => offset 112 = m_particleEmitter.m_identifier
//   *((_QWORD*)this+15) = 0   => offset 120 = m_resolvedEmitter
//   *((_QWORD*)this+13) = off_61FAB0 => offset 104 = m_particleEmitter vptr
//   *((_DWORD*)this+41) = 0   => offset 164 = m_elapsedTime
//   xmmword at +148 from xmmword_259E70 => m_worldOriginX/Y/Z + m_radius (16 bytes)
//   *((_BYTE*)this+172) = 0   => _pad172[0]
//   *((_QWORD*)this+22) = 0   => offset 176 = m_killer
//   *((_DWORD*)this+46) = 0   => offset 184 = m_state
//
#ifndef CAVER_MONSTERDEATHCONTROLLERCOMPONENT_H
#define CAVER_MONSTERDEATHCONTROLLERCOMPONENT_H

#include <string>
#include <boost/intrusive_ptr.hpp>
#include <boost/shared_ptr.hpp>

#include "Component.h"
#include "ComponentOutlet.h"
#include "ModelComponent.h"
#include "ParticleEmitterComponent.h"

namespace Caver {

class CollisionShapeComponent;
class SceneObject;
class Vector2;
class Vector3;
struct Vector4;

// Forward-declared here only; the full definition lives in the .cpp
// translation unit that needs it. This component does not expose a
// public Model or MeshClipper type in the header.
class Model;

// Death dissolution controller. Attached to a monster SceneObject; on
// KillMonster() it slices the monster's mesh along a plane derived from
// the killing weapon's world position, launches the two slice fragments
// as independent physics objects, and dissolves the particle emitter.
//
// Primary vtable: _ZTVN5Caver31MonsterDeathControllerComponentE @0x605340
// IBindable secondary:    @0x6218A8
// IUpdateable secondary:  @0x6218E8
class MonsterDeathControllerComponent : public Component {
public:
    MonsterDeathControllerComponent();
    virtual ~MonsterDeathControllerComponent();

    // Factory (operator new(0xC0) + ctor, proven by Create() @0x350C88).
    static MonsterDeathControllerComponent* Create();
    // Interface token (self-referential function pointer, @0x376270).
    static long Interface();

    // vtable slot 2 — proven by Clone() @0x38DCD0.
    virtual MonsterDeathControllerComponent* Clone() const;
    // Non-virtual overload; vtable slot 3 keeps Component's base.
    void InitWithComponent(const MonsterDeathControllerComponent* other);

    virtual void LoadFromProtobufMessage(const Proto::Component& message);       // slot 4
    virtual void SaveToProtobufMessage(Proto::Component* message) const;         // slot 5
    virtual bool ShouldSave();                                                   // slot 6 — returns true
    virtual void Prepare();                                                      // slot 8 — empty body

    // IBindable slots: the golden does override these five, each with a
    // body that is a single tail branch into Component (0x38C7E4,
    // 0x38C7F0, 0x38C7FC, 0x38C808) except PerformBindedAction, which is
    // empty (0x38C814).
    virtual void GetBindings(std::vector<Binding>* bindings);
    virtual void GetEnumValuesForBindedProperty(int property,
                                                std::vector<BindingValue>* values);
    virtual BindingValue ValueForBindedProperty(int property);
    virtual void SetValueForBindedProperty(int property,
                                           const BindingValue& value);
    virtual void PerformBindedAction(int action);

    virtual bool ImplementsInterface(long interfaceId);                          // slot 20
    virtual void RegisterInterfaces(ComponentManager* manager);                  // slot 21
    virtual bool RequiresUpdate() const;                                         // slot 23 — returns true
    virtual void Update(float deltaTime);                                        // slot 25
    virtual bool HandleMessage(int message, void* data);                         // slot 26 — returns false
    virtual std::string className() const;                                       // slot 32

    // Public actions.
    void KillMonster(const boost::intrusive_ptr<SceneObject>& killer);
    void SliceMonster(const boost::intrusive_ptr<SceneObject>& victim,
                      const Vector4& plane);
    void Explode();

private:
    // Internal helpers.
    void FindOriginAndRadius();
    SceneObject* CreateSliceObject(const boost::shared_ptr<Model>& model,
                                   const Vector3& velocity);

    // --- member layout (offsets from binary evidence) ---

    // +104  ComponentOutlet<ParticleEmitterComponent> m_particleEmitter
    //        BindOutlet id=0, name="particleEmitter", flag=true (ctor).
    //        sizeof(ComponentOutlet<T>) = 8 (vptr) + 4 (m_identifier) + 4 (pad) + 8 (m_target) = 24.
    ComponentOutlet<ParticleEmitterComponent> m_particleEmitter;    // +104

    // +120  (8 bytes padding / tail of outlet) — actually m_particleEmitter.m_target
    //       is the last field of the outlet above; resolved cache sits at +120 in
    //       the binary (*((_QWORD*)this+15)). This is the raw resolved pointer
    //       written to by ComponentOutletBase::Connect after outlet lookup.
    //       It is NOT a separate member; the layout is:
    //         ComponentOutlet<ParticleEmitterComponent> m_particleEmitter  @ +104 (24 bytes)
    //           +104: vptr
    //           +112: m_identifier  (== *((_DWORD*)this+28))
    //           +116: pad
    //           +120: m_target (ParticleEmitterComponent*)  (== *((_QWORD*)this+15))
    // So there is no explicit m_resolvedEmitter field; it lives inside the outlet.

    // +128  float m_localOriginX
    // +132  float m_localOriginY
    // +136  float m_localOriginZ  (depth of the owner SceneObject)
    float m_localOriginX;   // +128
    float m_localOriginY;   // +132
    float m_localOriginZ;   // +136

    // +140  float m_sliceCenterX  (collision-shape-local centre, used for radius)
    // +144  float m_sliceCenterY
    float m_sliceCenterX;   // +140
    float m_sliceCenterY;   // +144

    // +148  float m_worldOriginX  (world-space origin of the monster's hit point)
    // +152  float m_worldOriginY
    // +156  float m_worldOriginZ  (depth of slice scene object)
    // +160  float m_radius        (0.45 * halfHeight * scale, computed by FindOriginAndRadius)
    // These four are initialised as an xmmword from xmmword_259E70 (all zeros)
    // in the ctor, then overwritten by FindOriginAndRadius.
    float m_worldOriginX;   // +148
    float m_worldOriginY;   // +152
    float m_worldOriginZ;   // +156
    float m_radius;         // +160

    // +164  float m_elapsedTime   (accumulated dt; written as *((_DWORD*)this+41) = 0 in ctor)
    float m_elapsedTime;    // +164

    // +168  float m_dissolveAlpha (written in Update state-1: 1060320051 = 0.5f approx)
    float m_dissolveAlpha;  // +168

    // +172  char _pad172[4]       (*((_BYTE*)this+172) = 0 in ctor)
    char _pad172[4];        // +172

    // +176  SceneObject* m_killer (*((_QWORD*)this+22) = 0 in ctor; set by KillMonster)
    SceneObject* m_killer;  // +176

    // +184  int m_state           (*((_DWORD*)this+46) = 0 in ctor)
    //         0 = idle (tracking killer position), 1 = exploding, 2 = dissolving
    int m_state;            // +184

    // +188  char _pad188[4]       (pad to 192 = 0xC0)
    char _pad188[4];        // +188
};

static_assert(sizeof(MonsterDeathControllerComponent) == 0xC0,
              "MonsterDeathControllerComponent sizeof must be 192 bytes");

} // namespace Caver

#endif // CAVER_MONSTERDEATHCONTROLLERCOMPONENT_H
