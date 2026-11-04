// Recovered from libswordigo.so (1.4.13) evidence — see docs/CORE_HEADERS.md
// and the graphics deep-dive. GLES1 fixed-function state cache: every GL
// state change is funnelled through here and cached in the members below,
// so redundant GL calls are suppressed.
//
// Layout (byte offsets verified against the golden binary):
//   +0   RenderingAPI m_api
//   +8   program list anchor (head pointer + embedded sentinel node)
//   +32  boost::shared_ptr<RenderingProgram> m_currentProgram
//   +48  Rectangle m_viewport
//   +80  blend-func cache (src, dst)
//   +88  cached capability flags (blend/texture/lighting/cull/depth)
//   +95  Color m_color (init 255,255,255,255)
//   +100 float m_alpha (init 1.0)
//   +104 bool m_matrixDirty
//   +108 Matrix4 m_modelViewMatrix
//   +172 Matrix4 m_projectionMatrix
//   +236 unsigned m_boundTexture (GL name, 0 = none)
//   +280 cached white texture (boost::shared_ptr<Texture>)
#ifndef CAVER_RENDERINGCONTEXT_H
#define CAVER_RENDERINGCONTEXT_H

#include "../core/Color.h"
#include "../core/Matrix4.h"
#include "../core/Rectangle.h"
#include <boost/intrusive_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <map>

namespace Caver {

enum RenderingAPI {
    RenderingAPIGLES1 = 0,
    RenderingAPIGLES2 = 1,
    // Experimental desktop backend: real GLSL 3.30 shaders behind
    // CAVER_DESKTOP_EXPERIMENTAL (nightly only). Never selected on
    // stable or Android; the ES1 path is untouched by its existence.
    RenderingAPIGL3 = 2
};

// Values are the GL type constants themselves: DrawElements passes a
// DataType straight to glDrawElements in the golden binary.
enum DataType {
    DataTypeByte = 5120,          // GL_BYTE
    DataTypeUnsignedByte = 5121,  // GL_UNSIGNED_BYTE
    DataTypeShort = 5122,         // GL_SHORT
    DataTypeUnsignedShort = 5123, // GL_UNSIGNED_SHORT
    DataTypeInt = 5124,           // GL_INT
    DataTypeUnsignedInt = 5125,   // GL_UNSIGNED_INT
    DataTypeFloat = 5126          // GL_FLOAT
};

class RenderingProgram;
class Texture;
class VertexArrayObject;
class GUIWindowFramebuffer;

// Free helpers implemented by the platform/render glue (not members):
void BindBackgroundGLContext();
void UnbindBackgroundGLContext();
boost::shared_ptr<GUIWindowFramebuffer> NewFramebufferImplForDrawingToTexture(
    const boost::intrusive_ptr<Texture>& texture);

class RenderingContext {
    // Texture::SetDefaultParameters reads m_unknown76 (+76) directly in the
    // golden binary right after a CurrentContext() call.
    friend class Texture;

public:
    explicit RenderingContext(RenderingAPI api);

    static RenderingContext* CurrentContext();
    static RenderingContext* SetCurrentContext(RenderingContext* context);

    void StartBackgroundLoading();
    void FinishBackgroundLoading();

    void SetClearColor(const Color& color);
    void SetClearDepth(float depth);
    void Clear(bool color, bool depth, bool stencil);
    void SetViewport(const Rectangle& viewport);

    void SetColor(const Color& color);
    void SetAlpha(float alpha);
    void SetMatrix(const Matrix4& matrix);
    void SetProjectionMatrix(const Matrix4& matrix);
    void SetIdentityMatrix();

    void SetBlendFunc(unsigned int src, unsigned int dst);
    void SetDefaultBlendFunc();
    void SetBlendingEnabled(bool enabled);
    void SetTexturingEnabled(bool enabled);
    void SetLightingEnabled(bool enabled);
    void SetCullFaceEnabled(bool enabled);
    void SetDepthTestEnabled(bool enabled);
    void SetDepthWriteEnabled(bool enabled);
#if defined(CAVER_DESKTOP_EXPERIMENTAL) || defined(CAVER_STENCIL_FIX)
    // Stencil shadow volumes (experimental/stencilfix only): writes the
    // Scene::Draw volume gate flag (+72), which golden leaves at 0.
    void SetStencilShadowsEnabled(bool enabled);
#endif

