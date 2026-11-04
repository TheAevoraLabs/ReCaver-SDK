// Recovered from libswordigo.so (1.4.13) evidence — march 2 (world-
// components lane). Supersedes the core slice's stub header.
//
// Caver::CollisionShapeComponent : Caver::ShapeComponent :
// Caver::Component. The ShapeComponent base is not yet in the tree;
// this header derives from Component directly and declares the
// ShapeComponent-added virtuals in their golden vtable order, so the
// primary vtable matches the golden _ZTV at 0x619028 (416 bytes):
// slots 0..32 = Component's, 33 EditColor, 34 Draw, 35 SetShape,
// 36 SetDefaultShapeOfType, 37 UpdateCollisionShape (new here).
// Golden slot 3 (InitWithComponent) holds the BASE Component function:
// this class's InitWithComponent(const CollisionShapeComponent*) is a
// non-virtual overload, exactly as with HealthComponent. Golden slot
// 18 (PerformBindedAction) and slot 19 (componentCategories, returns
// 4) hold ShapeComponent functions — they land here only when the
// real ShapeComponent header does (hand-off, see results file).
//
// Layout (sizeof 504 = 0x1F8, proven by Create()/Clone()'s
// operator new(0x1F8) and the ctor/dtor store maps):
//   +104  Shape m_shape            (the ShapeComponent member)
//   +136  Program m_onCollide      (proto on_collide, field 9)
//   +192  Program m_onCollisionEnd (proto on_collision_end, field 10)
//   +248  Program m_onReceiveDamage(proto on_receive_damage, field 12)
//   +304  CollisionShape m_collisionShape (by value, 72 bytes)
//   +376  float m_minDepth (-15 in ctor) / +380 float m_maxDepth (+15)
//   +384  Rectangle m_bounds       (local bounds; localAABB() returns it)
//   +400  Rectangle m_worldAABB    (world AABB; Scene reads it here)
//   +416  bool m_enabled (true in ctor; proto enabled, field 11)
//   +417  bool m_field417 (UNRECOVERED name; when set, SetShape /
//         UpdateBounds skip the bounds recompute)
//   +418  bool m_isGround / +419 bool m_collides /
//         +420 bool m_receivesDamage / +421 bool m_inflictsDamage
//   +424  int m_specialType (editor enum: 0 none, 1 pickup,
//         2 pushable, 3 use, 4 portal, 5 collectable, 6 blocksDamage,
//         7 grabbable; the proto enum is a permutation of this —
//         the Load/Save tables in the .cpp are proven from .rodata)
//   +428  float m_friction (1.0 in ctor; proto friction, field 13)
//   +432  bool m_unsafeGround (proto unsafe_ground, field 14)
//   +440  SceneGridEntry storage (48 bytes; owner pointer at +448)
//   +488  bool m_field488 (UNRECOVERED; zeroed by the ctor, no other
//         in-class reader found)
//   +492  Vector2 m_field492 (UNRECOVERED semantics; copied into
//         CollisionShape +60/+64 by UpdateCollisionShape)
//   +500  bool m_field500 (UNRECOVERED name; Prepare sets it to
//         "a GroundMeshComponent sibling exists" when m_isGround)
#ifndef CAVER_COLLISIONSHAPECOMPONENT_H
#define CAVER_COLLISIONSHAPECOMPONENT_H

#include "Component.h"
#include "../core/Color.h"
#include "../core/Vector2.h"
#include "../physics/CollisionShape.h"
#include "../scripting/Program.h"

namespace Caver {

class RenderingContext;
struct Matrix4;

class CollisionShapeComponent : public Component {
public:
    CollisionShapeComponent();
    virtual ~CollisionShapeComponent();

    static CollisionShapeComponent* Create();
    static long Interface();
    static bool drawDepth; // golden BSS _ZN5Caver23CollisionShapeComponent9drawDepthE

