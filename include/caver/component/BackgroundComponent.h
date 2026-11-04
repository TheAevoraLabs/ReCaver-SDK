// Recovered from libswordigo.so (1.4.13) evidence.
// A Component (104 bytes) plus its own members:
//   +104 pointer field, initialised to 0
//   +112 float, initialised to -1.0
//   +120 boost::shared_ptr<Sprite>, reset to a fresh zero-initialised
//        88-byte Sprite by the constructor body
// Object size 136 (see Create()'s operator new(0x88)).
#ifndef CAVER_BACKGROUNDCOMPONENT_H
#define CAVER_BACKGROUNDCOMPONENT_H

#include "Component.h"
#include "../render/Sprite.h"
#include <boost/shared_ptr.hpp>

namespace Caver {

class Camera;
class RenderingContext;

class BackgroundComponent : public Component {
public:
    BackgroundComponent();
    virtual ~BackgroundComponent() {}

    static BackgroundComponent* Create();
    static long Interface();

    Component* Clone() const override;
    bool ShouldSave() override;
    void Prepare() override;
    int componentCategories() const override;
    bool ImplementsInterface(long interfaceId) override;
    void RegisterInterfaces(ComponentManager* manager) override;
    std::string className() const override;

    void InitWithComponent(const BackgroundComponent* other);

    void LoadFromProtobufMessage(const Proto::Component& message) override;
    void SaveToProtobufMessage(Proto::Component* message) const override;

    BindingValue ValueForBindedProperty(int property) override;
    void SetValueForBindedProperty(int property,
                                   const BindingValue& value) override;
    void GetBindings(std::vector<Binding>* bindings) override;

    virtual void Draw(RenderingContext* context, Camera* camera) const;

protected:
    float m_field104[2];                 // +104 parallax factor pair
                                         // (proven by Draw: loaded as a
                                         // float2 and scaled; was a void*
                                         // placeholder — see SHARED.md)
    float m_field112;                    // +112 = -1.0f
    unsigned char m_pad116[4];
    boost::shared_ptr<Sprite> m_sprite;  // +120
};

} // namespace Caver

#endif
