#ifndef CAVER_PARTICLECOMPONENT_H
#define CAVER_PARTICLECOMPONENT_H

#include "Component.h"
#include "../render/Texture.h"
#include "../animation/ParticleSystem.h"
#include <boost/intrusive_ptr.hpp>
#include <string>

namespace Caver {

class ParticleComponent : public Component {
public:
    ParticleComponent();
    virtual ~ParticleComponent();

    static ParticleComponent* Create();
    static long Interface();

    virtual Component* Clone() const;
    void InitWithComponent(const ParticleComponent* other);
    virtual void LoadFromProtobufMessage(const Proto::Component& message);
    virtual void SaveToProtobufMessage(Proto::Component* message) const;
    virtual bool ShouldSave();
    virtual void Prepare();
    virtual void GetBindings(std::vector<Binding>* bindings);
    virtual void GetEnumValuesForBindedProperty(
        int property, std::vector<BindingValue>* values);
    virtual BindingValue ValueForBindedProperty(int property);
    virtual void SetValueForBindedProperty(
        int property, const BindingValue& value);
    virtual void PerformBindedAction(int action);
    virtual bool ImplementsInterface(long interfaceId);
    virtual void RegisterInterfaces(ComponentManager* manager);
    virtual std::string className() const;

    void UpdateDimensions();
    void SetSize(float size);
    void AddTypeToParticleSystem(ParticleSystem* particleSystem);

    std::string m_textureName;                    // +104
    boost::intrusive_ptr<Texture> m_texture;      // +128
    float m_size;                                 // +136
    float m_width;                                // +140
    float m_height;                               // +144
    int m_field148;                               // +148
};

// Golden aarch64 NDK-libc++ layout anchor; MSVC STL member sizes differ,
// so this assert is Android/Linux-only (the Windows build is self-consistent
// under MSVC STL and does not claim golden byte-layout parity).
#ifndef _MSC_VER
static_assert(sizeof(ParticleComponent) == 0x98,
              "ParticleComponent size must be 152 bytes");
#endif

} // namespace Caver

#endif
