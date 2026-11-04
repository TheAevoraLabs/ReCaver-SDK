// Recovered from libswordigo.so (1.4.13) evidence.
// The scene graph root: owns the root ProgramState, the object and
// group registries, two ComponentManagers, the render grids, and
// per-frame update/draw. Layout anchors (ctor @0x4F5940, dtor,
// accessors):
//   +0   vptr (off_6336A0) — the class is polymorphic
//   +8   byte, 0 in ctor (semantics UNRECOVERED)
//   +9   bool, 1 in ctor — gates child ProgramState creation
//        (SceneObjectGroup::FinishLoad, Component::NewProgramState-
//        ForProgram read it)
//   +16  boost::shared_ptr<GameState> (proven by ~Scene + the ctor
//        cleanup path; see the member declaration)
//   +32  int pause counter — SetPaused(bool) increments/decrements
//        it (NOT a plain bool)
//   +40  ProgramState root state, embedded by value (96 bytes)
//   +136 16 bytes UNRECOVERED (zeroed by the ctor)
//   +152 Rectangle bounds (ctor: {-2000, -1000, 4000, 2000};
//        SetBounds replaces degenerate sizes with {0, 0, 1000, 1000})
//   +168 boost::shared_ptr<ObjectLibrary>
//   +184 map<string, intrusive_ptr<SceneObject>> objects by identifier
//   +208 map<string, intrusive_ptr<SceneObjectGroup>> groups by id
//   +232 std::list, element type UNRECOVERED
//   +256 boost::shared_ptr, pointee UNRECOVERED
//   +272 std::map, key/value UNRECOVERED
//   +296 list<intrusive_ptr<Component>> new components
//   +320 list<intrusive_ptr<SceneObject>> objects waiting activation
//   +344 ComponentManager #1 (48 bytes; hero/dimension/portal set)
//   +392 ComponentManager #2 (48 bytes; the render/update set)
//   +440 boost::shared_ptr<Camera> (TouchBegan calls through it)
//   +456 int, -1 in ctor (UNRECOVERED)
//   +464 LightOverlay (render/LightOverlay.h)
//   +648 frame counter (64-bit, zeroed by the ctor)
//   +664 int, 0 in ctor (UNRECOVERED)
//   +676 Rectangle visible rect (fallback for IsObjectVisible when an
//        object has no context rect at its +264 link's +56)
//   +696 vector<intrusive_ptr<SceneObject>> world-bounds update list
//   +720 byte, 0 in ctor (UNRECOVERED)
//   +728 SceneGrid #1 (64 bytes; the near-depth grid, slabs -300..300
//        and -3000..-300)
//   +792 SceneGrid #2 (64 bytes; the object/shape grid, slab 0..0)
//   +856 tail: 16 bytes + bytes at +872/+892 (UNRECOVERED)
#ifndef CAVER_SCENE_H
#define CAVER_SCENE_H

#include <list>
#include <map>
#include <string>
#include <vector>
#include <boost/intrusive_ptr.hpp>
#include <boost/shared_ptr.hpp>

#include "../component/ComponentManager.h"
#include "../core/FloatColor.h"
#include "../core/Rectangle.h"
#include "../render/LightOverlay.h"
#include "../scripting/ProgramState.h"
#include "FastVector.h"
#include "../physics/CollisionPairSet.h"
#include "SceneGrid.h"
#include "SceneObjectGroup.h"

namespace Caver {

class Camera;
class CollisionShapeComponent;
class Component;
class GameState;
class ComponentManager;
class ObjectLibrary;
class Program;
class RenderingContext;
class SceneObject;
class SceneObjectGroup;
class SceneProcessingContext;
struct FWTouch;
struct LineSegment;
struct Matrix4;

namespace Proto { class Scene; }

// LightOverlay (184 bytes at Scene+464) is recovered in
// render/LightOverlay.h — the darkening quad drawn after the opaque pass.
// Shadow vertex format proven by Scene::Draw's attribute pointers
// (stride 24: position 3 floats at +0, texcoord 2 floats at +12,
// color 4 bytes at +20). Full recovery belongs to the shadow work.
struct ShadowVertex {
    float m_position[3];        // +0
    float m_texcoord[2];        // +12
    unsigned char m_color[4];   // +20
};

class Scene {
public:
    Scene();
    virtual ~Scene();

