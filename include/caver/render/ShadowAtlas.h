// ShadowAtlas abstraction for CaverEngine OpenGL 3.3 Core (nightly only).
// Manages the shadow depth FBO, the directional light matrix, and the two sampler
// objects (hardware-PCF compare + raw depth) the PCSS world shader reads it through.
// Credit: Quantum Creeper / Aevora Labs.
#ifndef CAVER_RENDER_SHADOWATLAS_H
#define CAVER_RENDER_SHADOWATLAS_H

#ifdef CAVER_DESKTOP_EXPERIMENTAL

#include "FrameBufferObject.h"
#include "../core/Matrix4.h"
#include "../core/Vector3.h"
#include "../core/Rectangle.h"

namespace Caver {
namespace Render {

class ShadowAtlas {
public:
    // Texture units the world shader's u_shadowMap / u_shadowDepth samplers use.
    static const int kCompareUnit = 2;
    static const int kRawUnit = 3;

    ShadowAtlas();
    ~ShadowAtlas();

    // Falls back to 2048 if the requested size cannot be allocated.
    bool Init(int resolution = 4096);
    void Destroy();

    // Builds the light matrix and binds the shadow depth FBO.
    // lightDir is the direction the light TRAVELS in world space (pointing away
    // from the sun). The oblique shear that maps it onto the shadow map is derived
    // from it, so the cast shadows always agree with the shading direction.
    void BeginPass(const Vector3& lightDir, const Rectangle& sceneBounds);
    void EndPass();

    // Binds the depth texture + both samplers on kCompareUnit / kRawUnit.
    void BindForSampling();
    // Unbinds them (required before the depth texture is rendered to again, and
    // before the post-process passes reuse those units for ordinary textures).
    void UnbindSampling();

    unsigned int DepthTexture() const { return m_fbo.DepthTexture(); }
    const float* LightSpaceMatrix() const { return m_lightSpaceMatrix.m; }
    int Resolution() const { return m_resolution; }
    bool IsActive() const { return m_active; }

    // World units covered by one shadow texel, and the world depth span that the
    // 0..1 shadow depth range represents (for penumbra estimation in the shader).
    float TexelWorldSize() const { return m_texelWorld; }
    float DepthRangeWorld() const { return m_depthRangeWorld; }

private:
    FrameBufferObject m_fbo;
    Matrix4 m_lightSpaceMatrix;
    int m_resolution;
    bool m_active;
    unsigned int m_cmpSampler;
    unsigned int m_rawSampler;
    float m_texelWorld;
    float m_depthRangeWorld;
};

} // namespace Render
} // namespace Caver

#endif // CAVER_DESKTOP_EXPERIMENTAL

#endif // CAVER_RENDER_SHADOWATLAS_H
