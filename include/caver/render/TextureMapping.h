// Recovered from libswordigo.so (1.4.13) evidence — layout pinned by
// TexCoordForPosition @0x5550E4 (jni-small SF-1):
//   +0  Texture* m_texture (raw; its m_parent at Texture+144 gates
//       the subtexture coordinate conversion)
//   +8  float m_field8 — UNRECOVERED name: the divisor ("scale");
//       |scale| < 0.001 returns the zero vector
//   +12 Vector2 m_field12 — UNRECOVERED name: subtracted from the
//       position before dividing ("origin"/"offset")
#ifndef CAVER_TEXTUREMAPPING_H
#define CAVER_TEXTUREMAPPING_H

#include "../core/Vector2.h"
#include "../core/Vector3.h"

namespace Caver {

class Texture;

class TextureMapping {
public:
    Vector2 TexCoordForPosition(const Vector3& position) const;

    Texture* m_texture;  // +0
    float m_field8;      // +8
    Vector2 m_field12;   // +12
};

} // namespace Caver

#endif
