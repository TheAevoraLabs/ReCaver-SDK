// Recovered from libswordigo.so (1.4.13) evidence — see docs/CORE_HEADERS.md.
// Health pool component; sizeof 0xC0 (proven by Create()'s
// operator new(0xC0)). Member offsets are proven from the ctor,
// InitWithComponent, LoadFromProtobufMessage and TakeDamage bodies.
#ifndef CAVER_HEALTHCOMPONENT_H
#define CAVER_HEALTHCOMPONENT_H

#include "Component.h"
#include "../core/Vector2.h"
#include "../core/Color.h"

namespace Caver {

class RenderingContext;
class Matrix4;
class Camera;
class DamageImpact;
class Sprite;

class HealthComponent : public Component {
public:
    HealthComponent();
    virtual ~HealthComponent();

    static HealthComponent* Create();
    static long Interface();

    virtual Component* Clone() const;                                       // returns HealthComponent* (raw new)
    // NOT an override: the vtable slot 3 holds Component::InitWithComponent.
    // This is a non-virtual overload taking the narrowed type (its mangled
    // name uses EPKS0_ — the parameter is const HealthComponent*).
    void InitWithComponent(const HealthComponent* other);
    virtual void LoadFromProtobufMessage(const Proto::Component& message);
    virtual void SaveToProtobufMessage(Proto::Component* message) const;
    virtual bool ShouldSave();                                              // true
    virtual void GetBindings(std::vector<Binding>* bindings);
    virtual void GetEnumValuesForBindedProperty(int property, std::vector<BindingValue>* values);
    virtual BindingValue ValueForBindedProperty(int property);
    virtual void SetValueForBindedProperty(int property, const BindingValue& value);
    virtual void Prepare();
    void RegisterLibrary();
    virtual void PerformBindedAction(int action);                           // empty
    virtual int componentCategories() const;                                // 2
    virtual bool ImplementsInterface(long interfaceId);
    virtual void RegisterInterfaces(ComponentManager* manager);
    virtual std::string className() const;                                  // "HealthComponent"

    virtual void Update(float deltaTime);
    void Draw(RenderingContext* context, const Matrix4& matrix);
    void DrawHealthBar(RenderingContext* context, const Camera* camera, const Rectangle& rect);

    int TakeDamage(int damage);
    void ApplyFireArmorDamageToObject(SceneObject* object);
    int TakeDamageWithImpact(DamageImpact* impact);    // golden body returns the damage dealt

    boost::intrusive_ptr<Component> m_field104; // +104 — the owner's EntityInfoComponent, cached by Prepare (proven)
    bool m_dead;                // +112
    int m_maxHealth;            // +116
    int m_currentHealth;        // +120
    int m_field124;             // +124 — Load writes it = max value (name UNRECOVERED)
    int m_field128;             // +128 — Load writes it only for values <= 1 (name UNRECOVERED)
    Vector2 m_barOffset;        // +132 — the proto extension's bar_offset; ctor default (0, 50). Load/Save move it as one 8-byte unit
    float m_field140;           // +140 — 0.0f in ctor (name UNRECOVERED)
    float m_field144;           // +144 — 1.0f in ctor (name UNRECOVERED)
    Vector2 m_field148;         // +148 — read as two int counters by TakeDamageWithImpact: fire-armor charges at +148, ice-armor charges at +152 (names UNRECOVERED)
    int m_field156;             // +156 — Update's regen gate (compared >= 1; name UNRECOVERED)
    float m_field160;           // +160 — Update's regen timer (name UNRECOVERED)
    Vector2 m_field164;         // +164 — fade pair (x = time left, y = intensity; Draw gates on x, scales the flash color by y)
    Color m_field172;           // +172 — damage-flash color, set by TakeDamageWithImpact (name UNRECOVERED)
    Sprite* m_field176;         // +176 — damage-flash sprite, lazily created by Draw (sizeof Sprite 0x58, proven)
    bool m_damagedFlag;         // +184 — set by TakeDamage
    char _pad185[7];            // +185 — sizeof 0xC0
};

} // namespace Caver

#endif
