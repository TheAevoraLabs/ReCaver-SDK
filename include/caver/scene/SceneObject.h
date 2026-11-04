// Recovered from libswordigo.so (1.4.13) evidence — see docs/CORE_HEADERS.md.
// Scene graph node. Bases and vtable are proven by _ZTV at 0x6339E8
// (160 bytes, two address points): primary 0x6339F8, IBindable
// secondary 0x633A58 with _ZThn16_ thunks; _ZTI is a __vmi_class_type_info
// (56 bytes). Primary slots: 0 D1, 1 D0, 2 Clone() const,
// 3 UpdateBindingDependencies, 4 GetBindings,
// 5 GetEnumValuesForBindedProperty, 6 ValueForBindedProperty,
// 7 SetValueForBindedProperty, 8 PerformBindedAction,
// 9 AddComponent(const intrusive_ptr<Component>&).
// Member offsets proven from function bodies; unnamed regions are
// padding placeholders refined as their accessors are recovered.
#ifndef CAVER_SCENEOBJECT_H
#define CAVER_SCENEOBJECT_H

#include <cstddef>
#include <vector>
#include <boost/intrusive_ptr.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>

#include "../core/Vector2.h"
#include "../core/Vector3.h"
#include "../core/Rectangle.h"

#include "../component/ReferenceCountedObject.h"
#include "../component/IBindable.h"
#include "FastVector.h"
#include "SceneObjectGroup.h"

namespace Caver {

class Component;
class ComponentManager;
class ObjectTemplate;
class Program;
class Scene;
class SceneGridLayer;
class SceneObjectGroup;
struct Matrix4;
namespace Proto { class SceneObject; }

// The object's cell entry in the scene grid (48 bytes, embedded in
// SceneObject at +264 and in CollisionShapeComponent at +440).
// Recovered from the SceneObject / CollisionShapeComponent ctor and
// dtor bodies plus the grid side that consumes it
// (reference/ida-arm64/functions/Caver/SceneGridLayer/):
//   +0  SceneGridLayer* m_layer
//       the slab this entry is registered with, null when unlinked.
//       ~SceneGridEntry @0x4FE688 calls SceneGridLayer::RemoveObject
//       through it when set.
//   +8  SceneObject* m_object
//       the owner (the ctor stores it; the grid queries return this
//       pointer to the caller).
//   +16 char m_collected
//       duplicate-suppression flag. GetEntriesInBounds @0x4FF5D8 and
//       GetAllEntries @0x4FF848 only append an entry when this byte is
//       zero, then set it; SceneGrid::SetBounds and the two object
//       queries clear it again.
//   +20 Rectangle m_bounds
//       the world AABB that was registered (AddObject stores it, and
//       GetEntriesInBounds re-tests it against the query rectangle).
//   +36 unsigned long long m_cellRect
//       the packed cell rectangle GridRectForAABB returns:
//       [0:16) minX, [16:32) maxX, [32:48) minY, [48:64) maxY. The
//       golden stores/loads it with 8-byte unaligned accesses at +36
//       (stur/ldur), so the field is packed inside the otherwise
//       naturally-aligned struct; sizeof stays 48.
struct SceneGridEntry {
    SceneGridLayer* m_layer;        // +0
    SceneObject* m_object;          // +8 — the owner (ctor stores it)
    char m_collected;               // +16 — grid query duplicate flag
    char _pad17[3];                 // +17
    Rectangle m_bounds;             // +20 — registered world AABB
    unsigned long long m_cellRect __attribute__((packed)); // +36

    SceneGridEntry()
        : m_layer(0), m_object(0), m_collected(0), _pad17{0, 0, 0},
          m_bounds(), m_cellRect(0) {}
    explicit SceneGridEntry(SceneObject* object)
        : m_layer(0), m_object(object), m_collected(0), _pad17{0, 0, 0},
          m_bounds(), m_cellRect(0) {}
    ~SceneGridEntry();
};

static_assert(sizeof(SceneGridEntry) == 0x30, "sizeof must be 0x30");
static_assert(offsetof(SceneGridEntry, m_bounds) == 0x14,
              "m_bounds must be at 0x14");
static_assert(offsetof(SceneGridEntry, m_cellRect) == 0x24,
              "m_cellRect must be at 0x24");

class SceneObject : public ReferenceCountedObject, public IBindable {
public:
    SceneObject();
    virtual ~SceneObject();

