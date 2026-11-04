#ifndef CAVER_SKELLYMONSTERCONTROLLERCOMPONENT_H
#define CAVER_SKELLYMONSTERCONTROLLERCOMPONENT_H

#include "MonsterControllerComponent.h"
#include "CharControllerComponent.h"
#include "ShapeComponent.h"
#include <boost/intrusive_ptr.hpp>
#include <vector>
#include <string>

namespace Caver {

class SceneObject;

class SkellyMonsterControllerComponent : public MonsterControllerComponent {
public:
    SkellyMonsterControllerComponent();
    virtual ~SkellyMonsterControllerComponent();

    static SkellyMonsterControllerComponent* Create();
    static long Interface();

    virtual Component* Clone() const;                                       // slot 2
    void InitWithComponent(const SkellyMonsterControllerComponent* other);  // narrowed non-virtual
    virtual void LoadFromProtobufMessage(const Proto::Component& message);  // slot 4
    virtual void SaveToProtobufMessage(Proto::Component* message) const;    // slot 5
    virtual bool ShouldSave();                                              // slot 6
    virtual bool Process(std::vector<boost::intrusive_ptr<Component>>* components); // slot 7
    virtual void Prepare();                                                 // slot 8
    virtual void GetBindings(std::vector<Binding>* bindings);               // slot 14
    virtual void GetEnumValuesForBindedProperty(int property, std::vector<BindingValue>* values); // slot 15
    virtual BindingValue ValueForBindedProperty(int property);              // slot 16
    virtual void SetValueForBindedProperty(int property, const BindingValue& value); // slot 17
    virtual void PerformBindedAction(int action);                           // slot 18
    virtual bool ImplementsInterface(long interfaceId);                     // slot 20
    virtual void RegisterInterfaces(ComponentManager* manager);             // slot 21
    virtual void Update(float deltaTime);                                   // slot 25
    virtual bool HandleMessage(int message, void* data);                    // slot 26
    virtual std::string className() const;                                  // slot 32

    void CreateWeapons();
    void Leap();

    // Member layout: sizeof 0x120 (288 bytes)
    ComponentOutlet<CharControllerComponent> m_charControllerOutlet;        // +200 — ID 100 "charController"
    ComponentOutlet<ShapeComponent> m_attackAreaOutlet;                     // +224 — ID 101 "attackArea"
    boost::intrusive_ptr<SceneObject> m_rightWeaponObject;                  // +248
    boost::intrusive_ptr<SceneObject> m_leftWeaponObject;                   // +256
    bool m_canLeap;                                                         // +264 (ctor true)
    char _pad265[3];                                                        // +265
    float m_leapTimer;                                                      // +268 (ctor FLT_MAX)
    float m_attackTimer;                                                    // +272
    int m_attackCount;                                                      // +276 (ctor 0)
    float m_cachedWalkSpeed;                                                // +280 (from charController+608 in Prepare)
    bool m_weaponsAdded;                                                    // +284 (ctor false, message 5 / 2)
    char _pad285[3];                                                        // +285
};

static_assert(sizeof(SkellyMonsterControllerComponent) == 0x120,
              "SkellyMonsterControllerComponent sizeof must be exactly 288 bytes (0x120)");

} // namespace Caver

#endif // CAVER_SKELLYMONSTERCONTROLLERCOMPONENT_H
