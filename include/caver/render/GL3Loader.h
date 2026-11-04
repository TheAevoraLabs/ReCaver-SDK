// Experimental OpenGL 3.3 entry-point loader (nightly only).
// Resolves the GL >= 3.0 entry points the GL3 backend needs via
// SDL_GL_GetProcAddress at context creation. No GLEW/glad dependency.
// Whole-file guarded: on stable/Android this header expands to nothing
// and the TU compiles empty, so no GL3 symbol ever reaches those builds.
//
// Function-pointer types use plain C types only (no GL headers), so this
// header is safe to include from any TU.
#ifndef CAVER_RENDER_GL3LOADER_H
#define CAVER_RENDER_GL3LOADER_H

#ifdef CAVER_DESKTOP_EXPERIMENTAL

namespace Caver {

// GL types, spelled with standard C types (LP64: long is 64-bit, matching
// GLsizeiptr/GLintptr on the desktop targets).
typedef unsigned int GL3Enum;
typedef unsigned int GL3Bitfield;
typedef unsigned int GL3Uint;
typedef int GL3Int;
typedef int GL3Sizei;
typedef unsigned char GL3Boolean;
typedef float GL3Float;
typedef char GL3Char;
typedef long GL3Sizeiptr;
typedef long GL3Intptr;

struct GL3Functions {
    void (*GenVertexArrays)(GL3Sizei n, GL3Uint* arrays);
    void (*BindVertexArray)(GL3Uint array);
    void (*DeleteVertexArrays)(GL3Sizei n, const GL3Uint* arrays);
    void (*GenBuffers)(GL3Sizei n, GL3Uint* buffers);
    void (*BindBuffer)(GL3Enum target, GL3Uint buffer);
    void (*BufferData)(GL3Enum target, GL3Sizeiptr size, const void* data,
                       GL3Enum usage);
    void (*BufferSubData)(GL3Enum target, GL3Intptr offset, GL3Sizeiptr size,
                          const void* data);
    void (*DeleteBuffers)(GL3Sizei n, const GL3Uint* buffers);
    void (*VertexAttribPointer)(GL3Uint index, GL3Int size, GL3Enum type,
                                GL3Boolean normalized, GL3Sizei stride,
                                const void* pointer);
    void (*EnableVertexAttribArray)(GL3Uint index);
    void (*DisableVertexAttribArray)(GL3Uint index);
    GL3Uint (*CreateShader)(GL3Enum type);
    void (*ShaderSource)(GL3Uint shader, GL3Sizei count,
                         const GL3Char* const* string, const GL3Int* length);
    void (*CompileShader)(GL3Uint shader);
    void (*GetShaderiv)(GL3Uint shader, GL3Enum pname, GL3Int* params);
    void (*GetShaderInfoLog)(GL3Uint shader, GL3Sizei bufSize,
                             GL3Sizei* length, GL3Char* infoLog);
    void (*DeleteShader)(GL3Uint shader);
    GL3Uint (*CreateProgram)();
    void (*AttachShader)(GL3Uint program, GL3Uint shader);
    void (*LinkProgram)(GL3Uint program);
    void (*GetProgramiv)(GL3Uint program, GL3Enum pname, GL3Int* params);
    void (*GetProgramInfoLog)(GL3Uint program, GL3Sizei bufSize,
                              GL3Sizei* length, GL3Char* infoLog);
    void (*UseProgram)(GL3Uint program);
    void (*DeleteProgram)(GL3Uint program);
    GL3Int (*GetUniformLocation)(GL3Uint program, const GL3Char* name);
    void (*UniformMatrix4fv)(GL3Int location, GL3Sizei count,
                             GL3Boolean transpose, const GL3Float* value);
    void (*Uniform4f)(GL3Int location, GL3Float v0, GL3Float v1, GL3Float v2,
                      GL3Float v3);
    void (*Uniform2f)(GL3Int location, GL3Float v0, GL3Float v1);
    void (*Uniform1f)(GL3Int location, GL3Float v0);
    void (*Uniform1i)(GL3Int location, GL3Int v0);
    void (*Uniform3fv)(GL3Int location, GL3Sizei count,
                       const GL3Float* value);
    void (*Uniform4fv)(GL3Int location, GL3Sizei count,
                       const GL3Float* value);

    // Framebuffer Objects (FBO)
    void (*GenFramebuffers)(GL3Sizei n, GL3Uint* framebuffers);
    void (*BindFramebuffer)(GL3Enum target, GL3Uint framebuffer);
    void (*DeleteFramebuffers)(GL3Sizei n, const GL3Uint* framebuffers);
    void (*FramebufferTexture2D)(GL3Enum target, GL3Enum attachment,
                                 GL3Enum textarget, GL3Uint texture, GL3Int level);
    void (*FramebufferRenderbuffer)(GL3Enum target, GL3Enum attachment,
                                    GL3Enum renderbuffertarget, GL3Uint renderbuffer);
    GL3Enum (*CheckFramebufferStatus)(GL3Enum target);
    void (*BlitFramebuffer)(GL3Int srcX0, GL3Int srcY0, GL3Int srcX1, GL3Int srcY1,
                            GL3Int dstX0, GL3Int dstY0, GL3Int dstX1, GL3Int dstY1,
                            GL3Bitfield mask, GL3Enum filter);
    void (*DrawBuffers)(GL3Sizei n, const GL3Enum* bufs);

    // Renderbuffer Objects (RBO)
    void (*GenRenderbuffers)(GL3Sizei n, GL3Uint* renderbuffers);
    void (*BindRenderbuffer)(GL3Enum target, GL3Uint renderbuffer);
    void (*DeleteRenderbuffers)(GL3Sizei n, const GL3Uint* renderbuffers);
    void (*RenderbufferStorage)(GL3Enum target, GL3Enum internalformat,
                                GL3Sizei width, GL3Sizei height);
    void (*RenderbufferStorageMultisample)(GL3Enum target, GL3Sizei samples,
                                           GL3Enum internalformat, GL3Sizei width,
                                           GL3Sizei height);

    // Uniform Buffer Objects (UBO)
    GL3Uint (*GetUniformBlockIndex)(GL3Uint program, const GL3Char* uniformBlockName);
    void (*UniformBlockBinding)(GL3Uint program, GL3Uint uniformBlockIndex,
                                GL3Uint uniformBlockBinding);
    void (*BindBufferBase)(GL3Enum target, GL3Uint index, GL3Uint buffer);

    // Sampler Objects (GL 3.3 core). Lets one depth texture be read both as a
    // hardware-PCF shadow sampler and as a raw depth sampler (PCSS blocker search).
    void (*GenSamplers)(GL3Sizei count, GL3Uint* samplers);
    void (*DeleteSamplers)(GL3Sizei count, const GL3Uint* samplers);
    void (*BindSampler)(GL3Uint unit, GL3Uint sampler);
    void (*SamplerParameteri)(GL3Uint sampler, GL3Enum pname, GL3Int param);
};

// Resolved entry points. Valid only after a GL >= 3.3 context is current.
extern GL3Functions g_gl3;

// Resolves every entry point above. Returns false (leaving g_gl3
// partially filled) if any required symbol is missing; the caller must
// treat false as "GL3 backend unavailable" and fall back to ES1.
bool GL3LoadFunctions();

} // namespace Caver

#endif // CAVER_DESKTOP_EXPERIMENTAL

#endif // CAVER_RENDER_GL3LOADER_H
