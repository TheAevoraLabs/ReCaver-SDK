// Recovered from libswordigo.so (1.4.13) evidence — Ray3 layout
// pinned by IntersectionWithXYPlane @0x5387C0 (jni-small SF-1):
// origin at +0, direction at +12 (two Vector3s, 24 bytes).
#ifndef CAVER_RAY3_H
#define CAVER_RAY3_H

#include "Vector3.h"

namespace Caver {

struct Ray3 {
    Vector3 m_origin;     // +0
    Vector3 m_direction;  // +12

    Vector3 IntersectionWithXYPlane(float z) const;
};

} // namespace Caver

#endif
