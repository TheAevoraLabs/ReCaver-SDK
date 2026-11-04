// Recovered from libswordigo.so (1.4.13) evidence — layout pinned by
// the ctor @0x518238, dtor @0x518294 and AddUniformVariable
// @0x51831C (jni-small SF-1). sizeof 80 (operator new(0x50) at the
// BasicRenderingPrograms allocation sites).
//   +0  RenderingContext* m_context
//   +8  std::string m_name
//   +32 RenderingProgramShaderType m_type (0 = vertex, 1 = fragment
//       at the AddToContext call sites; enumerator names UNRECOVERED)
//   +36 int m_field36 — zeroed by the ctor, never read in-slice
//       (UNRECOVERED)
//   +40 int m_uniformCount — incremented by AddUniformVariable
//   +48 std::vector<UniformVariable> m_uniforms (element 32 bytes:
//       int at +0, std::string at +8)
//   +72 bool m_field72 — zeroed by the ctor (UNRECOVERED)
#ifndef CAVER_RENDERINGPROGRAMSHADER_H
#define CAVER_RENDERINGPROGRAMSHADER_H

#include <string>
#include <vector>

namespace Caver {

class RenderingContext;

enum RenderingProgramShaderType {
    RenderingProgramShaderType0 = 0,  // UNRECOVERED enumerator name (vertex at call sites)
    RenderingProgramShaderType1 = 1   // UNRECOVERED enumerator name (fragment at call sites)
};

class RenderingProgramShader {
public:
    struct UniformVariable {
        int m_field0;        // +0 (the location argument)
        std::string m_name;  // +8
    };

    RenderingProgramShader(RenderingContext* context, const std::string& name,
                           RenderingProgramShaderType type, const char* source);
    ~RenderingProgramShader();

    void AddUniformVariable(int location, const std::string& name);

    RenderingContext* m_context;                  // +0
    std::string m_name;                             // +8
    RenderingProgramShaderType m_type;              // +32
    int m_field36;                                  // +36 UNRECOVERED
    int m_uniformCount;                             // +40
    std::vector<UniformVariable> m_uniforms;        // +48
    bool m_field72;                                 // +72 UNRECOVERED
    // UNRECOVERED: the shader source/gl name are not stored in the
    // 80 bytes the ctor writes — compilation is a ctor side effect
    // on the context/GL, outside this slice's graded bodies.
};

} // namespace Caver

#endif
