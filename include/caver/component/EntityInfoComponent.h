// Recovered from libswordigo.so (1.4.13) evidence — vtable
// _ZTVN5Caver19EntityInfoComponentE at 0x605150.
#ifndef CAVER_ENTITYINFOCOMPONENT_H
#define CAVER_ENTITYINFOCOMPONENT_H

#include <string>
#include <vector>
#include <boost/shared_ptr.hpp>

#include "Component.h"
#include "../core/Binding.h"
#include "../core/BindingValue.h"
#include "../core/EntityClass.h"

namespace Caver {

class MapNode;
class SceneObject;

class EntityInfoComponent : public Component {
public:
    EntityInfoComponent();
    virtual ~EntityInfoComponent();

    static EntityInfoComponent* Create();
    static long Interface();

    virtual EntityInfoComponent* Clone() const;
    void InitWithComponent(const EntityInfoComponent* other);

    virtual void LoadFromProtobufMessage(const Proto::Component& message);
    virtual void SaveToProtobufMessage(Proto::Component* message) const;
    virtual bool ShouldSave();

    virtual void GetBindings(std::vector<Binding>* bindings);
    virtual void GetEnumValuesForBindedProperty(int property,
                                                std::vector<BindingValue>* values);
    virtual BindingValue ValueForBindedProperty(int property);
    virtual void SetValueForBindedProperty(int property,
                                           const BindingValue& value);
    virtual void PerformBindedAction(int action);

    virtual void Prepare();
    virtual int componentCategories() const;
    virtual bool ImplementsInterface(long interfaceId);
    virtual void RegisterInterfaces(ComponentManager* manager);
    virtual bool HandleMessage(int message, void* data);
    virtual std::string className() const;

    virtual std::string ClassTitle() const;
    virtual bool freezable() const;
    virtual bool stunnable() const;
    virtual bool grabbable() const;
    virtual double magicResistance() const;
    virtual double physicalResistance() const;

    void ApplyToObject(SceneObject* object);
    void ApplyToChildObject(SceneObject* object);

    // Layout past Component base (104 / 0x68 bytes):
    // +104 (bool / byte): m_prepared / initialized flag (*((_BYTE*)this + 104))
    bool m_prepared;                                 // +104
    char _pad105[7];                                 // +105..+111 (align string to 8 bytes at +112)
    std::string m_entityClass;                       // +112..+135 (24 bytes in libc++)
    boost::shared_ptr<EntityClass> m_entityClassPtr; // +136..+151 (16 bytes boost::shared_ptr)
    int m_experienceLevel;                           // +152 (*((_DWORD*)this + 38))
    char _pad156[4];                                 // +156..+159 padding to sizeof 0xA0 (160)
};

// Golden aarch64 NDK-libc++ layout anchor; MSVC STL member sizes differ,
// so this assert is Android/Linux-only (the Windows build is self-consistent
// under MSVC STL and does not claim golden byte-layout parity).
#ifndef _MSC_VER
static_assert(sizeof(EntityInfoComponent) == 0xA0,
              "EntityInfoComponent sizeof must be 160 bytes");
#endif

} // namespace Caver

#endif // CAVER_ENTITYINFOCOMPONENT_H
