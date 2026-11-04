// Recovered from libswordigo.so (1.4.13) evidence — Caver::MonsterEntityComponent.
//
// The monster-side EntityComponent: it adds the onHurt/onKill script hooks,
// the cached Damage/Health components, facing rotation, XP reward and the
// death presentation (item drops + MonsterDeathController slicing).
//
// Base: EntityComponent (RTTI __si_class_type_info). Primary vtable at
// 0x61F050 (address point), IBindable secondary 0x61F168, IUpdateable
// secondary 0x61F1A8. Overridden slots: 2/3 dtor, Clone, Load, Save,
// ShouldSave, Prepare, GetBindings, GetEnumValuesForBindedProperty,
// ValueForBindedProperty, SetValueForBindedProperty, PerformBindedAction,
// componentCategories, ImplementsInterface, RegisterInterfaces,
// UpdateWhenPaused, Update, HandleMessage, className. Slot 3 keeps
// Component::InitWithComponent, so InitWithComponent below is the
// non-virtual narrowed overload (the MonsterController pattern).
//
// sizeof 0x1B8 (440) is proven by Create()/Clone()'s operator new(0x1B8u).
// Member map from the ctor store map @0x373FAC and the D2 teardown
// @0x375FA0:
//   +296  Program onHurt       (proto extension 158 field 2, property 100)
//   +352  Program onKill       (proto extension 158 field 1, property 101)
//   +408  float rotation target (Update writes it, then drives the owner's
//         +144 rotation toward it; 0 when falling)
//   +412  float damage-enable timer (ctor FLT_MAX; Update accumulates and
//         re-arms the cached DamageComponent's +180 gate after 0.2s)
//   +416  intrusive_ptr<DamageComponent> (cached by Prepare)
//   +424  intrusive_ptr<HealthComponent> (cached by Prepare)
//   +432  bool givesExperience        (proto field 3, property 102)
//   +433  bool damageEnabled          (SetDamageEnabled; NOT serialised)
//   +434  bool defaultDeathAnimation  (proto field 4, property 103)
#ifndef CAVER_MONSTERENTITYCOMPONENT_H
#define CAVER_MONSTERENTITYCOMPONENT_H

#include <string>
#include <vector>

#include <boost/intrusive_ptr.hpp>

#include "EntityComponent.h"
#include "DamageComponent.h"
#include "HealthComponent.h"
#include "../scripting/Program.h"

namespace Caver {

class MonsterEntityComponent : public EntityComponent {
public:
    MonsterEntityComponent();
    virtual ~MonsterEntityComponent();

    static MonsterEntityComponent* Create();
    static long Interface();

    virtual MonsterEntityComponent* Clone() const;
    // NOT an override: vtable slot 3 keeps Component::InitWithComponent.
    void InitWithComponent(const MonsterEntityComponent* other);

    virtual void LoadFromProtobufMessage(const Proto::Component& message);
    virtual void SaveToProtobufMessage(Proto::Component* message) const;
    virtual bool ShouldSave();
    virtual void GetBindings(std::vector<Binding>* bindings);
    virtual void GetEnumValuesForBindedProperty(int property, std::vector<BindingValue>* values);
    virtual BindingValue ValueForBindedProperty(int property);
    virtual void SetValueForBindedProperty(int property, const BindingValue& value);
    virtual void PerformBindedAction(int action);
    virtual void Prepare();
    virtual int componentCategories() const;
    virtual bool ImplementsInterface(long interfaceId);
    virtual void RegisterInterfaces(ComponentManager* manager);
    virtual bool UpdateWhenPaused() const;
    virtual void Update(float deltaTime);
    virtual bool HandleMessage(int message, void* data);
    virtual std::string className() const;

    // Enables/disables the cached DamageComponent's intake gate (its
    // +180 byte) and mirrors it at +433.
    void SetDamageEnabled(bool enabled);
    // Message 14 payload handler (non-virtual, called from HandleMessage).
    void HandleDamageImpact(DamageImpact* impact);

    Program m_onHurt;                               // +296 — property 100
    Program m_onKill;                               // +352 — property 101
    float m_rotationTarget;                         // +408
    float m_damageEnableTimer;                      // +412 — FLT_MAX in ctor
    boost::intrusive_ptr<DamageComponent> m_damage; // +416
    boost::intrusive_ptr<HealthComponent> m_health; // +424
    bool m_givesExperience;                         // +432 — property 102
    bool m_damageEnabled;                           // +433
    bool m_defaultDeathAnimation;                   // +434 — property 103
    char _pad435[5];                                // +435 — sizeof 0x1B8
};

// sizeof 0x1B8 is proven by the golden's two operator new(0x1B8u) sites.
// (The class is not standard-layout, so member offsets are checked with
// the compiler's record layout rather than offsetof.)
// Golden aarch64 NDK-libc++ layout anchor; MSVC STL member sizes differ,
// so this assert is Android/Linux-only (the Windows build is self-consistent
// under MSVC STL and does not claim golden byte-layout parity).
#ifndef _MSC_VER
static_assert(sizeof(MonsterEntityComponent) == 0x1B8,
              "MonsterEntityComponent sizeof must be 440 bytes");
#endif

} // namespace Caver

#endif // CAVER_MONSTERENTITYCOMPONENT_H
