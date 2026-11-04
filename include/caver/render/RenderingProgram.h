// Recovered from libswordigo.so (1.4.13) evidence — declaration only;
// the class's own functions are outside the modelmesh queue. Field offsets
// are pinned by RenderingContext::UseProgram's accesses in the golden
// binary: bool @+36, attrib-array count @+40, attrib-array data @+48.
#ifndef CAVER_RENDERINGPROGRAM_H
#define CAVER_RENDERINGPROGRAM_H

#include <string>
#include <vector>
#include <boost/shared_ptr.hpp>

namespace Caver {

class RenderingContext;
class RenderingProgramShader;

class RenderingProgram {
public:
    RenderingProgram(RenderingContext* context, const std::string& name);
    ~RenderingProgram();

    void AddVertexAttribute(unsigned int index, const std::string& name);
    bool LinkWithShaders(const boost::shared_ptr<RenderingProgramShader>& vertexShader,
                         const boost::shared_ptr<RenderingProgramShader>& fragmentShader);

    int LocationOfUniformVariable(int location);
    bool HasUniformVariable(int location);

    RenderingContext* m_context;                                // +0
    std::string m_name;                                         // +8
    unsigned int m_glProgram;                                   // +32
    bool m_texturing;                                           // +36
    unsigned char m_pad37[3];                                   // +37
    int m_attribArrayCount;                                     // +40
    unsigned char m_pad44[4];                                   // +44
    std::vector<unsigned int> m_attribVector;                   // +48
    int m_uniformLocations[14];                                 // +72..127
    boost::shared_ptr<RenderingProgramShader> m_vertexShader;   // +128
    boost::shared_ptr<RenderingProgramShader> m_fragmentShader; // +144
};

} // namespace Caver

#endif
