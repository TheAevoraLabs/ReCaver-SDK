// Recovered from libswordigo.so (1.4.13) evidence — vtable
// _ZTVN5Caver27ElevatorControllerComponentE at 0x623A20 (primary
// address point 0x623A30, IBindable secondary 0x623B48, IUpdateable
// secondary 0x623B88). Slot 3 keeps the BASE InitWithComponent, so
// InitWithComponent below is a non-virtual narrowed overload (the
// DoorController / BushController pattern).
//
// sizeof 0xC8 (200 bytes) is proven by the two allocation sites,
// operator new(0xC8u) in Create() @0x350318 and Clone() @0x3A248C.
//
// Member map, from the ctor store sequence @0x3A0FB4, the destructor
// teardown @0x3A2338, Prepare @0x3A1790, StartElevating @0x3A19D4 and
// the Lua helpers @0x3A1FB4 / @0x3A20A0:
//   +104 ComponentOutlet<UtilityShapeComponent> elevationShape
//        (vtable off_620ED0 = _ZTV ..ComponentOutlet<UtilityShapeComponent>;
//         BindOutlet(0, "elevationShape", ..., true))
//   +128 Rectangle travel bounds — localAABB() returns these 16 bytes
//   +144 int mode (0 = inactive, 1 = wait, 2 = continuous; the proto
//        enum is 1-based, see Load/Save below)
//   +148 int state (0 = idle, 1 = elevating, 2 = returning)
//   +152 bool m_hasOriginalPosition
//   +156 Vector2 m_originalPosition
//   +164 Vector2 m_elevationOffset — travel offset added to the
//        original position to build the target
//   +172 float m_speed (ctor 180.0f)
//   +176 Vector2 m_targetPosition
//   +184 8 bytes, UNRECOVERED: no function in 0x3A0FB4..0x3A2590
//        reads or writes them (disassembly of the whole range has no
//        [x, #0xb8] / #0xbc access) and the ctor leaves them
//        untouched; they exist only to pad the object out to 0xC8
//   +192 float, name UNRECOVERED (running timer, cleared while the
//        float at +196 is >= 0.1)
//   +196 float, name UNRECOVERED (ctor FLT_MAX; HandleMessage resets it
//        to 0 whenever the hero contacts the elevator)
#ifndef CAVER_ELEVATORCONTROLLERCOMPONENT_H
#define CAVER_ELEVATORCONTROLLERCOMPONENT_H

#include <string>
#include <vector>

#include "Component.h"
#include "ComponentOutlet.h"
#include "UtilityShapeComponent.h" // outlet target
#include "../core/Rectangle.h"
#include "../core/Vector2.h"
#include "../physics/CollisionShape.h"

namespace Caver {

class ElevatorControllerComponent : public Component {
public:
    ElevatorControllerComponent();
    virtual ~ElevatorControllerComponent();

    static ElevatorControllerComponent* Create();
    static long Interface();

    // 0 = inactive, 1 = wait, 1 second of hero contact before coming
    // back down, 2 = continuous shuttling. The names come from the
    // mode string table at 0x64FF28 ("inactive" / "wait" /
    // "continuous"); the enumerator SPELLINGS are UNRECOVERED.
    enum Mode {
        ModeInactive = 0,
        ModeWait = 1,
        ModeContinuous = 2,
    };

    // Runtime phase, stored at +148.
    enum State {
        StateIdle = 0,
        StateElevating = 1,
        StateReturning = 2,
    };

    virtual ElevatorControllerComponent* Clone() const;
    void InitWithComponent(const ElevatorControllerComponent* other);

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
    virtual bool RequiresUpdate() const;
    virtual void Update(float deltaTime);
    virtual bool HandleMessage(int message, void* data);
    virtual bool HasBounds() const;
    virtual Rectangle localAABB() const;
    virtual std::string className() const;

    void StartElevating();
    bool IsAtTargetPosition();
    void ReturnToOriginalPosition();

    ComponentOutlet<UtilityShapeComponent> m_elevationShape; // +104
    Rectangle m_travelBounds;                               // +128
    int m_mode;                                             // +144
    int m_state;                                            // +148
    bool m_hasOriginalPosition;                             // +152
    char _pad153[3];                                        // +153
    Vector2 m_originalPosition;                             // +156
    Vector2 m_elevationOffset;                              // +164
    float m_speed;                                          // +172
    Vector2 m_targetPosition;                               // +176
    char _unrecovered184[8];                                // +184
    float m_field192;                                       // +192
    float m_field196;                                       // +196
};

static_assert(sizeof(ElevatorControllerComponent) == 0xC8,
              "ElevatorControllerComponent sizeof must be 200 bytes");

} // namespace Caver

#endif // CAVER_ELEVATORCONTROLLERCOMPONENT_H
