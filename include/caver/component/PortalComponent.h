// Recovered from libswordigo.so (1.4.13) evidence — vtable
// _ZTVN5Caver15PortalComponentE at 0x61A710 (0x178 bytes: primary
// address point 0x61A720, IBindable secondary 0x61A838, IUpdateable
// secondary 0x61A878), _ZTI at 0x61A8B8 (a plain __si_class_type_info
// naming Caver::Component as the single base).
//
// sizeof 0xC0 (192) is proven twice by the golden: Create()'s
// operator new(0xC0) @0x350F88 and Clone()'s @0x33BF88.
//
// A scripted level portal. It holds an outlet to the
// CollisionShapeComponent that triggers it (BindOutlet 0, "triggerShape",
// @0x33AD7C in the ctor) and, when tapped, builds a throwaway
// SceneObject carrying a glow / sprite / touchable component, then sends
// a DidEnterPortalGameEvent for the destination scene.
#ifndef CAVER_PORTALCOMPONENT_H
#define CAVER_PORTALCOMPONENT_H

#include <string>
#include <boost/intrusive_ptr.hpp>

#include "Component.h"
#include "ComponentOutlet.h"
#include "CollisionShapeComponent.h"

namespace Caver {

class SceneObject;

class PortalComponent : public Component {
public:
    PortalComponent();
    virtual ~PortalComponent();

    static PortalComponent* Create();
    static long Interface();

    virtual PortalComponent* Clone() const;
    // NOT an override: vtable slot 3 keeps the BASE function; this is the
    // narrowed non-virtual overload (golden EPKS0_ @0x33AE60).
    void InitWithComponent(const PortalComponent* other);

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
    virtual bool HandleMessage(int message, void* data);
    virtual std::string className() const;                       // "Portal"

    // Non-virtual (absent from the vtable — no slots beyond className).
    void Activate();                                            // @0x33B6B0
    void Enter();                                               // @0x33BBE4
    void Deactivate();                                          // @0x33BC94

    // +104 (0x68) — ComponentOutlet<CollisionShapeComponent>. Its
    // identifier (+0x70) is the trigger shape's component identifier,
    // loaded from proto field 4 and re-adopted by Prepare when a portal
    // shape is found on the object.
    ComponentOutlet<CollisionShapeComponent> m_shapeOutlet;
    std::string m_destinationSceneName;                         // +128 (0x80) — proto field 1
    std::string m_spawnPointName;                               // +152 (0x98) — proto field 2
    bool m_tapToEnter;                                          // +176 (0xB0) — proto field 3
    char _pad177[7];                                            // +177
    boost::intrusive_ptr<SceneObject> m_portalEffectObject;     // +184 (0xB8) — the spawned effect object
};

// Golden aarch64 NDK-libc++ layout anchor; MSVC STL member sizes differ,
// so this assert is Android/Linux-only (the Windows build is self-consistent
// under MSVC STL and does not claim golden byte-layout parity).
#ifndef _MSC_VER
static_assert(sizeof(PortalComponent) == 192, "PortalComponent size must be 192 bytes");
#endif

} // namespace Caver

#endif // CAVER_PORTALCOMPONENT_H
