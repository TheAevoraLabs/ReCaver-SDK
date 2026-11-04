// PostProcessPipeline for CaverEngine OpenGL 3.3 Core (nightly only).
//
// Frame flow (all HDR until the composite):
//   BeginScene  -> 8x MSAA RGBA16F scene FBO
//   EndSceneAndComposite:
//     1. MSAA resolve (colour + depth)
//     2. GTAO ambient occlusion from the depth buffer (half-res) + 2-pass depth-aware blur
//     3. 6-level bloom pyramid: 13-tap Karis prefilter, 13-tap downsample, tent upsample
//     4. Composite: lens CA, multi-bounce AO, bloom, filmic tonemap, colour grade -> LDR
//     5. Display pass: FXAA, sharpen, vignette, grain + dither -> default framebuffer
//
// Credit: Quantum Creeper / Aevora Labs.
#ifndef CAVER_RENDER_POSTPROCESSPIPELINE_H
#define CAVER_RENDER_POSTPROCESSPIPELINE_H

#ifdef CAVER_DESKTOP_EXPERIMENTAL

#include "FrameBufferObject.h"

namespace Caver {

class RenderingContext;

namespace Render {

class PostProcessPipeline {
public:
    PostProcessPipeline();
    ~PostProcessPipeline();

    bool Init(int width, int height);
    void Destroy();
    void Resize(int width, int height);

    // Call before 3D world scene draws (binds MSAA HDR FBO)
    void BeginScene(RenderingContext* context);

    // Call after 3D world scene draws and before 2D GUI (resolves MSAA, runs the
    // whole post stack, renders to FBO 0)
    void EndSceneAndComposite(RenderingContext* context);

    // The projection the lit world was drawn with (column-major). Lets SSAO rebuild
    // exact view-space positions from depth. Cheap: call it per lit draw.
    void SetSceneProjection(const float* proj16);

    // ---- Tuning ---------------------------------------------------------
    void SetBloomEnabled(bool enabled) { m_bloomEnabled = enabled; }
    bool BloomEnabled() const { return m_bloomEnabled; }
    void SetBloomIntensity(float intensity) { m_bloomIntensity = intensity; }
    void SetBloomThreshold(float threshold) { m_bloomThreshold = threshold; }
    void SetBloomScatter(float scatter) { m_bloomScatter = scatter; }
    void SetExposure(float exposure) { m_exposure = exposure; }
    void SetVibrance(float vibrance) { m_vibrance = vibrance; }
    void SetSaturation(float saturation) { m_saturation = saturation; }
    void SetContrast(float contrast) { m_contrast = contrast; }

    void SetSSAOEnabled(bool enabled) { m_ssaoEnabled = enabled; }
    bool SSAOEnabled() const { return m_ssaoEnabled; }
    // radius in world units, intensity 0..1, power (contrast of the occlusion)
    void SetSSAOParams(float radius, float intensity, float power) {
        m_aoRadius = radius; m_aoIntensity = intensity; m_aoPower = power;
    }
    // linear view depth where AO starts / finishes fading (keeps backdrops AO-free)
    void SetSSAOFade(float start, float end) { m_aoFadeStart = start; m_aoFadeEnd = end; }

    void SetFXAAEnabled(bool enabled) { m_fxaaEnabled = enabled; }
    bool FXAAEnabled() const { return m_fxaaEnabled; }
    void SetSharpen(float amount) { m_sharpen = amount; }
    void SetVignette(float strength) { m_vignette = strength; }
    void SetGrain(float amount) { m_grain = amount; }
    void SetChromaticAberration(float amount) { m_chroma = amount; }

    void SetMSAASamples(int samples);
    int MSAASamples() const { return m_msaaSamples; }

    // 0 = off, 1 = AO only, 2 = bloom only (also settable via env CAVER_POSTFX_DEBUG=ao|bloom)
    void SetDebugView(int mode) { m_debugView = mode; }

    void SetBiome(int biomeId) { m_biomeId = biomeId; }
    int Biome() const { return m_biomeId; }

    bool IsActive() const { return m_active; }
    int Width() const { return m_width; }
    int Height() const { return m_height; }

private:
    struct Program {
        unsigned int id;
        int loc[16];
        Program() : id(0) { for (int i = 0; i < 16; ++i) loc[i] = -1; }
    };

    void InitQuadVAO();
    void InitShaders();
    void RenderQuad();
    void ComputeInverseProjection(float* invProj16, bool* isOrtho) const;

    int m_width;
    int m_height;
    int m_aoWidth;
    int m_aoHeight;
    bool m_active;
    int m_msaaSamples;

    // Tuning state
    bool m_bloomEnabled;
    float m_bloomIntensity;
    float m_bloomThreshold;
    float m_bloomKnee;
    float m_bloomScatter;
    float m_exposure;
    float m_vibrance;
    float m_saturation;
    float m_contrast;
    bool m_ssaoEnabled;
    float m_aoRadius;
    float m_aoIntensity;
    float m_aoPower;
    float m_aoFadeStart;
    float m_aoFadeEnd;
    bool m_fxaaEnabled;
    float m_sharpen;
    float m_vignette;
    float m_grain;
    float m_chroma;
    int m_debugView;
    int m_biomeId;

    // Captured scene projection (0 = none yet, use built-in default)
    float m_sceneProj[16];
    bool m_haveSceneProj;

    static const int kBloomLevels = 6;

    // FBO hierarchy
    FrameBufferObject m_msaaSceneFBO;      // 8x MSAA RGBA16F + Depth24Stencil8
    FrameBufferObject m_resolvedSceneFBO;  // 1x RGBA16F + Depth24Stencil8 texture
    FrameBufferObject m_aoFBO[2];          // half-res AO ping-pong
    FrameBufferObject m_bloomDown[kBloomLevels];      // 1/2 ... 1/64
    FrameBufferObject m_bloomUp[kBloomLevels - 1];    // 1/32 ... 1/2
    FrameBufferObject m_ldrFBO;            // 1x RGBA8 (tonemapped, pre-display)

    // Programs
    Program m_progSSAO;
    Program m_progAOBlur;
    Program m_progBloomPrefilter;
    Program m_progBloomDown;
    Program m_progBloomUp;
    Program m_progComposite;
    Program m_progFinal;

    // Fullscreen quad
    unsigned int m_quadVAO;
    unsigned int m_quadVBO;
};

} // namespace Render
} // namespace Caver

#endif // CAVER_DESKTOP_EXPERIMENTAL

#endif // CAVER_RENDER_POSTPROCESSPIPELINE_H
