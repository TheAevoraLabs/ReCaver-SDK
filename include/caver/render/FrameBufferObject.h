// FrameBufferObject abstraction for CaverEngine OpenGL 3.3 Core (nightly only).
// Manages FBOs, HDR/LDR color attachments, depth textures, and MSAA renderbuffers.
// Credit: Quantum Creeper / Aevora Labs.
#ifndef CAVER_RENDER_FRAMEBUFFEROBJECT_H
#define CAVER_RENDER_FRAMEBUFFEROBJECT_H

#ifdef CAVER_DESKTOP_EXPERIMENTAL

#include "GL3Loader.h"

namespace Caver {
namespace Render {

enum class FBOColorFormat {
    None,
    RGBA8,
    RGBA16F
};

enum class FBODepthFormat {
    None,
    Depth24,
    Depth32F,
    Depth24Stencil8
};

class FrameBufferObject {
public:
    FrameBufferObject();
    ~FrameBufferObject();

    // Creates an FBO with the specified formats, dimensions, and sample count.
    // If samples > 1, creates multisample renderbuffers for color and depth.
    // If depthIsTexture is true (and samples <= 1), depth is created as a readable texture.
    bool Create(int width, int height, FBOColorFormat colorFmt,
                FBODepthFormat depthFmt, int samples = 1,
                bool depthIsTexture = false, bool depthCompareMode = false);

    void Destroy();

    void Bind(bool setViewport = true);
    void Unbind();

    // Resolves/copies color (and optionally depth) to another FBO or default backbuffer (nullptr).
    void BlitTo(FrameBufferObject* target, bool blitColor = true, bool blitDepth = false);

    unsigned int FBO() const { return m_fbo; }
    unsigned int ColorTexture() const { return m_colorTexture; }
    unsigned int DepthTexture() const { return m_depthTexture; }
    int Width() const { return m_width; }
    int Height() const { return m_height; }
    int Samples() const { return m_samples; }
    bool IsValid() const { return m_fbo != 0; }

private:
    unsigned int m_fbo;
    unsigned int m_colorTexture;
    unsigned int m_depthTexture;
    unsigned int m_colorRbo;
    unsigned int m_depthRbo;
    int m_width;
    int m_height;
    int m_samples;
    FBOColorFormat m_colorFormat;
    FBODepthFormat m_depthFormat;
};

} // namespace Render
} // namespace Caver

#endif // CAVER_DESKTOP_EXPERIMENTAL

#endif // CAVER_RENDER_FRAMEBUFFEROBJECT_H
