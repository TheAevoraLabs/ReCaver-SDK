// Recovered from libswordigo.so (1.4.13) evidence - per-function IDA exports
// in reference/ida-arm64/functions/Caver/LightOverlay/ checked against the raw
// arm64 disassembly:
//   ctor            0x4EE490   SetFrame           0x4EE4F0
//   CreateVertices  0x4EE5FC   SetPointLights     0x4EE82C
//   UpdateWithCamera 0x4EE8E4  Draw               0x4EEE7C
//   dtor            0x4F5F38
//
// The overlay is the golden's post-opaque-pass darkening quad: a screen-space
// triangle grid the size of the visible rectangle, one vertex per cell corner,
// whose per-vertex alpha is clamp((1 - accumulatedPointLight) * intensity) *
// overlayColour.a. With no point lights in range every vertex is the overlay
// colour at full intensity, which is how the black silhouettes in the menu
// scene are produced (Scene::Draw runs it before the shadow volumes and the
// emissive glow pass).
//
// Layout (184 bytes; every offset below is read or written by the bodies):
//   +0   Rectangle frame (the visible rect the grid covers)
//   +16  int grid width  (rows of the vertex grid, along the frame's y)
//   +20  int grid height (columns of the vertex grid, along the frame's x)
//   +24  int cell height (ceil(frame.height / (gridWidth - 1)))
//   +28  int cell width  (ceil(frame.width  / (gridHeight - 1)))
//   +32  float overlay intensity
//   +36  unsigned char[4] overlay colour, r,g,b,a
//   +40  float* per-cell accumulated point light (gridWidth * gridHeight)
//   +48  int triangle count
//   +56  void* vertex data (mirrors the VAO's m_vertexData at +80)
//   +64  void* index data  (mirrors the VAO's m_indexData  at +96)
//   +72  VertexArrayObject (72 bytes)
//   +144 unsigned char "array descriptors created" latch
//   +148 int point light count
//   +152 PointLight* point lights (60-byte records, copied by SetPointLights)
//   +160 FloatColor combined light colour
//   +176 float combined light intensity (average, read by Scene::Draw)
#ifndef CAVER_LIGHTOVERLAY_H
#define CAVER_LIGHTOVERLAY_H

#include "../core/FloatColor.h"
#include "../core/Rectangle.h"
#include "PointLightGroup.h"
#include "VertexArrayObject.h"

namespace Caver {

class Camera;
class RenderingContext;

class LightOverlay {
public:
    LightOverlay();
    ~LightOverlay();

    // 0x4EE4F0: rebuild the grid when the visible rectangle moved or
    // resized. Golden Scene::Draw (0x4F9270) hands over the camera pointer
    // and the binary stores it straight into the Rectangle reference, so the
    // visible rectangle is the camera's leading member.
    void SetFrame(const Rectangle& frame);
    void SetFrame(const Camera* camera) {
        SetFrame(*reinterpret_cast<const Rectangle*>(camera));
    }

    // 0x4EE82C: replace the light array. Scene::Draw builds its
    // PointLight records through PointLightManager
    // (render/PointLightGroup.h) and hands over that array, so the
    // void* overload forwards.
    void SetPointLights(const PointLight* lights, int count);
    void SetPointLights(const void* lights, int count) {
        SetPointLights(static_cast<const PointLight*>(lights), count);
    }

    // 0x4EE8E4: project every light into screen space, splat its radial
    // falloff into the cell grid, then rewrite every vertex colour.
    void UpdateWithCamera(Camera* camera);

    // 0x4EEE7C: blend the grid over the opaque pass.
    void Draw(RenderingContext* context);

private:
    // 0x4EE5FC
    void CreateVertices();

    Rectangle m_frame;                // +0
    int m_gridWidth;                  // +16
    int m_gridHeight;                 // +20
    int m_cellHeight;                 // +24
    int m_cellWidth;                  // +28
    float m_intensity;                // +32
    unsigned char m_color[4];         // +36 r, g, b, a
    float* m_cellLight;               // +40
    int m_triangleCount;              // +48
    int _pad52;                       // +52
    void* m_vertices;                 // +56
    void* m_indices;                  // +64
    VertexArrayObject m_vertexArray;  // +72
    unsigned char m_arraysCreated;    // +144
    unsigned char _pad145[3];         // +145
    int m_lightCount;                 // +148
    PointLight* m_lights;             // +152
    FloatColor m_combinedColor;       // +160
    float m_combinedIntensity;        // +176
    unsigned char _pad180[4];         // +180
};

static_assert(sizeof(LightOverlay) == 184, "LightOverlay size must be 184");

} // namespace Caver

#endif // CAVER_LIGHTOVERLAY_H
