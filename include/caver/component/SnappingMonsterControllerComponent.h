// Recovered from libswordigo.so (1.4.13) evidence — vtable
// _ZTVN5Caver34SnappingMonsterControllerComponentE at 0x621E70.
// sizeof 0x148 (328 bytes) proven by:
//   operator new(0x148u) in Create() @0x350A88 and Clone() @0x391A74.
#ifndef CAVER_SNAPPINGMONSTERCONTROLLERCOMPONENT_H
#define CAVER_SNAPPINGMONSTERCONTROLLERCOMPONENT_H

#include <vector>
#include <string>

#include "MonsterControllerComponent.h"
#include "KeyframeAnimationComponent.h"
#include "BlendAnimationComponent.h"
#include "ShapeComponent.h"
#include "SoundEffectComponent.h"
#include "../animation/AnimBlendNode.h"

namespace Caver {

class SnappingMonsterControllerComponent : public MonsterControllerComponent {
public:
    SnappingMonsterControllerComponent();
    virtual ~SnappingMonsterControllerComponent();

    static SnappingMonsterControllerComponent* Create();
    static long Interface();

    virtual SnappingMonsterControllerComponent* Clone() const;
    void InitWithComponent(const SnappingMonsterControllerComponent* other);

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

    void RegisterLibrary();
    void Attack();
    void StopAttacking();

    ComponentOutlet<KeyframeAnimationComponent> m_standAnimationOutlet;   // +200 — ID 100 "standAnimation"
    ComponentOutlet<KeyframeAnimationComponent> m_attackAnimationOutlet;  // +224 — ID 101 "attackAnimation"
    ComponentOutlet<BlendAnimationComponent> m_blendAnimationOutlet;      // +248 — ID 102 "blendAnimation"
    ComponentOutlet<ShapeComponent> m_attackAreaOutlet;                   // +272 — ID 103 "attackArea"
    ComponentOutlet<SoundEffectComponent> m_attackSoundOutlet;            // +296 — ID 104 "attackSound"
    bool m_isAttacking;                                                   // +320 — Active attack animation flag
    char _pad321[3];                                                      // +321 — Alignment padding
    float m_attackTimer;                                                  // +324 — Cooldown / detection timer (FLT_MAX init)
};

static_assert(sizeof(SnappingMonsterControllerComponent) == 0x148, "SnappingMonsterControllerComponent must be exactly 328 bytes");

} // namespace Caver

#endif // CAVER_SNAPPINGMONSTERCONTROLLERCOMPONENT_H
