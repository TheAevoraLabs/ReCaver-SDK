#ifndef CAVER_ENTITYACTIONCOMPONENT_H
#define CAVER_ENTITYACTIONCOMPONENT_H

#include "Component.h"
#include "../scripting/Program.h"
#include <boost/shared_ptr.hpp>

namespace Caver {

class EntityActionComponent : public Component {
public:
    EntityActionComponent();
    virtual ~EntityActionComponent();

    static EntityActionComponent* Create();
    static long Interface();

    virtual Component* Clone() const;
    void InitWithComponent(const EntityActionComponent* other);
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
    virtual int componentCategories() const;
    virtual bool ImplementsInterface(long interfaceId);
    virtual void RegisterInterfaces(ComponentManager* manager);
    virtual void Update(float deltaTime);
    virtual bool HandleMessage(int message, void* data);
    virtual std::string className() const;

    void Perform();
    bool IsFinished();

    Program m_onActivate;                              // +104
    boost::shared_ptr<ProgramState> m_programState;   // +160
    float m_elapsed;                                  // +176
};

// Golden aarch64 NDK-libc++ layout anchor; MSVC STL member sizes differ,
// so this assert is Android/Linux-only (the Windows build is self-consistent
// under MSVC STL and does not claim golden byte-layout parity).
#ifndef _MSC_VER
static_assert(sizeof(EntityActionComponent) == 0xB8,
              "EntityActionComponent size must be 184 bytes");
#endif

} // namespace Caver

#endif
