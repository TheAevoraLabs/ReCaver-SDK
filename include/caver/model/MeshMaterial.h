// Recovered from libswordigo.so (1.4.13) evidence.
// Caver::MeshMaterial — 88 bytes, embedded by value in Mesh at +160.
// Layout proven from Mesh::Mesh's inlined member-init store map (three
// {1,1,1,1} float colours at +0..+47, a zeroed dword at +48) and from
// ~MeshMaterial (88B, WEAK in the golden = header-defined): it destroys
// a std::string at +64 (long-buffer delete via flag byte at +64, data
// pointer at +80) and releases an intrusive_ptr at +56 (refcount at
// pointee +8, destroy through vtable +8) — in that order, i.e. reverse
// declaration order, exactly as the member teardown inlined into
// Mesh::~Mesh. Member NAMES (which colour is which, the string's role,
// the +48 field) are UNRECOVERED; the colour order follows the proto
// field order (ambient first — Proto::MeshMaterial has
// kAmbientColorFieldNumber).
#ifndef CAVER_MESHMATERIAL_H
#define CAVER_MESHMATERIAL_H

#include "../core/FloatColor.h"
#include "../render/Texture.h"
#include <boost/intrusive_ptr.hpp>
#include <string>

namespace Caver {

namespace Proto {
class MeshMaterial;
}

class MeshMaterial {
public:
    MeshMaterial() : m_field48(0) {
        m_color0.r = 1.0f; m_color0.g = 1.0f; m_color0.b = 1.0f; m_color0.a = 1.0f;
        m_color1.r = 1.0f; m_color1.g = 1.0f; m_color1.b = 1.0f; m_color1.a = 1.0f;
        m_color2.r = 1.0f; m_color2.g = 1.0f; m_color2.b = 1.0f; m_color2.a = 1.0f;
    }
    ~MeshMaterial() {}

    void LoadFromProtobufMessage(const Proto::MeshMaterial& message);
    void SaveToProtobufMessage(Proto::MeshMaterial* message) const;

    FloatColor m_color0;                        // +0  ambient (proto order)
    FloatColor m_color1;                        // +16 diffuse
    FloatColor m_color2;                        // +32 specular
    float m_field48;                            // +48 shininess (proto field 4;
                                                //  Load stores it as a raw
                                                //  dword, Save sets it)
    boost::intrusive_ptr<Texture> m_texture;    // +56
    std::string m_string64;                     // +64 texture name (path
                                                //  extension removed; set
                                                //  by PODLoader::CreateMesh)
};

} // namespace Caver

#endif
