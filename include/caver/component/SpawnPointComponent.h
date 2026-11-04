#ifndef CAVER_SPAWNPOINTCOMPONENT_H
#define CAVER_SPAWNPOINTCOMPONENT_H

#include "Component.h"
#include "../core/Vector3.h"
#include <string>
#include <vector>

namespace Caver {

// Recovered from libswordigo.so (1.4.13) evidence — vtable _ZTVN5Caver19SpawnPointComponentE
// at 0x61B3B0 (sizeof 0x78 = 120 bytes).
class SpawnPointComponent : public Component {
public:
    SpawnPointComponent();
    virtual ~SpawnPointComponent();

    static SpawnPointComponent* Create();
    static long Interface();

    virtual SpawnPointComponent* Clone() const;
    void InitWithComponent(const SpawnPointComponent* other);

    virtual void LoadFromProtobufMessage(const Proto::Component& message);
    virtual void SaveToProtobufMessage(Proto::Component* message) const;
    virtual bool ShouldSave();
    virtual int componentCategories() const;

    virtual void GetBindings(std::vector<Binding>* bindings);
    virtual void GetEnumValuesForBindedProperty(int property, std::vector<BindingValue>* values);
    virtual BindingValue ValueForBindedProperty(int property);
    virtual void SetValueForBindedProperty(int property, const BindingValue& value);
    virtual void PerformBindedAction(int action);
    virtual bool ImplementsInterface(long interfaceId);
    virtual void RegisterInterfaces(ComponentManager* manager);
    virtual std::string className() const;

    int m_facingDirection;        // +104 (default 1)
    Vector3 m_spawnOffset;        // +108 (default {0, 0, 0})
};

} // namespace Caver

#endif // CAVER_SPAWNPOINTCOMPONENT_H
