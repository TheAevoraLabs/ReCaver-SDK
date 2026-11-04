#ifndef CAVER_MAGICBOMBCOMPONENT_H
#define CAVER_MAGICBOMBCOMPONENT_H

#include "SpellComponent.h"
#include "ModelComponent.h"
#include "../core/FloatColor.h"
#include <boost/intrusive_ptr.hpp>

namespace Caver {

class MagicBombComponent : public SpellComponent {
public:
    MagicBombComponent();
    virtual ~MagicBombComponent();
    static MagicBombComponent* Create();
    static long Interface();

    virtual Component* Clone() const;
    void InitWithComponent(const MagicBombComponent* other);
    virtual void LoadFromProtobufMessage(const Proto::Component& message);
    virtual void SaveToProtobufMessage(Proto::Component* message) const;
    virtual bool ShouldSave();
    virtual void GetBindings(std::vector<Binding>* bindings);
    virtual void GetEnumValuesForBindedProperty(int property, std::vector<BindingValue>* values);
    virtual BindingValue ValueForBindedProperty(int property);
    virtual void SetValueForBindedProperty(int property, const BindingValue& value);
    virtual void PerformBindedAction(int action);
    virtual void Prepare();
    virtual bool ImplementsInterface(long interfaceId);
    virtual void RegisterInterfaces(ComponentManager* manager);
    virtual bool RequiresUpdate() const;
    virtual std::string className() const;
    virtual void Cast();
    virtual void Update(float deltaTime);

    void Explode();

    bool m_active;                                     // +192
    char _pad193[3];
    float m_elapsed;                                   // +196
    float m_fuseDuration;                              // +200
    char _pad204[4];
    boost::intrusive_ptr<SceneObject> m_caster;        // +208
    FloatColor m_color;                                // +216
    float m_pulseTimer;                                // +232
    char _pad236[4];
    boost::intrusive_ptr<ModelComponent> m_targetObject; // +240
};

// Golden aarch64 NDK-libc++ layout anchor; MSVC STL member sizes differ,
// so this assert is Android/Linux-only (the Windows build is self-consistent
// under MSVC STL and does not claim golden byte-layout parity).
#ifndef _MSC_VER
static_assert(sizeof(MagicBombComponent) == 0xF8, "sizeof must be 248 bytes");
#endif

} // namespace Caver

#endif
