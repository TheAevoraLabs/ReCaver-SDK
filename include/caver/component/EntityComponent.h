// Recovered from libswordigo.so (1.4.13) evidence — Caver::EntityComponent,
// the base living/moving-entity component. sizeof 0x128 (296), proven by
// Create()'s operator new(0x128). Layout proven from the ctor store map
// @0x36488C, the D2 teardown @0x3660D0, InitWithComponent/Clone and the
// Update/HandleGroundCollision/HandleMessage bodies.
//
// The PhysicsObjectState is embedded BY VALUE at +112. Its golden
// embedded span here is 156 bytes (112..267): EntityComponent's own
// flash / saved-position members live at +248..+267, INSIDE the state's
// tail region (state offsets +136..+155 — bytes PhysicsObjectState's own
// functions never touch; see physics/PhysicsObjectState.h, whose
// in-tree sizeof is 152, hence the explicit 4-byte tail pad below).
// Those overlapping members are therefore accessed through the raw-offset
// helpers in the .cpp, not as C++ members.
#ifndef CAVER_ENTITYCOMPONENT_H
#define CAVER_ENTITYCOMPONENT_H

#include "Component.h"
#include "../physics/PhysicsObjectState.h"
#include "../core/Color.h"

namespace Caver {

class CollisionShapeComponent;
class CollisionInfo;
class ParticleEmitterComponent;

class EntityComponent : public Component {
public:
    EntityComponent();
    virtual ~EntityComponent();

    static EntityComponent* Create();
    static long Interface();

    virtual EntityComponent* Clone() const;
    // NOT an override: the vtable slot 3 holds Component::InitWithComponent.
    // This is a non-virtual overload taking the narrowed type (its mangled
    // name uses EPKS0_ — the parameter is const EntityComponent*).
    void InitWithComponent(const EntityComponent* other);
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
    virtual bool HandleMessage(int message, void* data);
    virtual void Update(float deltaTime);
    virtual int componentCategories() const;                                // 2
    virtual bool ImplementsInterface(long interfaceId);
    virtual void RegisterInterfaces(ComponentManager* manager);
    virtual std::string className() const;                                  // "Entity"

    void SetFacingDirection(int direction);
    void FlashModel(const Color& color, float duration);
    void HandleGroundCollision(const CollisionMessageData& data);
    void HandleDamageCollision(CollisionShapeComponent* shapeA,
                               CollisionShapeComponent* shapeB,
                               const CollisionInfo& info);                  // empty

    int m_facingDirection;                  // +104 — proto EntityComponent field 1 (facing_direction, default 1); editor property 0 "facingDirection"
    char _pad108[4];                        // +108
    PhysicsObjectState m_physicsState;      // +112 — embedded by value; the state's m_enabled (+228 abs) is editor property 1 "physicsEnabled" / proto field 2
    char _stateTail[4];                     // +264 — rest of the state's 156-byte embedded span (its +148 field sits at abs +260, inside the span)
    bool m_field268;                        // +268 — HandleMessage(9) gate byte (name UNRECOVERED; only the ctor clears it in this class)
    // +269 — dead/incapacitated flag read by CharControllerComponent::Update
    // (name UNRECOVERED). FIX 2026-10-28 (hero runs-in-place, GDB-proven):
    // promoted from pad to a named bool initialised false in the ctor.
    // Golden writes +269 = 1 only on the hero-death path
    // (HeroEntityComponent.cpp:532), i.e. zero-while-alive — and the
    // golden Entity ctor (0x36488C) itself zeroes +269, via a 16-bit
    // WORD store at +268 covering bytes 268-269, so this ctor
    // initialisation reproduces golden behaviour rather than inventing
    // it. Before the fix this build never wrote the byte, so it held
    // uninitialised heap memory (0xC5 observed) and Update zeroed
    // facing every frame, giving physics target speed runSpeed x 0 = 0
    // with the run animation still playing. Layout unchanged: 1 + 2
    // pad bytes, m_dustObject still at +272.
    bool m_field269;
    char _pad270[2];                        // +270..+271
    boost::intrusive_ptr<SceneObject> m_dustObject; // +272 — lazily created "wsmoke" template object spawned on hard ground landings
    ParticleEmitterComponent* m_particleEmitter;    // +280 — RAW pointer to the dust object's ParticleEmitterComponent (cached by HandleGroundCollision, not retained)
    bool m_emitting;                        // +288 — dust emission active flag
    char _pad289[3];                        // +289
    float m_emittingTimer;                  // +292 — time since dust emission started (StopEmitting after > 0.01)
};

} // namespace Caver

#endif
