// Experimental OpenGL 3.3 backend (nightly only). Side-table design:
// backend state lives in a map keyed by RenderingContext pointer, so the
// RenderingContext layout (golden-verified offsets) never changes.
// Whole-file guarded: stable/Android see nothing here.
//
// The backend owns: the 4 shader programs (one template, §GL3Shaders.h),
// one VAO + one streaming ring VBO for GUI quads, and the uniform state
// (MVP, uniform color, alpha). RenderingContext methods call into it
// only on the m_api == RenderingAPIGL3 branches.
#ifndef CAVER_RENDER_GL3BACKEND_H
#define CAVER_RENDER_GL3BACKEND_H

#ifdef CAVER_DESKTOP_EXPERIMENTAL

#include "../core/Rectangle.h"

namespace Caver {

class RenderingContext;

namespace Render {
class PostProcessPipeline;
}

// Compiles the 4 program variants, creates the VAO + streaming VBO.
// False on any failure (bad loader, shader compile/link error): the
// caller must fall back to the ES1 backend in that case. Safe to call
// more than once for the same context (second call is a no-op true).
bool GL3BackendInit(RenderingContext* context);

// Selects a GUI program variant (engine program id 0, 1 or 2): binds
// the backend VAO + the variant program. Must be called from UseProgram.
void GL3BackendUseVariant(RenderingContext* context, int variant);

// FFP fallback for non-GUI programs (program 3, world programs):
// unbinds the shader (program 0) and the backend VAO so the fixed
// function path draws exactly as on ES1.
void GL3BackendUseFFP(RenderingContext* context);

// GUI-draw scope: FillRect/DrawRect/DrawTexture open it with their
// program id before calling UseProgram and close it after drawing.
// UseProgram routes by this flag, NOT by program id: the engine reuses
// programs 0/1/2 for world geometry too (Scene::Draw), and routing by
// id would push world draws through the GUI shader with wrong
// attributes (legacy texcoord lands on generic attrib 8, not 2).
// While the flag is clear, every UseProgram falls back to FFP.
void GL3BackendBeginGUIDraw(RenderingContext* context, int variant);
void GL3BackendEndGUIDraw(RenderingContext* context);
// True while a GUI method's scope is open (the SetVertexAttribPointer
// record gate).
bool GL3BackendHasVariant(RenderingContext* context);

// Records one vertex attribute for the next GUI draw. The data pointer
// is NOT copied here: it must stay valid until GL3BackendCommitDraw
// (the GUI methods pass stack quads and commit in the same call).
void GL3BackendRecordAttrib(RenderingContext* context,
                            unsigned int engineIndex, int size,
                            unsigned int type, int stride,
                            const void* data);

// Commits the pending GUI draw: uploads MVP + uniform color + alpha,
// streams the recorded attributes into the ring VBO, sets the attrib
// pointers + enables on the backend VAO. Called by FillRect/DrawRect/
// DrawTexture right before glDrawArrays.
void GL3BackendCommitDraw(RenderingContext* context, int count,
                          const float* proj, const float* mv,
                          unsigned char r, unsigned char g, unsigned char b,
                          unsigned char a, float alpha);

// Uploads client index buffer to immIBO during GUI variant draw
void GL3BackendUploadGUIIndices(RenderingContext* context, int count,
                                unsigned int type, const void* indices);

// Flush-path uniform uploads (used by FlushPendingDrawState).
// No-ops when no GUI variant is current.
void GL3BackendUploadColor(RenderingContext* context, unsigned char r,
                           unsigned char g, unsigned char b,
                           unsigned char a);
void GL3BackendUploadMVP(RenderingContext* context, const float* proj,
                         const float* mv);
void GL3BackendUploadAlpha(RenderingContext* context, float alpha);

// ---- World (Phase 5+) state. Sticky FFP state, mirrored: material
// colors, global ambient, directional lights (eye space), flash and
// rescale flags. Set by the GL3 branches next to the FFP calls they
// mirror (dual: the FFP calls still run on compat for the draws that
// have not moved to the shader yet). Defaults are the FFP defaults.
void GL3BackendSetMaterialAmbient(RenderingContext* context,
                                  const float* rgba);
void GL3BackendSetMaterialDiffuse(RenderingContext* context,
                                  const float* rgba);
void GL3BackendSetMaterialAmbientAndDiffuse(RenderingContext* context,
                                            const float* rgba);
void GL3BackendSetMaterialEmission(RenderingContext* context,
                                   const float* rgba);
void GL3BackendSetGlobalAmbient(RenderingContext* context,
                                const float* rgba);
// Eye-space direction (already transformed by the view matrix, like
// FFP transforms POSITION by the modelview current at glLightfv).
void GL3BackendSetDirLight(RenderingContext* context, int index,
                           float dirX, float dirY, float dirZ,
                           const float* diffuse, const float* specular);
// Mirrors glDisable(GL_LIGHT0 + index): zeroes that light's diffuse
// and specular so it contributes nothing, WITHOUT touching other
// indices — FFP lets lights beyond the current frame's count leak
// enabled, and the backend reproduces that instead of resetting.
void GL3BackendDisableDirLight(RenderingContext* context, int index);
void GL3BackendSetPointLights(RenderingContext* context, int count,
                              const float* posRadius4f, const float* colorIntensity4f);
void GL3BackendSetFlash(RenderingContext* context, bool active,
                        const float* rgba);
void GL3BackendSetRescaleNormal(RenderingContext* context, bool enabled);

// World-draw scope, owned by VertexArrayObject::Draw on GL3 (like the
// GUI scope): while open, DrawArrays/DrawElements bind the world
// program and upload its uniforms instead of drawing FFP. Direct
// draws (trails, shapes, shadow volumes) never open it, so they keep
// their FFP path on compat until their own phase translates them.
void GL3BackendBeginWorldDraw(RenderingContext* context);
void GL3BackendEndWorldDraw(RenderingContext* context);
bool GL3BackendWorldDrawActive(RenderingContext* context);
// Attribute presence for the mesh being drawn (from its VAO
// descriptors): color array -> vertex colors, normal array -> real
// normals, else the FFP current defaults (glColor / (0,0,1)).
void GL3BackendSetWorldAttribs(RenderingContext* context, bool hasNormal,
                               bool hasColor);
// Binds the world program and uploads every uniform for one draw.
// No-op when the world program failed to compile (FFP fallback).
void GL3BackendBindWorldDraw(RenderingContext* context, const float* proj,
                             const float* mv, bool lightingEnabled,
                             bool texturingEnabled, unsigned char cr,
                             unsigned char cg, unsigned char cb,
                             unsigned char ca, float alpha);

// ---- Immediate (Phase 6) draws: direct context->DrawArrays calls
// that never go through a VertexArrayObject (stencil shadow volumes,
// Scene's blob shadows). The component opens the scope, then
// SetVertexAttribPointer records its client arrays, and the first
// DrawArrays consumes the scope: world-shader draw from a scratch
// VBO, then unbind + flag clear, so the next direct draw is plain
// FFP. DrawElements with the scope open cancels it and draws FFP
// (safety valve; no Phase 6 call site uses DrawElements). No-ops
// without a GL3 backend; call sites are ifdef'd too, so stable and
// Android never see this.
// Single-shot scope (Phase 6): the first DrawArrays/DrawElements
// consumes it (shader draw + unbind + flag clear).
void GL3BackendBeginImmediateDraw(RenderingContext* context);
// Multi-draw scope (Phase 7): draws do NOT consume it; the component
// calls GL3BackendFinishImmediateDraw after its last draw (used by
// MagicExplosion's two star draws, which share one pointer setup).
void GL3BackendBeginImmediateDrawMulti(RenderingContext* context);
bool GL3BackendImmediateDrawActive(RenderingContext* context);
bool GL3BackendImmediateDrawMulti(RenderingContext* context);
void GL3BackendRecordImmediateAttrib(RenderingContext* context,
                                     unsigned int engineIndex, int size,
                                     unsigned int type, int stride,
                                     const void* data);
void GL3BackendBindImmediateDraw(RenderingContext* context, int first,
                                 int count);
// Indexed form: scans the index array for the highest vertex, uploads
// exactly that many vertices, uploads the indices to a scratch IBO.
// The caller must draw with a zero index offset (the IBO is bound).
void GL3BackendBindImmediateDrawElements(RenderingContext* context,
                                         int count, unsigned int type,
                                         const void* indices);
void GL3BackendFinishImmediateDraw(RenderingContext* context);

// VAO side table keyed by VertexArrayObject pointer (no layout change
// to that class): one real VAO per active buffer index, plus a
// client-side upload buffer for VAOs without a VBO. Returns 0 when the
// backend is not initialized.
class VertexArrayObject;
unsigned int GL3BackendGetVAO(RenderingContext* context,
                              VertexArrayObject* vaoObj, int activeIdx);
unsigned int GL3BackendGetClientVBO(RenderingContext* context,
                                    VertexArrayObject* vaoObj,
                                    const void* data, int size);
// Indexed twin of GetClientVBO (Phase 8): a scratch IBO for VAOs
// that draw indexed without ever calling CreateBufferObjects
// (water is the only one). Re-uploaded on every draw.
unsigned int GL3BackendGetClientIBO(RenderingContext* context,
                                    VertexArrayObject* vaoObj,
                                    const void* data, int size);
void GL3BackendForgetVAO(RenderingContext* context,
                         VertexArrayObject* vaoObj);

// Biome state (Phase 10)
void GL3BackendSetBiome(RenderingContext* context, int biomeId);
int GL3BackendGetBiome(RenderingContext* context);

// Post-process & Shadow pass scope (Phase 9)
void GL3BackendSetCameraViewMatrix(RenderingContext* context, const float* viewMatrix16f);
void GL3BackendBeginScene(RenderingContext* context, int width = 0, int height = 0);
void GL3BackendEndScene(RenderingContext* context);
bool GL3BackendSceneActive(RenderingContext* context);
void GL3BackendBeginShadowPass(RenderingContext* context, float dirX, float dirY, float dirZ,
                               float minX, float minY, float maxX, float maxY);
void GL3BackendEndShadowPass(RenderingContext* context);

// Thumbnail / Offscreen Subview Render Target Scope (ProfilePanelView, miniature 3D previews)
void GL3BackendBeginThumbnail(RenderingContext* context, int width = 256, int height = 256);
void GL3BackendEndThumbnail(RenderingContext* context);
bool GL3BackendThumbnailActive(RenderingContext* context);
unsigned int GL3BackendGetThumbnailTexture(RenderingContext* context);
void GL3BackendDrawTexturedRect(RenderingContext* context, const Rectangle& rect, unsigned int textureId);

Render::PostProcessPipeline* GL3GetPostProcess(RenderingContext* context = 0);
bool GL3GetShadowsEnabled();
void GL3SetShadowsEnabled(bool enabled);

} // namespace Caver

#endif // CAVER_DESKTOP_EXPERIMENTAL

#endif // CAVER_RENDER_GL3BACKEND_H