    void BindTexture(Texture* texture);
    boost::intrusive_ptr<Texture> WhiteTexture();
    // This must be boost::shared_ptr<GUIWindowFramebuffer>, not
    // intrusive_ptr<Texture>. The golden function it wraps
    // (NewFramebufferImplForDrawingToTexture @0x539D74) writes 16 zero
    // bytes, i.e. a NULL shared_ptr, and GUIWindow.cpp's binding for
    // this symbol already declares the shared_ptr return. With the
    // old intrusive_ptr declaration the callee returned 8 bytes while
    // the caller read 16 (RAX:RDX), so RDX garbage became the
    // shared_ptr's control block: releasing it jumped to address 0x8
    // (the modal-view crash), and when the garbage aliased a live
    // control block it corrupted that object's lifetime (a later
    // abort in unrelated teardown).
    boost::shared_ptr<GUIWindowFramebuffer> FramebufferForDrawingToTexture(
        const boost::intrusive_ptr<Texture>& texture);

    void AddProgram(unsigned int identifier,
                    const boost::shared_ptr<RenderingProgram>& program);
    boost::shared_ptr<RenderingProgram> ProgramForIdentifier(
        unsigned int identifier);
    void UseProgram(const boost::shared_ptr<RenderingProgram>& program);
    void UseProgram(unsigned int identifier);
    int SetUniformVariable(int location, const Matrix4& value);

    void SetEnabledVertexAttribArrays(const unsigned int* arrays, int count);
    void SetEnabledVertexAttribArrays(unsigned int array, ...);
    void SetVertexAttribPointer(unsigned int index, int size, DataType type,
                                 int stride, const void* pointer);

    void PrepareForDrawing();
    void DrawArrays(int mode, int first, int count);
    void DrawElements(int mode, int count, DataType type,
                      const void* indices);
    void DrawTexture(const boost::intrusive_ptr<Texture>& texture,
                     const Rectangle& srcRect, const Rectangle& dstRect,
                     float alpha);
    void DrawRect(const Rectangle& rect, const Color& color, float depth);
    void FillRect(const Rectangle& rect, const Color* colors, float depth);
    void FillRect(const Rectangle& rect, const Color& color, float depth);

private:
    void SetEnabledVertexAttribArraysImpl(unsigned int* arrays, int count);
    void FlushPendingDrawState();

    RenderingAPI m_api;                                     // +0
    std::map<unsigned int, boost::shared_ptr<RenderingProgram>> m_programs; // +8
    boost::shared_ptr<RenderingProgram> m_currentProgram;   // +32
    Rectangle m_viewport;                                   // +48
    float m_unknown64;                                      // +64 = 1.0
    float m_unknown68;                                      // +68 = 1.0
    int m_unknown72;                                        // +72 = 0
    bool m_unknown76;                                       // +76 = 0
    unsigned int m_blendSrc;                                // +80 = 1
    unsigned int m_blendDst;                                // +84 = 0
    bool m_blendingEnabled;                                 // +88 = false
    bool m_texturingEnabled;                                // +89 = false
    bool m_lightingEnabled;                                 // +90 = false
    bool m_cullFaceEnabled;                                 // +91 = false
    bool m_depthTestEnabled;                                // +92 = false
    bool m_depthWriteEnabled;                               // +93 = true
    bool m_colorChanged;                                    // +94 = false
    Color m_color;                                          // +95 = white
    unsigned char m_pad99;                                  // +99
    float m_alpha;                                          // +100 = 1.0
    bool m_matrixDirty;                                     // +104 = false
    unsigned char m_pad105[3];                              // +105
    Matrix4 m_modelViewMatrix;                              // +108
    Matrix4 m_projectionMatrix;                             // +172
    unsigned int m_boundTexture;                            // +236 = 0
    unsigned int m_enabledAttribArrays[10];                 // +240
    int m_enabledAttribArrayCount;                          // +280 = 0
    boost::intrusive_ptr<Texture> m_whiteTexture;           // +288
};

} // namespace Caver

#endif
