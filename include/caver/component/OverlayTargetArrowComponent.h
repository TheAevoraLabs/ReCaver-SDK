#ifndef CAVER_OVERLAYTARGETARROWCOMPONENT_H
#define CAVER_OVERLAYTARGETARROWCOMPONENT_H

#include "OverlayComponent.h"
#include "../core/Vector2.h"
#include <boost/intrusive_ptr.hpp>
#include <vector>

namespace Caver {

class Camera;
class ComponentManager;
class SceneObject;
class Sprite;
class Texture;

class OverlayTargetArrowComponent : public OverlayComponent {
public:
    OverlayTargetArrowComponent();
    virtual ~OverlayTargetArrowComponent();
    static OverlayTargetArrowComponent* Create();
    static long Interface();
    virtual OverlayTargetArrowComponent* Clone() const;
    void InitWithComponent(const OverlayTargetArrowComponent* other);
    virtual void LoadFromProtobufMessage(const Proto::Component& message);
    virtual void SaveToProtobufMessage(Proto::Component* message) const;
    virtual bool ShouldSave();
    virtual void GetBindings(std::vector<Binding>* bindings);
    virtual void GetEnumValuesForBindedProperty(int property,
                                                std::vector<BindingValue>* values);
    virtual BindingValue ValueForBindedProperty(int property);
    virtual void SetValueForBindedProperty(int property,
                                           const BindingValue& value);
    virtual void PerformBindedAction(int action);
    virtual void Prepare();
    virtual bool RequiresUpdate() const;
    virtual bool ImplementsInterface(long interfaceId);
    virtual void RegisterInterfaces(ComponentManager* manager);
    virtual std::string className() const;
    virtual void Update(float deltaTime);
    virtual void Draw(RenderingContext* context, const Camera* camera,
                      const Rectangle& rect);

    void SetIconTexture(const boost::intrusive_ptr<Texture>& texture);
    void AddTargetObject(const boost::intrusive_ptr<SceneObject>& object);

    boost::intrusive_ptr<SceneObject> m_targetObject; // +104
    std::vector<boost::intrusive_ptr<SceneObject> > m_targetObjects; // +112
    boost::intrusive_ptr<SceneObject> m_selectedTarget; // +136
    bool m_intersectTargetBounds; // +144
    bool m_hideWhenNear; // +145
    bool m_hidden; // +146
    char _pad147[5];
    Sprite* m_arrowSprite; // +152
    Sprite* m_iconSprite; // +160
    Vector2 m_focusPoint; // +168
};

static_assert(sizeof(OverlayTargetArrowComponent) == 0xB0,
              "OverlayTargetArrowComponent size must be 176 bytes");

} // namespace Caver

#endif
