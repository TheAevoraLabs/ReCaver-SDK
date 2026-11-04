// Recovered from libswordigo.so (1.4.13) evidence — IDA/Hex-Rays exports
// for Caver::PhysicsObjectState (0x3779CC-0x37844C) plus the raw
// instruction stream. Every offset below is read or written by at least
// one of the 15 out-of-line functions; regions no function in this class
// touches are kept as named padding (UNRECOVERED semantics).
//
// The state is embedded by value inside EntityComponent and
// PhysicsObjectComponent (at +0x70 in both, per the desktop-port
// research notes); it has no vtable. Its first member points back at the
// owning SceneObject, whose transform the state drives directly:
// SceneObject velocity at +120, position at +128, rotation at +144,
// rotationSpeed at +148 and the world-bounds-update flag at +196.
// sizeof is 152 (0x98): the last field any evidence touches is the
// float at +148. The desktop-port notes quote a 0x9C embedded span in
// PhysicsObjectComponent (state at +0x70, next field at +0x10C); the
// extra 4 bytes are embedding tail padding (UNRECOVERED).
#ifndef CAVER_PHYSICSOBJECTSTATE_H
#define CAVER_PHYSICSOBJECTSTATE_H

#include <boost/intrusive_ptr.hpp>

#include "../core/Vector2.h"

namespace Caver {

class SceneObject;

// Collision message payload, laid out from
// PhysicsObjectState::HandleGroundCollision's accesses:
//   +0  object whose +408 float scales the rotation-speed transfer
//   +8  the other object's (derived) component: SceneObject* at +40,
//       bool at +418, float at +428, byte at +432
//   +24 Vector2 scaled by 1/60 in the entry gate
//   +32 Vector2 tested for stillness at the end
//   +40 Vector2 collision normal, +48 penetration depth
// Field names are UNRECOVERED; the type itself is only forward-used by
// other slices through this header.
struct CollisionMessageData {
    void* m_field0;         // +0
    void* m_field8;         // +8
    char _pad16[8];         // +16
    Vector2 m_field24;      // +24
    Vector2 m_field32;      // +32
    Vector2 m_normal;       // +40
    float m_depth;          // +48
    char _pad52[4];         // +52
};

class PhysicsObjectState {
public:
    PhysicsObjectState();
    ~PhysicsObjectState();

    Vector2 position() const;
    void setPosition(const Vector2& position);
    Vector2 velocity() const;
    void setVelocity(const Vector2& velocity);
    float rotation() const;
    void setRotation(float rotation);
    float rotationSpeed() const;
    void setRotationSpeed(float rotationSpeed);

    void Update(float deltaTime);
    void UpdateSpeedComponents(float deltaTime);
    void UpdateObjectState(float deltaTime);
    float AdjustGroundCollisionVector(Vector2* vector, float* magnitude);
    void HandleGroundCollision(const CollisionMessageData& data);

    SceneObject* m_object;                              // +0 — owning SceneObject (raw back-pointer)
    float m_field8;                                     // +8 — FLT_MAX in ctor; zeroed on ground contact, += dt in UpdateObjectState (name UNRECOVERED)
    float m_field12;                                    // +12 — FLT_MAX in ctor; zeroed on steep ground contact, += dt in UpdateObjectState (name UNRECOVERED)
    float m_groundNormalX;                              // +16 — 0 in ctor
    float m_groundNormalY;                              // +20 — 1 in ctor
    bool m_onSteepGround;                               // +24 — set to (normal.y < 0.7)
    unsigned char m_field25;                            // +25 — copied raw from the ground component's +432 byte
    char _pad26[6];                                     // +26
    boost::intrusive_ptr<SceneObject> m_groundObject;   // +32 — platform/ground object, velocity source in UpdateObjectState
    float m_field40X;                                   // +40 — platform velocity / accumulated extra velocity (Vector2 with +44)
    float m_field40Y;                                   // +44
    float m_field48;                                    // +48 — 1 in ctor; ramp factor (climbs to 1 at 0.5/s), reloaded from ground component +428
    float m_maxSpeed;                                   // +52 — 700 in ctor; UpdateObjectState speed clamp
    float m_field56;                                    // +56 — easing target for the +92 speed component
    float m_field60;                                    // +60 — easing rate, scaled by m_field48 and by 4 in the platform branch
    float m_field64;                                    // +64 — forced value for the +96 speed component when m_field68 is set
    bool m_field68;                                     // +68
    char _pad69[3];                                     // +69
    float m_field72;                                    // +72 — velocity damping coefficient in UpdateObjectState (0 in ctor)
    float m_frame76;                                    // +76 — surface frame matrix (76,80,84,88): (nx, ny, ny, -nx), identity (0,1,1,0) in ctor
    float m_frame80;                                    // +80
    float m_frame84;                                    // +84
    float m_frame88;                                    // +88
    float m_speedComponent92;                           // +92 — velocity projected on the surface frame, eased/clamped in UpdateSpeedComponents
    float m_speedComponent96;                           // +96
    bool m_field100;                                    // +100 — cleared when the owner moves; set (with +101) by the stillness check
    bool m_field101;                                    // +101 — suppresses gravity in UpdateSpeedComponents; consumed by UpdateObjectState
    char _pad102[2];                                    // +102
    float m_gravityDirectionX;                          // +104 — (0,-1) in ctor
    float m_gravityDirectionY;                          // +108
    float m_gravity;                                    // +112 — 800 in ctor
    bool m_enabled;                                     // +116 — 1 in ctor (stored as a WORD with +117)
    bool m_field117;                                    // +117 — enables the rotation-speed transfer in HandleGroundCollision
    char _pad118[2];                                    // +118
    float m_field120;                                   // +120 — negated/clamped as the bounce factor in HandleGroundCollision (name UNRECOVERED)
    bool m_field124;                                    // +124 — enables the +128 clamp in UpdateSpeedComponents
    char _pad125[3];                                    // +125
    float m_field128;                                   // +128 — clamp limit for the +92 component (not initialised by the ctor)
    char _pad132[16];                                   // +132 — UNRECOVERED (untouched by this class's functions)
    float m_field148;                                   // +148 — written by the embedding components' LoadFromProtobufMessage (1500 default in PhysicsObjectComponent)
};

} // namespace Caver

#endif
