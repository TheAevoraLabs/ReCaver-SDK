// First-party 2D/2.5D Raycast Engine for CaverEngine.
// High-performance spatial grid raycaster and line-of-sight query system.
// Credit: Quantum Creeper / Aevora Labs.

#ifndef CAVER_PHYSICS_RAYCAST_H
#define CAVER_PHYSICS_RAYCAST_H

#include "../core/Vector2.h"
#include <cstdint>

namespace Caver {

class Scene;
class SceneObject;
class CollisionShapeComponent;

enum RaycastFilter {
    RAYCAST_FILTER_NONE     = 0,
    RAYCAST_FILTER_GROUND   = 1 << 0, // Collide against static terrain/polygons (m_isGround == true)
    RAYCAST_FILTER_ENTITIES = 1 << 1, // Collide against monsters, players, breakables, interactive objects
    RAYCAST_FILTER_ALL      = RAYCAST_FILTER_GROUND | RAYCAST_FILTER_ENTITIES
};

struct RaycastHit2D {
    bool hit = false;
    Vector2 point{0.0f, 0.0f};
    Vector2 normal{0.0f, 0.0f};
    float distance = 0.0f;
    float fraction = 1.0f; // 0.0 .. 1.0 along the ray
    SceneObject* object = nullptr; // nullptr if ground or non-object
    CollisionShapeComponent* component = nullptr;
};

class RaycastEngine {
public:
    // Casts a 2D ray from start to end through the scene's spatial grid.
    // Finds the closest intersection with terrain, entities, or both based on filter.
    // Returns true if an intersection was found.
    static bool CastRay(Scene* scene,
                        const Vector2& start,
                        const Vector2& end,
                        RaycastHit2D* outHit,
                        uint32_t filter = RAYCAST_FILTER_ALL,
                        SceneObject* ignoreObject = nullptr,
                        float minDepth = -100.0f,
                        float maxDepth = 100.0f);

    // Fast boolean line-of-sight query from start to end.
    // Early-outs immediately on the first obstructing collider without sorting.
    // Returns true if the path is completely clear, false if obstructed.
    static bool LineOfSight(Scene* scene,
                            const Vector2& start,
                            const Vector2& end,
                            SceneObject* ignoreObject = nullptr,
                            uint32_t filter = RAYCAST_FILTER_GROUND,
                            float minDepth = -100.0f,
                            float maxDepth = 100.0f);
};

} // namespace Caver

#endif // CAVER_PHYSICS_RAYCAST_H
