// Recovered from libswordigo.so (1.4.13) evidence — vtable
// _ZTVN5Caver19PropertiesComponentE at 0x61AAE0 (0x1A8 bytes: primary
// address point 0x61AAF0, IBindable secondary 0x61AC08, IUpdateable
// secondary 0x61AC48), _ZTI at 0x61AC88 (a plain __si_class_type_info
// naming Caver::Component as the single base).
//
// sizeof 0xC0 (192) is proven twice by the golden: Create()'s
// operator new(0xC0) @0x350398 and Clone()'s @0x33EA18.
//
// A string property bag with an optional on-load program: scripts read
// and write it through the "Properties" Lua library
// ({ "SetProperty", 0x33E630 }, { "GetProperty", 0x33E77C }; table at
// .data.rel.ro 0x61AC58), which is also what SetPropertyForKey /
// PropertyForKey expose to native callers.
#ifndef CAVER_PROPERTIESCOMPONENT_H
#define CAVER_PROPERTIESCOMPONENT_H

#include <map>
#include <string>
#include <vector>

#include "Component.h"
#include "../scripting/Program.h"

namespace Caver {

class ProgramState;

class PropertiesComponent : public Component {
public:
    PropertiesComponent();
    virtual ~PropertiesComponent();

    static PropertiesComponent* Create();
    static long Interface();

    virtual PropertiesComponent* Clone() const;
    // NOT an override: vtable slot 3 keeps the BASE function (the golden
    // slot points at Component::InitWithComponent); this is the narrowed
    // non-virtual overload (golden EPKS0_ @0x33DD6C).
    void InitWithComponent(const PropertiesComponent* other);

    virtual void LoadFromProtobufMessage(const Proto::Component& message);
    virtual void SaveToProtobufMessage(Proto::Component* message) const;
    virtual bool ShouldSave();
    virtual void Prepare();
    void RegisterLibrary();

    virtual void GetBindings(std::vector<Binding>* bindings);
    virtual void GetEnumValuesForBindedProperty(int property, std::vector<BindingValue>* values);
    virtual BindingValue ValueForBindedProperty(int property);
    virtual void SetValueForBindedProperty(int property, const BindingValue& value);
    virtual void PerformBindedAction(int action);

    virtual int componentCategories() const;
    virtual bool ImplementsInterface(long interfaceId);
    virtual void RegisterInterfaces(ComponentManager* manager);
    virtual std::string className() const;                       // "Properties"

    // Non-virtual (absent from the vtable).
    // Runs the on-load program once; the +104 flag makes it idempotent
    // and is checked by every property accessor below.
    void Load();                                                // @0x33E3F0
    void SetPropertyForKey(const std::string& key,
                           const std::string& value);           // @0x33E4F0
    std::string PropertyForKey(const std::string& key);          // @0x33E580

    bool m_field104;                                            // +104 (0x68) — "already loaded" latch, set by Load; name UNRECOVERED
    char _pad105[7];                                            // +105
    Program m_program;                                          // +112 (0x70) — proto field 1 "on_load"
    // std::map<std::string, std::string>: the node's mapped value is the
    // string at +56 of the __tree_node (the golden assigns it through
    // std::string::operator= at that offset).
    std::map<std::string, std::string> m_properties;            // +168 (0xA8)
};

static_assert(sizeof(PropertiesComponent) == 192,
              "PropertiesComponent size must be 0xC0 as allocated in Create/Clone");

} // namespace Caver

#endif // CAVER_PROPERTIESCOMPONENT_H
