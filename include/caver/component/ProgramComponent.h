// Recovered from libswordigo.so (1.4.13) evidence — vtable
// _ZTVN5Caver16ProgramComponentE at 0x61A920 (0x178 bytes: primary
// address point 0x61A940, IBindable secondary 0x61AA48, IUpdateable
// secondary 0x61AA88), _ZTI at 0x61AAC8 (a plain __si_class_type_info
// naming Caver::Component as the single base, 24 bytes).
//
// sizeof 0xC8 (200) is proven twice by the golden: Create()'s
// operator new(0xC8) @0x351188 and Clone()'s @0x33DC34.
//
// A scripted level component: it runs one Caver::Program when the
// object it sits on reports the message matching its Trigger. The
// running state is held in m_programState (+184) and the Lua library
// it installs in Prepare/RegisterLibrary is "Program"
// ({ "Execute", 0x33D964 }, { "IsRunning", 0x33D9E8 }; table at
// .data.rel.ro 0x61AA98).
#ifndef CAVER_PROGRAMCOMPONENT_H
#define CAVER_PROGRAMCOMPONENT_H

#include <string>
#include <vector>
#include <boost/shared_ptr.hpp>

#include "Component.h"
#include "../scripting/Program.h"

namespace Caver {

class ProgramState;

class ProgramComponent : public Component {
public:
    // The IN-ENGINE trigger order, recovered from the two permutation
    // tables the golden switches through: Load's proto->engine table at
    // .rodata 0x264A4C and Save's engine->proto table at 0x264A74. Both
    // are 10-entry permutations proving the engine order below differs
    // from Scene.proto's ProgramComponent::Trigger, where
    // DEACTIVATE = 9, LOAD = 10, USE = 2, POPUP_OK = 3,
    // POPUP_CANCEL = 4, DESTROY = 5, RECEIVE_DAMAGE = 6,
    // BLOCK_DAMAGE = 7 and INFLICT_DAMAGE = 8.
    enum Trigger {
        TriggerNone          = 0,
        TriggerActivate      = 1,
        TriggerDeactivate    = 2,
        TriggerLoad          = 3,
        TriggerUse           = 4,
        TriggerPopupOK       = 5,
        TriggerPopupCancel   = 6,
        TriggerDestroy       = 7,
        TriggerReceiveDamage = 8,
        TriggerBlockDamage   = 9,
        TriggerInflictDamage = 10,
    };

    ProgramComponent();
    virtual ~ProgramComponent();

    static ProgramComponent* Create();
    static long Interface();

    virtual ProgramComponent* Clone() const;
    // NOT an override: vtable slot 3 keeps the BASE function (the golden
    // slot points at Component::InitWithComponent); this is the narrowed
    // non-virtual overload (golden EPKS0_ @0x33C25C).
    void InitWithComponent(const ProgramComponent* other);

    virtual void LoadFromProtobufMessage(const Proto::Component& message);
    virtual void SaveToProtobufMessage(Proto::Component* message) const;
    virtual bool ShouldSave();
    virtual bool Process(std::vector<boost::intrusive_ptr<Component>>* components);
    virtual void Prepare();
    void RegisterLibrary();
    virtual void FinishLoad();

    virtual void GetBindings(std::vector<Binding>* bindings);
    virtual void GetEnumValuesForBindedProperty(int property, std::vector<BindingValue>* values);
    virtual BindingValue ValueForBindedProperty(int property);
    virtual void SetValueForBindedProperty(int property, const BindingValue& value);
    virtual void PerformBindedAction(int action);

    virtual int componentCategories() const;
    virtual bool ImplementsInterface(long interfaceId);
    virtual void RegisterInterfaces(ComponentManager* manager);
    virtual bool HandleMessage(int message, void* data);
    virtual std::string className() const;                       // "Program"

    // Non-virtual (absent from the vtable).
    void Execute();                                             // @0x33D394
    void ResetIfFinished();                                     // @0x33D314

    bool m_enabled;                                             // +104 (0x68) — proto field 3 "enabled"; set true by Process
    char _pad105[3];                                            // +105
    Trigger m_trigger;                                          // +108 (0x6C) — proto field 4 "trigger" (default ACTIVATE)
    bool m_executeOnce;                                         // +112 (0x70) — proto field 1 "execute_once" (default true)
    char _pad113[7];                                            // +113
    Program m_program;                                          // +120 (0x78) — proto field 2 "program"
    bool m_field176;                                            // +176 (0xB0) — gates re-execution in Execute; name UNRECOVERED
    char _pad177[7];                                            // +177
    // The state of the currently running program. The golden stores and
    // releases it with the boost::shared_ptr refcount sequence
    // (use_count at +8 / weak_count at +12 of the control block).
    boost::shared_ptr<ProgramState> m_programState;              // +184 (0xB8)
};

// Golden aarch64 NDK-libc++ layout anchor; MSVC STL member sizes differ,
// so this assert is Android/Linux-only (the Windows build is self-consistent
// under MSVC STL and does not claim golden byte-layout parity).
#ifndef _MSC_VER
static_assert(sizeof(ProgramComponent) == 200,
              "ProgramComponent size must be 0xC8 as allocated in Create/Clone");
#endif

// The trigger names the editor bindings show (golden
// @0x33C51C, a plain namespace-scope function over the nested enum).
std::string StringForTrigger(ProgramComponent::Trigger trigger);

} // namespace Caver

#endif // CAVER_PROGRAMCOMPONENT_H
