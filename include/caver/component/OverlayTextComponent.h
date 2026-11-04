// Recovered from libswordigo.so (1.4.13) evidence — IDA exports in
// ~/workspace/analysis/swordigo/functions/Caver/OverlayTextComponent/.
// Overlay text component: draws a floating text (via FontText) and/or a
// sprite above its owner object, with a timed fade-out. sizeof 0xF0
// (240), proven by Create()'s operator new(0xF0).
//
// Bases: OverlayTextComponent : OverlayComponent : Component.
// OverlayComponent itself adds NO members over Component (its ctor
// @0x3380D8 only re-stores the Component field initialisation and its
// own vtable pointers) — sizeof(Component)==104, so this class's first
// member sits at +104. The OverlayComponent declaration below is a
// minimal local declaration (the class's own header belongs to the
// components lane; only the signatures OverlayTextComponent's bodies
// call or override are declared, all proven from the golden symbols):
// its Load/Save/GetBindings/GetEnumValues/ValueFor/SetValueFor bodies
// are 4-byte tail branches to Component's, PerformBindedAction is an
// empty override, ShouldSave returns true, componentCategories
// returns 8, className is "Overlay".
#ifndef CAVER_OVERLAYTEXTCOMPONENT_H
#define CAVER_OVERLAYTEXTCOMPONENT_H

#include "Component.h"
#include "OverlayComponent.h"
#include "../core/Vector2.h"
#include "../core/Vector3.h"
#include "../core/Color.h"
#include "../gui/FontText.h"
#include <boost/intrusive_ptr.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <string>

namespace Caver {

class Camera;
class GradientColor;
class RenderingContext;
class SceneObject;
class Sprite;

class OverlayTextComponent : public OverlayComponent {
public:
    OverlayTextComponent();
    virtual ~OverlayTextComponent();

    static OverlayTextComponent* Create();
    static long Interface();

    virtual OverlayTextComponent* Clone() const;
    // NOT an override (narrowed EPKS0_ overload, Health pattern).
    void InitWithComponent(const OverlayTextComponent* other);
    virtual void LoadFromProtobufMessage(const Proto::Component& message);
    virtual void SaveToProtobufMessage(Proto::Component* message) const;
    virtual bool ShouldSave();
    virtual void GetBindings(std::vector<Binding>* bindings);
    virtual void GetEnumValuesForBindedProperty(int property, std::vector<BindingValue>* values);
    virtual BindingValue ValueForBindedProperty(int property);
    virtual void SetValueForBindedProperty(int property, const BindingValue& value);
    virtual void PerformBindedAction(int action);
    virtual void Prepare();
    void RegisterLibrary();
    virtual bool ImplementsInterface(long interfaceId);
    virtual void RegisterInterfaces(ComponentManager* manager);
    virtual bool RequiresUpdate() const;
    virtual std::string className() const;
    virtual void Update(float deltaTime);

    void SetText(const std::string& text);
    void SetTextOffset(const Vector2& offset);
    void SetSpriteName(const std::string& name);
    void SetSpriteOffset(const Vector2& offset);
    // Parameter NAMES unrecovered: the first argument lands in the
    // fade-duration field (+212), the second in the fade-delay (+208).
    void FadeOut(float duration, float delay);
    void UpdateText();
    void UpdateSprite();
    void Draw(RenderingContext* context, const Camera* camera, const Rectangle& rect);
    void CombatTextFromObject(const std::string& text,
                              const boost::intrusive_ptr<SceneObject>& object,
                              const Vector3& offset, float duration);

    std::string m_text;                             // +104 — proto ext field 1 "text"
    Vector2 m_textOffset;                           // +128 — proto ext field 2 "text_offset"
    bool m_textDirty;                               // +136 — set by SetText/SetValue(100)/Lua SetText; cleared by UpdateText
    bool m_centered;                                // +137 — set only by CombatTextFromObject; Draw centres the text when set (name UNRECOVERED)
    char _pad138[6];                                // +138
    boost::scoped_ptr<FontText> m_fontText;         // +144 — lazily created by UpdateText, deleted when the text becomes empty
    std::string m_spriteName;                       // +152 — proto ext field 3 "sprite_name"
    Vector2 m_spriteOffset;                         // +176 — proto ext field 4 "sprite_offset"
    boost::scoped_ptr<Sprite> m_sprite;             // +184 — lazily created by UpdateSprite
    bool m_spriteDirty;                             // +192 — set by SetSpriteName/SetValue(103); cleared by UpdateSprite
    char _pad193[3];                                // +193
    float m_alpha;                                  // +196 — 1.0f in ctor; driven by the fade in Update; name UNRECOVERED
    bool m_fading;                                  // +200 — fade in progress (name UNRECOVERED)
    char _pad201[3];                                // +201
    float m_fadeElapsed;                            // +204 — NOT initialised by the ctor (bug-for-bug); FadeOut/CombatText zero it
    float m_fadeDelay;                              // +208 — NOT initialised by the ctor (bug-for-bug)
    float m_fadeDuration;                           // +212 — NOT initialised by the ctor (bug-for-bug)
    Color m_color;                                  // +216 — white (0xFFFFFFFF) in ctor; passed to FontText::SetColor (name UNRECOVERED)
    char _pad220[4];                                // +220
    boost::shared_ptr<GradientColor> m_gradient;    // +224 — copied into the FontText on creation (name UNRECOVERED)
};

} // namespace Caver

#endif