    virtual SceneObject* Clone() const;                                     // slot 2 — raw-pointer return CONFIRMED (Component::Clone's base body returns 0 in x0, no sret)
    void UpdateBindingDependencies();                                       // slot 3 (IBindable)
    void GetBindings(std::vector<Binding>* bindings);                       // slot 4 (IBindable)
    void GetEnumValuesForBindedProperty(int property, std::vector<BindingValue>* values); // slot 5
    BindingValue ValueForBindedProperty(int property);                      // slot 6
    void SetValueForBindedProperty(int property, const BindingValue& value); // slot 7
    void PerformBindedAction(int action);                                   // slot 8
    virtual void AddComponent(const boost::intrusive_ptr<Component>& component); // slot 9
    void AddComponent(Component* component);    // raw-pointer overload (golden 0x500514, direct-called by HealthComponent's gameplay bodies)

    // Non-virtual, recovered:
    void Process();
    void LoadFromProtobufMessage(const Proto::SceneObject& message);
    void SaveToProtobufMessage(Proto::SceneObject* message) const;
    void SetScene(Scene* scene);
    void SetIdentifier(const std::string& identifier);
    void FinishLoad();
    void Update(float deltaTime, bool paused, bool editorMode);
    bool HandleMessage(int message, void* data);   // golden returns whether a component handled it
    Component* ComponentWithIdentifier(int identifier) const;
    Component* ComponentWithInterface(long interfaceId) const;
    bool HasVisualComponent() const;
    bool isHidden() const;
    bool IsInHiddenGroup() const;
    bool IsInLockedGroup() const;
    bool HasGroup(SceneObjectGroup* group) const;
    void AddGroup(SceneObjectGroup* group);
    void RemoveGroup(SceneObjectGroup* group);
    float updateSpeedMultiplier() const;
    void GetAllComponents(std::vector<boost::intrusive_ptr<Component>>* components) const;
    void GetTemplateComponents(std::vector<boost::intrusive_ptr<Component>>* components) const;
    void GetInstanceComponents(std::vector<boost::intrusive_ptr<Component>>* components) const;
    void RegisterComponentInterfaces(ComponentManager* manager);
    void GetComponentsWithInterface(long interfaceId, std::vector<Component*>* components);
    template<typename T>
    void GetComponentsWithInterface(std::vector<boost::intrusive_ptr<T>>* components);
    template<typename T>
    void GetComponentsWithInterface(std::vector<T*>* components);
    void AddChildObject(const boost::intrusive_ptr<SceneObject>& child);
    void RemoveChildObject(SceneObject* child, bool keepWorldPosition);
    void RemoveFromAllGroups(bool keepWorldPosition);
    void RemoveAllComponents();
    void RemoveComponentWithIdentifier(int identifier);
    void RemoveAllComponentsWithInterface(long interfaceId);
    SceneObject* UpdateBounds();
    // Declared for the PhysicsObjectState slice (systems): both are
    // proven callees of PhysicsObjectState functions in the golden
    // binary (RegisterForWorldBoundsUpdate @0x503B2C,
    // VelocityAtWorldPoint @0x503E28); bodies belong to this slice.
    void RegisterForWorldBoundsUpdate();
    Vector2 VelocityAtWorldPoint(const Vector2& point);
    // Transform/bounds set (recovered run 3 from the golden bodies):
    Matrix4 WorldMatrix() const;
    Vector2 WorldPointFromLocalPoint(const Vector2& point);
    Vector2 LocalPointFromWorldPoint(const Vector2& point);
    Vector2 WorldDirectionFromLocalDirection(const Vector2& direction);
    Vector2 LocalDirectionFromWorldDirection(const Vector2& direction);
    bool ContainsPoint(const Vector2& point);
    void setLocalAABB(const Rectangle& rect);
    void ExpandBoundsToIncludeLocalAABB(const Rectangle& rect);
    void Activate();
    void SetAlwaysActive(bool alwaysActive);
    // Bodies belong to this slice (declared for HealthComponent::Update):
    void InitWithTemplate(const boost::intrusive_ptr<ObjectTemplate>& objectTemplate);
    void Templatize(const std::string& name);
    void UpdateFromTemplate();
    void setPosition(const Vector2& position);
    void SetInstanceScaling(float scaling);
    void UnlinkFromTemplate();

