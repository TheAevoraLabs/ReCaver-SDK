#ifndef CAVER_DIMENSIONSPELLCOMPONENT_H
#define CAVER_DIMENSIONSPELLCOMPONENT_H

#include "../scene/SceneObject.h"
#include "SpellComponent.h"

namespace Caver {

class DimensionSpellComponent : public SpellComponent {
public:
    DimensionSpellComponent();
    virtual ~DimensionSpellComponent();
    static DimensionSpellComponent* Create();
    static long Interface();

    virtual Component* Clone() const;
    void InitWithComponent(const DimensionSpellComponent* other);
    virtual void LoadFromProtobufMessage(const Proto::Component& message);
    // 0x3792C0: thunk to SpellComponent::SaveToProtobufMessage.
    virtual void SaveToProtobufMessage(Proto::Component* message) const;
    virtual void GetBindings(std::vector<Binding>* bindings);
    virtual void GetEnumValuesForBindedProperty(int property, std::vector<BindingValue>* values);
    virtual BindingValue ValueForBindedProperty(int property);
    virtual void SetValueForBindedProperty(int property, const BindingValue& value);
    virtual void PerformBindedAction(int action);
    virtual bool ShouldSave();
    virtual bool ImplementsInterface(long interfaceId);
    virtual void RegisterInterfaces(ComponentManager* manager);
    virtual std::string className() const;
    virtual void Cast();

    bool m_dimensionActive; // +192
    char _pad193[7];
};

// Golden aarch64 NDK-libc++ layout anchor; MSVC STL member sizes differ,
// so this assert is Android/Linux-only (the Windows build is self-consistent
// under MSVC STL and does not claim golden byte-layout parity).
#ifndef _MSC_VER
static_assert(sizeof(DimensionSpellComponent) == 0xC8,
              "DimensionSpellComponent sizeof must be 200 bytes");
#endif

} // namespace Caver

#endif
