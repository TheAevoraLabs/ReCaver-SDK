#ifndef CAVER_SPRITECOMPONENT_H
#define CAVER_SPRITECOMPONENT_H

#include "Component.h"
#include "../core/Rectangle.h"
#include "../render/Sprite.h"
#include <boost/intrusive_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <string>
#include <vector>

namespace Caver {

class RenderingContext;
struct Matrix4;
class Texture;

// Recovered from libswordigo.so (1.4.13) evidence — vtable _ZTVN5Caver15SpriteComponentE
// at 0x61B530 (sizeof 0xA8 = 168 bytes).
class SpriteComponent : public Component {
public:
    SpriteComponent();
    explicit SpriteComponent(const boost::intrusive_ptr<Texture>& texture);
    virtual ~SpriteComponent();

    static SpriteComponent* Create();
    static long Interface();

    virtual SpriteComponent* Clone() const override;
    void InitWithComponent(const SpriteComponent* other);

    void SetTexture(const boost::intrusive_ptr<Texture>& texture);

    virtual void LoadFromProtobufMessage(const Proto::Component& message) override;
    virtual void SaveToProtobufMessage(Proto::Component* message) const override;
    virtual bool ShouldSave() override;
    virtual void Prepare() override;

    virtual void GetBindings(std::vector<Binding>* bindings) override;
    virtual BindingValue ValueForBindedProperty(int property) override;
    virtual void SetValueForBindedProperty(int property, const BindingValue& value) override;

    virtual int componentCategories() const override;
    virtual bool ImplementsInterface(long interfaceId) override;
    virtual void RegisterInterfaces(ComponentManager* manager) override;
    virtual bool HasBounds() const override;
    virtual Rectangle localAABB() const override;
    virtual bool IsVisualComponent() const override;
    virtual std::string className() const override;

    void Draw(RenderingContext* context, const Matrix4& transform) const;

    std::string m_textureName;                          // +104
    boost::shared_ptr<Sprite> m_sprite;                 // +128
    Rectangle m_localAABB;                              // +144
    bool m_flag160;                                     // +160
    unsigned char _pad161[7];                           // +161
};

// Golden aarch64 NDK-libc++ layout anchor; MSVC STL member sizes differ,
// so this assert is Android/Linux-only (the Windows build is self-consistent
// under MSVC STL and does not claim golden byte-layout parity).
#ifndef _MSC_VER
static_assert(sizeof(SpriteComponent) == 168, "SpriteComponent size must be 168 bytes");
#endif

} // namespace Caver

#endif // CAVER_SPRITECOMPONENT_H
