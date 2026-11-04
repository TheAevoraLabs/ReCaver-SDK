// Recovered from libswordigo.so (1.4.13) evidence — Caver::ParticleSystem,
// the engine-side particle system (billboarded quads, one shared texture).
//
// Layout (sizeof 192), from the ctor (0x4E904C), the inlined destructor
// in boost::checked_delete<ParticleSystem> (0x39B028), InitWithMaxParticles
// (0x4E9174), Update (0x4E95B4), Draw (0x4EA0C0) and the checked_delete
// member-teardown order (VAO first, then particles, then types, then
// texture — reverse declaration order):
//   +0   boost::intrusive_ptr<Texture> texture (root/atlas texture,
//        resolved through Texture::SharedPtr of the parent in
//        AddParticleType when the added type's texture is a subtexture)
//   +8   std::vector<ParticleType> types (88-byte elements)
//   +32  int maxParticles
//   +36  int numParticles
//   +40  Particle* particles (operator new[] of 68-byte elements)
//   +48  int vertexSize (0 in ctor, 24 after InitWithMaxParticles)
//   +56  void* indexData  (copy of the VAO's index buffer pointer)
//   +64  void* vertexData (copy of the VAO's vertex buffer pointer)
//   +72  VertexArrayObject vao (72 bytes)
//   +144 bool bufferObjectsCreated
//   +145 bool gravityEnabled
//   +148 Vector3 gravity
//   +160 bool fadeParticles (init true)
//   +164 float fadeSizeStart (init 1.0)
//   +168 FloatColor color (init 1,1,1,1)
//   +184 bool useFloatColor (init false)
//
// Particle (68 bytes), defaults from the InitWithMaxParticles array
// initialisation loop (rodata words 0x259F30 = (0,0,0,1) at +32 and
// 0x25A630 = (1,1,1,0) at +48):
//   +0 int type, +4 Vector3 position, +16 Vector3 velocity,
//   +28 Color color (default white), +32 float rotation,
//   +36 float rotationVelocity, +40 float size (default 0),
//   +44 float endSize (default 1), +48 Vector2 scale (default 1,1),
//   +56 float alpha (default 1), +60 float life (default 0),
//   +64 float age (default 0)
//
// ParticleType (88 bytes), from AddParticleType (0x4E92BC):
//   +0 intrusive_ptr<Texture> texture, +8 Rectangle rect,
//   +24 Vector2 corners[4]  = (x,y), (x+w,y), (x,y+h), (x+w,y+h),
//   +56 Vector2 uvCorners[4] = the texture's UV transform rectangle
//        (offset at Texture+124, offset+scale at Texture+132) in the
//        same corner order.
//
// The vertex format (stride 24) is: Vector3 position (+0), Vector2 uv
// (+12), Color (+20) — see Draw's AddArray calls.
#ifndef CAVER_PARTICLESYSTEM_H
#define CAVER_PARTICLESYSTEM_H

#include <vector>

#include <boost/intrusive_ptr.hpp>

#include "../core/Color.h"
#include "../core/FloatColor.h"
#include "../core/Matrix4.h"
#include "../core/Rectangle.h"
#include "../core/Vector2.h"
#include "../core/Vector3.h"
#include "../render/Texture.h"
#include "../render/VertexArrayObject.h"

namespace Caver {

struct Particle {
    Particle()
        : type(0), rotation(0.0f), rotationVelocity(0.0f), size(0.0f),
          endSize(1.0f), alpha(1.0f), life(0.0f), age(0.0f) {
        position.x = 0.0f; position.y = 0.0f; position.z = 0.0f;
        velocity.x = 0.0f; velocity.y = 0.0f; velocity.z = 0.0f;
        color.r = 255; color.g = 255; color.b = 255; color.a = 255;
        scale.x = 1.0f; scale.y = 1.0f;
    }

    int type;               // +0
    Vector3 position;       // +4
    Vector3 velocity;       // +16
    Color color;            // +28
    float rotation;         // +32
    float rotationVelocity; // +36
    float size;             // +40
    float endSize;          // +44
    Vector2 scale;          // +48
    float alpha;            // +56
    float life;             // +60
    float age;              // +64
};

struct ParticleType {
    boost::intrusive_ptr<Texture> texture; // +0
    Rectangle rect;                        // +8
    Vector2 corners[4];                    // +24
    Vector2 uvCorners[4];                  // +56
};

class RenderingContext;

class ParticleSystem {
public:
    ParticleSystem();

    void InitWithMaxParticles(int maxParticles);
    int AddParticleType(const boost::intrusive_ptr<Texture>& texture,
                        const Rectangle& rect);
    Particle* AddParticle(const Particle& particle);
    void Update(float dt);
    void SetVertex(int index, const Vector3& position, const Vector2& uv,
                   const Color& color);
    void BuildVertexDataWithCamera(const Vector3& cameraPosition,
                                   const Vector3& cameraUp);
    void BuildVertexDataWithCamera(const Vector3& cameraPosition,
                                   const Vector3& cameraUp,
                                   const Matrix4& matrix);
    void BuildVertexDataWithTransform(const Matrix4& matrix);
    void Draw(RenderingContext* context);

    boost::intrusive_ptr<Texture> m_texture; // +0
    std::vector<ParticleType> m_types;       // +8
    int m_maxParticles;                      // +32
    int m_numParticles;                      // +36
    Particle* m_particles;                   // +40
    int m_vertexSize;                        // +48
    void* m_indexData;                       // +56
    void* m_vertexData;                      // +64
    VertexArrayObject m_vao;                 // +72
    bool m_bufferObjectsCreated;             // +144
    bool m_gravityEnabled;                   // +145
    Vector3 m_gravity;                       // +148
    bool m_fadeParticles;                    // +160
    float m_fadeSizeStart;                   // +164
    FloatColor m_color;                      // +168
    bool m_useFloatColor;                    // +184
};

} // namespace Caver

#endif
