// Recovered from libswordigo.so (1.4.13) evidence — Caver::WeaponTrail,
// the ribbon geometry behind WeaponTrailComponent (triangle strip of
// segments that fade out over time).
//
// Layout (sizeof 64), from the ctor (0x3B4014), InitWithMaxSegments
// (0x3B4040), Clear (0x3B4098), AddSegment (0x3B40A4),
// SetVertex (0x3B4470), FadeOut/FadeIn (0x3B45A4/0x3B45B4),
// Update (0x3B45C0), Draw (0x3B4794) and the destructor (0x3B4920):
//   +0   int maxSegments
//   +4   int numSegments
//   +8   Color color (init 255,255,255,0)
//   +12  float opacity (init 1.0; final multiplier applied to every
//        vertex color written by AddSegment/SetVertex)
//   +16  boost::intrusive_ptr<Texture> texture
//   +24  WeaponTrailVertex* vertices (array of 2 per segment)
//   +32  int vertexSize (32, set by InitWithMaxSegments; the ctor does
//        NOT initialise it)
//   +36  bool fadingOut
//   +40  float fadeOutTime (countdown set by FadeOut; not initialised
//        by the ctor)
//   +44  float lifeTime (init 0.0 = disabled; when > 0.001, Update also
//        decays vertex life by dt/lifeTime)
//   +48  float fadeInProgress (init 1.0; FadeIn resets it to 0 and
//        Update grows it back to 1 over fadeInTime; it scales the
//        color of segments added while fading in, and Clear and a
//        completed fade-out reset it to 0)
//   +52  float fadeInTime (init 1.0)
//   +56  float uvX (init 0.0; written as the U coordinate of both
//        vertices of every added segment; V is 0 for the first
//        vertex of a segment and 1 for the second)
//
// WeaponTrailVertex (32 bytes; vertexSize/stride in Draw):
//   +0  Vector3 position
//   +12 Color color (the displayed color; Update rewrites it from
//       baseColor scaled by the vertex's remaining life)
//   +16 Vector2 uv
//   +24 Color baseColor (the color the segment was added with)
//   +28 float life (starts at 1.0, decays in Update; segments whose
//       vertices have died are dropped from the beginning)
#ifndef CAVER_WEAPONTRAIL_H
#define CAVER_WEAPONTRAIL_H

#include <boost/intrusive_ptr.hpp>

#include "../core/Color.h"
#include "../core/Vector2.h"
#include "../core/Vector3.h"
#include "../core/Matrix4.h"
#include "../render/Texture.h"

namespace Caver {

struct WeaponTrailVertex {
    Vector3 position; // +0
    Color color;      // +12
    Vector2 uv;       // +16
    Color baseColor;  // +24
    float life;       // +28
};

class RenderingContext;

class WeaponTrail {
public:
    WeaponTrail();
    ~WeaponTrail();

    void InitWithMaxSegments(int maxSegments);
    void Clear();
    void AddSegment(const Vector3& point1, const Vector3& point2,
                    float t);
    void RemoveSegmentsFromBeginning(int count);
    void SetVertex(int index, const Vector3& position,
                   const Color& color, const Vector2& uv);
    void FadeOut(float time);
    void FadeIn(float time);
    void Update(float dt);
    void Draw(RenderingContext* context, const Matrix4& matrix) const;

    int m_maxSegments;                       // +0
    int m_numSegments;                       // +4
    Color m_color;                           // +8
    float m_opacity;                         // +12
    boost::intrusive_ptr<Texture> m_texture; // +16
    WeaponTrailVertex* m_vertices;           // +24
    int m_vertexSize;                        // +32
    bool m_fadingOut;                        // +36
    float m_fadeOutTime;                     // +40
    float m_lifeTime;                        // +44
    float m_fadeInProgress;                  // +48
    float m_fadeInTime;                      // +52
    float m_uvX;                             // +56
};

} // namespace Caver

#endif