    void SetPaused(bool paused);
    bool TouchBegan(const FWTouch& touch);
    void TouchMoved(const FWTouch& touch);
    void TouchEnded(const FWTouch& touch);
    void TouchCancelled(const FWTouch& touch);

    // Virtual set and order proven from the golden's vtable at
    // 0x6336A0: ~Scene, RegisterObject, Update, Draw, AddObject,
    // RemoveObject, SetBounds (slots 0-7).
    virtual void RegisterObject(SceneObject* object);
    void RegisterNewComponent(const boost::intrusive_ptr<Component>& component);
    void RegisterObjectWaitingForActivation(const boost::intrusive_ptr<SceneObject>& object);
    void RegisterObjectForWorldBoundsUpdate(const boost::intrusive_ptr<SceneObject>& object);

    void Process(SceneProcessingContext* context);
    void FinishLoad();
    virtual void Update(float deltaTime);
    virtual void Draw(RenderingContext* context, const Matrix4& transform);
    bool LineSegmentIntersectsGround(const LineSegment& segment, float minDepth,
                                     float maxDepth, Vector2* intersection);
    void RegisterLibrary();
    bool LoadFromFile(const std::string& filename);
    bool SaveToFile(const std::string& filename);
    void LoadFromProtobufMessage(const Proto::Scene& message);
    void SaveToProtobufMessage(Proto::Scene* message) const;
    void UpdateObjectWorldBounds(const boost::intrusive_ptr<SceneObject>& object);
    void UpdateCollisionShapeWorldBounds(CollisionShapeComponent* component);
    bool IsObjectVisible(SceneObject* object);
    void GetAllObjects(std::vector<boost::intrusive_ptr<SceneObject>>* objects);
    void GetAllGroups(std::vector<boost::intrusive_ptr<SceneObjectGroup>>* groups);

