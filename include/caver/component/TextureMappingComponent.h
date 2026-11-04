#ifndef CAVER_TEXTUREMAPPINGCOMPONENT_H
#define CAVER_TEXTUREMAPPINGCOMPONENT_H

#include "Component.h"
#include "../core/Vector2.h"
#include "../render/TextureMapping.h"
#include <boost/intrusive_ptr.hpp>
#include <string>
#include <vector>

namespace Caver {

class Texture;

// Recovered from libswordigo.so (1.4.13) evidence — vtable _ZTVN5Caver23TextureMappingComponentE
// at 0x61B8E8 (sizeof 0x98 = 152 bytes).
class TextureMappingComponent : public Component {
public:
    TextureMappingComponent();
    virtual ~TextureMappingComponent();

    static TextureMappingComponent* Create();
    static long Interface();

    virtual TextureMappingComponent* Clone() const override;
    void InitWithComponent(const TextureMappingComponent* other);

    virtual void LoadFromProtobufMessage(const Proto::Component& message) override;
    virtual void SaveToProtobufMessage(Proto::Component* message) const override;
    virtual bool ShouldSave() override;
    virtual void Prepare() override;

    virtual void GetBindings(std::vector<Binding>* bindings) override;
    virtual BindingValue ValueForBindedProperty(int property) override;
    virtual void SetValueForBindedProperty(int property, const BindingValue& value) override;

    virtual bool ImplementsInterface(long interfaceId) override;
    virtual void RegisterInterfaces(ComponentManager* manager) override;
    virtual std::string className() const override;

    const TextureMapping* mapping() const {
        return reinterpret_cast<const TextureMapping*>(&m_texture);
    }

    std::string m_textureName;                          // +104
    boost::intrusive_ptr<Texture> m_texture;            // +128
    float m_scale;                                      // +136 (default 250.0f)
    Vector2 m_offset;                                   // +140 (default {0, 0})
    char _pad148[4];                                    // +148
};

// Golden aarch64 NDK-libc++ layout anchor; MSVC STL member sizes differ,
// so this assert is Android/Linux-only (the Windows build is self-consistent
// under MSVC STL and does not claim golden byte-layout parity).
#ifndef _MSC_VER
static_assert(sizeof(TextureMappingComponent) == 152, "TextureMappingComponent size must be 152 bytes");
#endif

} // namespace Caver

#endif // CAVER_TEXTUREMAPPINGCOMPONENT_H
