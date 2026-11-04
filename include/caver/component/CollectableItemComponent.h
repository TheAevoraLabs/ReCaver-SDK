// Recovered from libswordigo.so (1.4.13) evidence — vtable
// _ZTVN5Caver24CollectableItemComponentE at 0x61DC80 (0x108 bytes:
// primary address point 0x61DC80, IBindable secondary 0x61DD98,
// IUpdateable secondary 0x61DDD8).
//
// sizeof 0xE0 (224 bytes) is proven by:
//   operator new(0xE0u) in Create() @0x351108 and Clone() @0x3619F0.
#ifndef CAVER_COLLECTABLEITEMCOMPONENT_H
#define CAVER_COLLECTABLEITEMCOMPONENT_H

#include <string>
#include <vector>

#include "Component.h"
#include "../scripting/Program.h"

namespace Caver {

class SceneObject;

class CollectableItemComponent : public Component {
public:
    enum Type {
        TYPE_UNKNOWN = 0,
        TYPE_HEALTH_POTION = 1,
        TYPE_MANA_POTION = 2,
        TYPE_MAGIC_POWER = 3,
        TYPE_COIN = 4,
        TYPE_EXPERIENCE = 5
    };

    CollectableItemComponent();
    virtual ~CollectableItemComponent();

    static CollectableItemComponent* Create();
    static long Interface();

    virtual CollectableItemComponent* Clone() const;
    void InitWithComponent(const CollectableItemComponent* other);

    virtual void LoadFromProtobufMessage(const Proto::Component& message);
    virtual void SaveToProtobufMessage(Proto::Component* message) const;
    virtual bool ShouldSave();
    virtual int componentCategories() const;
    virtual void Prepare();
    void RegisterLibrary();

    virtual void GetBindings(std::vector<Binding>* bindings);
    virtual void GetEnumValuesForBindedProperty(int property, std::vector<BindingValue>* values);
    virtual BindingValue ValueForBindedProperty(int property);
    virtual void SetValueForBindedProperty(int property, const BindingValue& value);
    virtual void PerformBindedAction(int action);

    virtual bool ImplementsInterface(long interfaceId);
    virtual void RegisterInterfaces(ComponentManager* manager);
    virtual std::string className() const;

    bool IsCollected() const;
    void HandleCollection();

    Type m_type;                  // +104 (default 0)
    int m_value;                  // +108 (default 0)
    std::string m_itemName;       // +112 (default empty)
    std::string m_identifier;     // +136 (default empty)
    bool m_requiresPickup;        // +160 (default false)
    char _pad161[7];              // +161
    Program m_onCollect;          // +168 (sizeof 56)
};

// Golden aarch64 NDK-libc++ layout anchor; MSVC STL member sizes differ,
// so this assert is Android/Linux-only (the Windows build is self-consistent
// under MSVC STL and does not claim golden byte-layout parity).
#ifndef _MSC_VER
static_assert(sizeof(CollectableItemComponent) == 0xE0, "CollectableItemComponent sizeof must be 224 bytes");
#endif

} // namespace Caver

#endif // CAVER_COLLECTABLEITEMCOMPONENT_H