    boost::intrusive_ptr<SceneObject> ObjectWithIdentifier(const std::string& identifier);
    void GroupIdentifierChanged(const std::string& oldIdentifier,
                                const std::string& newIdentifier);
    void ObjectIdentifierChanged(const std::string& oldIdentifier,
                                 const std::string& newIdentifier);
    virtual void AddObject(const boost::intrusive_ptr<SceneObject>& object);
#if defined(CAVER_DESKTOP_EXPERIMENTAL) || defined(CAVER_STENCIL_FIX)
    // Stencil volumes (experimental/stencilfix only): attach a
    // ShadowVolumeComponent to every model-owning object so the
    // Scene::Draw stencil section has something to draw. Code-only
    // enablement: no scene, Lua, factory, or API change. The full walk
    // runs at load; the per-object form runs at first activation to
    // catch objects spawned after load (e.g. the hero). Both are
    // idempotent: objects that already carry a volume are skipped.
    void AttachStencilVolumes();
    void AttachStencilVolumeTo(const boost::intrusive_ptr<SceneObject>& object);
#endif
    void ActivateObject(const boost::intrusive_ptr<SceneObject>& object);
    virtual void RemoveObject(const boost::intrusive_ptr<SceneObject>& object, bool removeChildren);
    virtual void SetBounds(const Rectangle& bounds);
    void AddGroup(const boost::intrusive_ptr<SceneObjectGroup>& group);
    void RemoveGroup(const boost::intrusive_ptr<SceneObjectGroup>& group);
    boost::intrusive_ptr<SceneObjectGroup> GroupWithIdentifier(const std::string& identifier,
                                                               bool createIfMissing);
    std::string MakeUniqueObjectIdentifier(const std::string& identifier);

private:
    void DrawGroundMeshes(RenderingContext* context, const Matrix4& transform, bool drawFlag);
    void DrawModels(RenderingContext* context, const Matrix4& transform, bool flag1, bool flag2);

public:
    char _pad8 = 0;                                     // +8
    bool m_childStatesEnabled = true;                   // +9
    bool m_flag10 = false;                              // +10 — ActivateObject: activate even when hidden
    char _pad11 = 0;                                    // +11
    // +16: boost::shared_ptr<GameState> — proven by ~Scene, whose last
    // release is the shared_ptr at +16/+24, and by the ctor cleanup
    // path instantiating ~shared_ptr<GameState> in this TU.
    boost::shared_ptr<GameState> m_gameState;           // +16
    int m_pauseCount = 0;                               // +32
    char _pad36[4] = {0, 0, 0, 0};                      // +36
    ProgramState m_rootState;                           // +40
    boost::shared_ptr<Program> m_onLoad;                // +136 (set by LoadFromProtobufMessage)
    Rectangle m_bounds;                                 // +152 (assigned in the ctor body)
    boost::shared_ptr<ObjectLibrary> m_objectLibrary;   // +168
    std::map<std::string, boost::intrusive_ptr<SceneObject>> m_objectsByIdentifier;       // +184
    std::map<std::string, boost::intrusive_ptr<SceneObjectGroup>> m_groupsByIdentifier;  // +208
    std::list<boost::intrusive_ptr<SceneObject>> _list232;                               // +232 (element type UNRECOVERED)
    // +256: hand-rolled growable array of CollisionShapeComponent* —
    // Scene::Update's per-frame collision candidate set (proven by
    // Update's growth code and ~Scene's delete[] of the data buffer).
    int _collisionSetCount = 0;                         // +256
    int _collisionSetCapacity = 0;                      // +260
    void** _collisionSetData = 0;                       // +264
    // Real CollisionPairSet member, not a char placeholder: the
    // placeholder was never constructed, so the first
    // RegisterCollision operated on a null begin node and faulted
    // (see CollisionPairSet.h). Same 24 bytes at +272, so the layout
    // is unchanged.
    CollisionPairSet _collisionPairSet;                 // +272
    std::list<boost::intrusive_ptr<Component>> m_newComponents;                          // +296
    std::list<boost::intrusive_ptr<SceneObject>> m_objectsWaitingForActivation;          // +320
    ComponentManager m_componentManager1;               // +344
    ComponentManager m_componentManager2;               // +392
    boost::shared_ptr<Camera> m_camera;                 // +440
    int _int456 = -1;                                   // +456
    char _pad460[4] = {0, 0, 0, 0};                     // +460
    LightOverlay m_lightOverlay;                        // +464
    int m_frameCounter = 0;                             // +648
    Rectangle _rect652;                                 // +652 (visible rect outset by 200; written by Update)
    float _zRange668[2] = {-300.0f, 300.0f};            // +668 (camera AABB z range; rewritten by Update)
    Rectangle m_visibleRect = {0, 0, 0, 0};               // +676
    char _pad692[4] = {};                               // +692
    std::vector<boost::intrusive_ptr<SceneObject>> m_objectsForWorldBoundsUpdate;        // +696
    char _byte720 = 0;                                  // +720
    char _pad721[7] = {};                               // +721
    SceneGrid m_grid1;                                  // +728
    SceneGrid m_grid2;                                  // +792
    FastVector<ShadowVertex> m_shadowVertices;          // +856
    bool _lightOverride872 = false;                     // +872
    FloatColor _lightColor876;                          // +876 (set by OverrideLights; not initialised by the ctor)
    char _byte892 = 0;                                  // +892
    char _pad893[3] = {};                               // +893
};

} // namespace Caver

#endif
