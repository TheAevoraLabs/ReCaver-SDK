#ifndef CAVER_TEXTBUBBLECOMPONENT_H
#define CAVER_TEXTBUBBLECOMPONENT_H

#include "OverlayComponent.h"
#include "../gui/GUITextBubble.h"
#include <boost/shared_ptr.hpp>
#include <string>
#include <vector>

namespace Caver {

class Camera;
class RenderingContext;

// Recovered from libswordigo.so (1.4.13) evidence — vtable _ZTVN5Caver19TextBubbleComponentE
// at 0x61B6C0 (sizeof 0x80 = 128 bytes).
class TextBubbleComponent : public OverlayComponent {
public:
    TextBubbleComponent();
    virtual ~TextBubbleComponent();

    static TextBubbleComponent* Create();
    static long Interface();

    virtual TextBubbleComponent* Clone() const override;
    void InitWithComponent(const TextBubbleComponent* other);

    virtual void LoadFromProtobufMessage(const Proto::Component& message) override;
    virtual void SaveToProtobufMessage(Proto::Component* message) const override;
    virtual bool ShouldSave() override;
    virtual void Prepare() override;

    virtual void GetBindings(std::vector<Binding>* bindings) override;
    virtual void GetEnumValuesForBindedProperty(int property, std::vector<BindingValue>* values) override;
    virtual BindingValue ValueForBindedProperty(int property) override;
    virtual void SetValueForBindedProperty(int property, const BindingValue& value) override;
    virtual void PerformBindedAction(int action) override;

    virtual bool ImplementsInterface(long interfaceId) override;
    virtual void RegisterInterfaces(ComponentManager* manager) override;
    virtual bool RequiresUpdate() const override;
    virtual std::string className() const override;

    virtual void Update(float deltaTime) override;
    virtual bool HandleMessage(int message, void* data) override;
    virtual void Draw(RenderingContext* context, const Camera* camera, const Rectangle& rect) override;

    void RegisterLibrary();
    void ShowText(const std::string& text, float duration);
    bool IsTextFinishedShowing();
    void SetHandleTouches(bool handleTouches);

    float m_timer;                                      // +104
    bool m_handleTouches;                               // +108
    bool m_textShowing;                                 // +109
    bool m_boundsSet;                                   // +110
    char _pad111;                                       // +111
    boost::shared_ptr<GUITextBubble> m_bubble;          // +112
};

static_assert(sizeof(TextBubbleComponent) == 128, "TextBubbleComponent size must be 128 bytes");

} // namespace Caver

#endif // CAVER_TEXTBUBBLECOMPONENT_H
