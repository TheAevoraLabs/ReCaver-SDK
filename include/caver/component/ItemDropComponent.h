// Recovered from libswordigo.so (1.4.13) evidence.
// Item-drop component: when triggered (Lua "ItemDrop" library /
// PerformBindedAction), rolls its drop table and spawns collectable
// item objects around the owner. sizeof 0xC8 (proven by Create()'s
// operator new(0xC8) and Clone()). Member offsets are proven from the
// ctor (0x370774), InitWithComponent (0x3707D0), Load/Save bodies and
// ~ItemDropComponent (0x3736A0).
#ifndef CAVER_ITEMDROPCOMPONENT_H
#define CAVER_ITEMDROPCOMPONENT_H

#include "Component.h"
#include "../scene/FastVector.h"

namespace Caver {

class ItemDropComponent : public Component {
public:
    // One row of the drop table (stride 64, proven by the FastVector
    // loops). Field order proven from Load/Save: the two strings, then
    // drop_chance / min_count / max_count packed at +48/+52/+56 (moved
    // as one qword + one dword in InitWithComponent).
    struct ItemDropEntry {
        std::string m_templateName;     // +0
        std::string m_itemIdentifier;   // +24
        float m_dropChance;             // +48
        int m_minCount;                 // +52
        int m_maxCount;                 // +56
        char _pad60[4];                 // +60 — sizeof 64
        ~ItemDropEntry();   // out-of-line in the golden (weak, 0x370BAC)
    };

    // A concrete rolled drop (stride 48, proven by the local
    // FastVector<ItemDrop> in CreateItemObjects and its teardown).
    struct ItemDrop {
        std::string m_templateName;     // +0
        std::string m_itemIdentifier;   // +24
        ~ItemDrop();        // out-of-line in the golden (weak, 0x3732CC)
    };

    ItemDropComponent();
    virtual ~ItemDropComponent();

    static ItemDropComponent* Create();
    static long Interface();

    virtual Component* Clone() const;
    // NOT an override: vtable slot 3 keeps Component::InitWithComponent
    // (proven from _ZTVN5Caver17ItemDropComponentE relocations).
    void InitWithComponent(const ItemDropComponent* other);
    virtual void LoadFromProtobufMessage(const Proto::Component& message);
    virtual void SaveToProtobufMessage(Proto::Component* message) const;
    virtual bool ShouldSave();                                              // true
    virtual void GetBindings(std::vector<Binding>* bindings);
    virtual void GetEnumValuesForBindedProperty(int property, std::vector<BindingValue>* values);
    virtual BindingValue ValueForBindedProperty(int property);
    virtual void SetValueForBindedProperty(int property, const BindingValue& value);
    virtual void PerformBindedAction(int action);
    virtual void Prepare();
    void RegisterLibrary();
    virtual int componentCategories() const;                                // 2
    virtual bool ImplementsInterface(long interfaceId);
    virtual void RegisterInterfaces(ComponentManager* manager);
    virtual std::string className() const;                                  // "ItemDrop"

    // Non-virtual gameplay API (the Lua "ItemDrop" library forwards to
    // Trigger / AllItemsCollected / the entry accessors).
    void Trigger();
    void CreateItemObjects();
    void DropItemObjects(const std::vector<boost::intrusive_ptr<SceneObject> >& objects);
    bool IsItemCollected(const std::string& identifier);
    bool AllItemsCollected();
    int RandomFromOdds(int count, float* odds);
    int RandomSingleItem(int count, ItemDropEntry* entries);

    bool m_canDropMultipleItems;    // +104 — ctor true (word at +104 = 1)
    bool m_canDropDefaultItems;     // +105 — ctor false
    char _pad106[6];                // +106
    std::string m_itemName;         // +112 — proto field 1
    std::string m_itemIdentifier;   // +136 — proto field 2
    FastVector<ItemDropEntry> m_entries;                        // +160 (count +160, capacity +164, items +168)
    std::vector<boost::intrusive_ptr<SceneObject> > m_createdObjects; // +176 — filled by CreateItemObjects, consumed by Trigger
};

} // namespace Caver

#endif
