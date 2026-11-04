// Recovered from libswordigo.so (1.4.13) evidence — layouts pinned by
// PointLightGroup::CombinedLight @0x4F519C and PointLightManager::
// AddLight @0x4F4768 (jni-small SF-1).
// PointLight is a 60-byte POD (AddLight copies it as 60 bytes;
// CombinedLight reads the fields at the offsets below). Field NAMES
// are UNRECOVERED beyond their roles in CombinedLight:
//   +0  float[4] — first three components are summed weighted by the
//       +44 weight; the +12 component is a packed value summed as raw
//       float bits (CombinedLight never multiplies it by the weight)
//   +16 float[4] — summed unweighted
//   +32 float[2] — summed unweighted
//   +40 float    — summed unweighted
//   +44 float    — the weight (intensity); CombinedLight stores the
//                  weight total into the result's +44
//   +48 float    — never read/written by CombinedLight
//   +52 float    — result takes the minimum (init FLT_MAX)
//   +56 float    — result takes the minimum (init FLT_MAX)
#ifndef CAVER_POINTLIGHTGROUP_H
#define CAVER_POINTLIGHTGROUP_H

#include <boost/shared_ptr.hpp>

#include "../scene/FastVector.h"

namespace Caver {

struct Rectangle;

struct PointLight {
    float m_field0[4];   // +0
    float m_field16[4];  // +16
    float m_field32[2];  // +32
    float m_field40;     // +40
    float m_field44;     // +44 (weight/intensity)
    float m_field48;     // +48
    float m_field52;     // +52 (min-combined)
    float m_field56;     // +56 (min-combined)
};

// A FastVector<PointLight>-shaped group: count +0, capacity +4,
// data +8 (AddLight's FastVector<PointLight> use proves the shape).
// Groups are heap objects owned by a single boost::shared_ptr (see
// GetOptimizedLights); they are never value-copied, so the destructor
// freeing the data array is safe. The default constructor zeroes the
// fields, matching the golden's zeroed 16-byte group allocation.
class PointLightGroup {
public:
    PointLightGroup();
    ~PointLightGroup();

    PointLight CombinedLight() const;

    int m_count;          // +0
    int m_capacity;       // +4
    PointLight* m_data;   // +8
};

// PointLightManager (golden ctor @0x4F475C, AddLight @0x4F4768,
// dtor @0x4FAAA8): 32 bytes, two FastVectors. The lights vector is
// the base (count +0, capacity +4, data +8 -- AddLight passes the
// manager itself as the FastVector<PointLight> to Resize); the
// groups vector sits at +16 (count +16, capacity +20, data +24).
// The ctor zeroes all 32 bytes. AddLight grows like the golden:
// if count >= capacity, Resize to 2*capacity floored at 10, then
// copy the 60-byte record and bump count. The dtor releases the
// +24 groups vector first (each shared_ptr released), then deletes
// the +8 lights array (member destruction order: groups, then base).
// One deliberate divergence: the golden FastVector<shared_ptr>
// allocates its array with an 8-byte header at data-8 (capacity
// stored there, used by its release loop); the in-tree generic
// FastVector uses plain new[]/delete[], so the header is not
// reproduced. Observable behaviour is identical (every shared_ptr
// released, array freed), and no in-tree caller populates the groups
// vector yet -- GetOptimizedLights, its only golden user, is
// unimplemented and has zero callers in the golden either. This
// class replaces the duplicated TU-local PointLightData /
// PointLightManager copies that scene/Scene.cpp and
// render/LightOverlay.cpp used to carry.
class PointLightManager : public FastVector<PointLight> {
public:
    PointLightManager();
    ~PointLightManager();
    void AddLight(const PointLight& light);
    // Golden @0x4F47F4, standalone: zero callers in the golden binary
    // and zero callers in-tree (Scene feeds LightOverlay directly).
    // Implemented from the IDA body with no new call path added.
    int GetOptimizedLights(const Rectangle& rect, PointLight* out, int maxCount);

private:
    FastVector<boost::shared_ptr<PointLightGroup> > m_groups;  // +16
};

static_assert(sizeof(PointLight) == 60, "PointLight sizeof must be 60 bytes");
static_assert(sizeof(PointLightManager) == 32,
              "PointLightManager sizeof must be 32 bytes");

} // namespace Caver

#endif
