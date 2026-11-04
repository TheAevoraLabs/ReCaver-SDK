// Recovered from libswordigo.so (1.4.13) evidence — vtable
// _ZTVN5Caver36ProjectileMonsterControllerComponentE at 0x621960
// (primary address point 0x621970, IBindable secondary 0x621A88,
// IUpdateable secondary 0x621AC8; _ZTI at 0x621AD8 is a
// __si_class_type_info naming Caver::MonsterControllerComponent as the
// single base).
//
// sizeof 0xC8 (200 bytes) equals the base class and is proven twice by
// the golden: Create() @0x350BC8 and Clone() @0x38E490 both allocate
// operator new(0xC8u). The ctor @0x38E1A0 only calls the base ctor and
// installs the three vptrs, so this class adds no members.
//
// Behavioural overrides: Prepare, Update, HandleMessage,
// ImplementsInterface, RegisterInterfaces, className and ShouldSave.
// LoadFromProtobufMessage, SaveToProtobufMessage and the four binding
// methods are out-of-line thunks that forward to
// MonsterControllerComponent (the class persists through the base
// extension, number 302).
#ifndef CAVER_PROJECTILEMONSTERCONTROLLERCOMPONENT_H
#define CAVER_PROJECTILEMONSTERCONTROLLERCOMPONENT_H

#include <string>
#include <vector>

#include "MonsterControllerComponent.h"

namespace Caver {

class ProjectileMonsterControllerComponent : public MonsterControllerComponent {
public:
    // No destructor is declared: the golden emits only the implicit
    // deleting destructor (slot 3) and leaves the complete-object slot 2
    // pointing at MonsterControllerComponent's destructor.
    ProjectileMonsterControllerComponent();

    static ProjectileMonsterControllerComponent* Create();
    static long Interface();

    virtual ProjectileMonsterControllerComponent* Clone() const;
    // NOT an override: vtable slot 3 holds Component::InitWithComponent.
    // The golden emits this narrowed overload as a thunk to
    // MonsterControllerComponent::InitWithComponent.
    void InitWithComponent(const ProjectileMonsterControllerComponent* other);

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
};

static_assert(sizeof(ProjectileMonsterControllerComponent) == 0xC8,
              "ProjectileMonsterControllerComponent sizeof must be exactly 200 bytes (0xC8)");

} // namespace Caver

#endif // CAVER_PROJECTILEMONSTERCONTROLLERCOMPONENT_H