    virtual CollisionShapeComponent* Clone() const;                 // slot 2
    // Non-virtual overload (slot 3 keeps Component::InitWithComponent):
    void InitWithComponent(const CollisionShapeComponent* other);
    virtual void LoadFromProtobufMessage(const Proto::Component& message); // slot 4
    virtual void SaveToProtobufMessage(Proto::Component* message) const;   // slot 5
    virtual bool ShouldSave();                                      // slot 6 (true)
    virtual void Prepare();                                         // slot 8
    virtual void GetBindings(std::vector<Binding>* bindings);       // slot 14
    virtual void GetEnumValuesForBindedProperty(int property, std::vector<BindingValue>* values); // slot 15
    virtual BindingValue ValueForBindedProperty(int property);      // slot 16
    virtual void SetValueForBindedProperty(int property, const BindingValue& value); // slot 17
    virtual bool ImplementsInterface(long interfaceId);             // slot 20
    virtual void RegisterInterfaces(ComponentManager* manager);    // slot 21
    virtual bool HandleMessage(int message, void* data);            // slot 26
    virtual bool HasBounds() const;                                 // slot 27 (true)
    virtual Rectangle localAABB() const;                            // slot 28
    virtual float minDepth() const;                                 // slot 29 — golden body returns the +376 float in s0
    virtual float maxDepth() const;                                 // slot 30 — golden body returns the +380 float in s0
    virtual std::string className() const;                          // slot 32 — "CollisionShape"

    // Virtuals added by ShapeComponent (slots 33-36), overridden here:
    virtual Color EditColor() const;                                // slot 33
    virtual void Draw(RenderingContext* context, const Matrix4& matrix); // slot 34
    virtual void SetShape(const Shape& shape);                      // slot 35
    virtual void SetDefaultShapeOfType(Shape::Type type);           // slot 36
    // New virtual in this class:
    virtual void UpdateCollisionShape();                            // slot 37

    // Non-virtual, recovered:
    void RegisterLibrary();
    void setOnCollide(const Program& program);
    CollisionShape* collisionShape();
    void SetEnabled(bool enabled);
    void SetUpdatedShape(const Shape& shape);
    void UpdateBounds();
    void UpdateWorldAABB();
    float WorldMinDepth() const;
    float WorldMaxDepth() const;
    Matrix4 WorldMatrix(float depth) const;

    Shape m_shape;                  // +104
    char _shapeComponentTail[8];     // +128 — trailing ShapeComponent storage
    Program m_onCollide;            // +136
    Program m_onCollisionEnd;       // +192
    Program m_onReceiveDamage;      // +248
    CollisionShape m_collisionShape; // +304
    float m_minDepth;               // +376
    float m_maxDepth;               // +380
    Rectangle m_bounds;             // +384
    Rectangle m_worldAABB;          // +400
    bool m_enabled;                 // +416
    bool m_field417;                // +417
    bool m_isGround;                // +418
    bool m_collides;                // +419
    bool m_receivesDamage;          // +420
    bool m_inflictsDamage;          // +421
    char _pad422[2];                // +422
    int m_specialType;              // +424
    float m_friction;               // +428
    bool m_unsafeGround;            // +432
    char _pad433[7];                // +433
    char m_gridEntry[48];           // +440 — SceneGridEntry storage (see top note)
    bool m_field488;                // +488
    char _pad489[3];                // +489
    Vector2 m_field492;             // +492
    bool m_field500;                // +500
    char _pad501[3];                // +501
};

// Golden aarch64 NDK-libc++ layout anchor; MSVC STL member sizes differ,
// so this assert is Android/Linux-only (the Windows build is self-consistent
// under MSVC STL and does not claim golden byte-layout parity).
#ifndef _MSC_VER
static_assert(sizeof(CollisionShapeComponent) == 0x1F8,
              "CollisionShapeComponent size must be 504 bytes");
#endif

} // namespace Caver

#endif