    bool m_flag24;                                      // +24 — true in ctor (the ctor's word store at +24 sets +24=1, +25=0; semantics UNRECOVERED)
    bool m_field25;                                     // +25 — "in scene" flag: false in ctor, SetScene sets it on attach and clears it on detach (name UNRECOVERED)
    char _pad26[6];                                     // +26
    Scene* m_scene;                                     // +32
    boost::scoped_ptr<FastVector<SceneObjectGroup*>> m_groups; // +40 — lazily allocated group list; scoped: ~SceneObject's teardown ends with the pointee's delete[] + delete (member-teardown position, after m_childGroup)
    SceneObject* m_parent;                              // +48
    boost::intrusive_ptr<SceneObjectGroup> m_childGroup; // +56 — the group holding this object and its children (created by AddChildObject)
    float m_speedMultiplier;                            // +64 — 1.0f in ctor
    char m_field68;                                     // +68 — byte zeroed by the ctor (UNRECOVERED)
    char _pad69[3];                                     // +69
    boost::intrusive_ptr<ObjectTemplate> m_template;    // +72
    std::string m_identifier;                           // +80 — the string form (SetIdentifier; the int form lives on components)
    boost::shared_ptr<Program> m_onLoad;                // +104 — the "onLoad" binding's program
    Vector2 m_velocity;                                 // +120 — VelocityAtWorldPoint's base term
    Vector3 m_position;                                 // +128 (z at +136)
    char _pad140[4];                                    // +140
    float m_rotation;                                   // +144
    float m_angularVelocity;                            // +148
    float m_instanceScaling;                            // +152 — SetInstanceScaling's input
    float m_scale;                                      // +156 — effective scale (instanceScaling × the template's +320 factor); the scalar every transform uses
    bool m_flipped;                                     // +160 — x-flip flag for the point/direction transforms
    char _pad161[3];                                    // +161
    Rectangle m_localAABB;                              // +164 — (-10, -10, 20, 20) in ctor (a .rodata constant in the golden)
    Rectangle m_worldAABB;                              // +180 — zeroed in ctor (name UNRECOVERED; the region is real member space, zeroed by two qword stores)
    bool m_field196;                                    // +196 — set once registered for a world-bounds update (setPosition/setLocalAABB guard)
    char _pad197[3];                                    // +197
    float m_field200;                                   // +200 — min depth, maintained by UpdateBounds from components' minDepth()
    float m_field204;                                   // +204 — max depth, maintained by UpdateBounds from components' maxDepth()
    std::vector<boost::intrusive_ptr<Component>> m_components; // +208 — sorted by component identifier (AddComponent inserts in order)
    FastVector<Component*> m_updateComponents;          // +232 — raw mirror of the RequiresUpdate components
    bool m_field248;                                    // +248 — set when detached keeping world position (UNRECOVERED name)
    char _pad249[7] = {0, 0, 0, 0, 0, 0, 0};            // +249 — zeroed in ctor (the golden's dword stores at +252/+255)
    bool m_field256;                                    // +256 — Activate's gate (UNRECOVERED name)
    bool m_alwaysActive;                                // +257
    char _pad258;                                       // +258
    bool m_flag259;                                     // +259 — true in ctor (semantics UNRECOVERED)
    bool m_hidden;                                      // +260
    char _pad261[3];                                    // +261
    SceneGridEntry m_gridEntry;                         // +264 (48 bytes) — the embedded grid entry; its ctor stores the owner pointer at +272
};

} // namespace Caver

#endif
