// Recovered from libswordigo.so (1.4.13) evidence — vtable
// _ZTVN5Caver34ShootingMonsterControllerComponentE at 0x621B00.
// sizeof 0x120 (288 bytes) proven by:
//   operator new(0x120u) in Create() @0x350B88 and Clone() @0x38F594.
#ifndef CAVER_SHOOTINGMONSTERCONTROLLERCOMPONENT_H
#define CAVER_SHOOTINGMONSTERCONTROLLERCOMPONENT_H

#include <cmath>
#include <limits>
#include <string>
#include <vector>

#include "MonsterControllerComponent.h"
#include "KeyframeAnimationComponent.h"
#include "ReferenceCountedObject.h"
#include "../animation/AnimBlendNode.h"
#include "../scene/ObjectTemplate.h"

namespace Caver {

class ShootingMonsterControllerComponent : public MonsterControllerComponent {
public:
    ShootingMonsterControllerComponent();
    virtual ~ShootingMonsterControllerComponent();

    static ShootingMonsterControllerComponent* Create();
    static long Interface();

    virtual ShootingMonsterControllerComponent* Clone() const;
    void InitWithComponent(const ShootingMonsterControllerComponent* other);

    virtual void LoadFromProtobufMessage(const Proto::Component& message);
    virtual void SaveToProtobufMessage(Proto::Component* message) const;
    virtual bool ShouldSave();

    virtual void GetBindings(std::vector<Binding>* bindings);
    virtual void GetEnumValuesForBindedProperty(int property, std::vector<BindingValue>* values);
    virtual BindingValue ValueForBindedProperty(int property);
    virtual void SetValueForBindedProperty(int property, const BindingValue& value);
    virtual void PerformBindedAction(int action);

    virtual void Prepare();
    virtual void Update(float deltaTime);
    virtual bool HandleMessage(int message, void* data);

    virtual bool ImplementsInterface(long interfaceId);
    virtual void RegisterInterfaces(ComponentManager* manager);
    virtual std::string className() const;

    void StartMovingToDirection(int direction);
    void Shoot();

    KeyframeAnimationComponent* walkAnimation();
    KeyframeAnimationComponent* shootAnimation();

    ComponentOutlet<KeyframeAnimationComponent> m_walkAnimationOutlet;   // +200 — ID 100 "walkAnimation"
    ComponentOutlet<KeyframeAnimationComponent> m_shootAnimationOutlet;  // +224 — ID 101 "shootAnimation"
    boost::intrusive_ptr<AnimBlendNode> m_blendNode;                    // +248 — cross-fades walk and shoot
    int m_direction;                                                    // +256 — facing / movement direction (-1, 1, 0 initially)
    char _pad260[4];                                                    // +260 — padding
    boost::intrusive_ptr<ObjectTemplate> m_projectileTemplate;          // +264 — "snart_projectile" template
    float m_shootTimer;                                                 // +272 — timer since last shot (init FLT_MAX)
    bool m_isShooting;                                                  // +276 — true while shoot animation is active
    bool m_projectileSpawned;                                           // +277 — true once projectile spawned at frame >= 7
    char _pad278[2];                                                    // +278 — padding
    float m_turnTimer;                                                  // +280 — timer since direction change / turn (init FLT_MAX)
    char _pad284[4];                                                    // +284 — pad to 0x120 (288 bytes)
};

static_assert(sizeof(ShootingMonsterControllerComponent) == 0x120,
              "ShootingMonsterControllerComponent sizeof must be exactly 288 bytes");

} // namespace Caver

#endif // CAVER_SHOOTINGMONSTERCONTROLLERCOMPONENT_H
