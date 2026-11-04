// Recovered from libswordigo.so (1.4.13) evidence — IDA exports in
// ~/workspace/analysis/swordigo/functions/Caver/GUISlider/, the ABS64
// relocations of the golden vtable, and the ctor/dtor bodies.
//
// GUISlider: a GUIControl horizontal slider — a value in [min, max]
// rendered as a three-texture composite (frame plate, filled bar,
// peg/thumb) built into a lazily created Sprite, with direct
// touch-to-value tracking.
//
// Vtable _ZTVN5Caver9GUISliderE @0x635230, 328 bytes = the 39
// GUIControl slots (38 GUIView slots + slot 38 StateDidChange, which
// stays GUIControl's) with these overrides (ABS64 relocations):
//   0 TouchBegan, 1 TouchMoved, 2 TouchEnded, 3 TouchCancelled,
//   7/8 ~GUISlider D1/D0, 9 Update, 10 DrawRect, 11 LayoutSubviews.
// RTTI @0x635378 is a __si_class_type_info whose single base is
// Caver::GUIControl (@0x635388 -> _ZTIN5Caver10GUIControlE).
//
// Layout: GUIControl (280 bytes), then (all offsets byte-verified
// against the ctor @0x524BC8 stores and every method body):
//   +280 int field280 (ctor zeroes it; no reader in this class —
//        name UNRECOVERED)
//   +284 float value (the current value: every setter writes it and
//        consumers read it — SettingsView::SliderValueChanged reads
//        the slider at +284; GameInterfaceBuilder::Slider writes it)
//   +288 float displayValue (the operative value used by Update's
//        animation step, IndicatorCenter, DrawRect and UpdateSprite;
//        the setters keep it equal to value — name UNRECOVERED)
//   +292 float velocity (Update's animation rate; every setter and
//        touch handler in this build only ever writes 0 — name
//        UNRECOVERED)
//   +296 bool field296 (set by Update while the value is animating,
//        cleared when the velocity decays — name UNRECOVERED)
//   +300 float minValue (ctor default 0.0)
//   +304 float maxValue (ctor default 100.0)
//   +312 intrusive_ptr<Texture> frameTexture (SetFrameTexture)
//   +320 intrusive_ptr<Texture> barTexture (SetBarTexture)
//   +328 intrusive_ptr<Texture> pegTexture (SetPegTexture)
//   +336 Sprite* sprite (RAW pointer: created by UpdateSprite,
//        torn down by hand in ~GUISlider — delete[] of its index
//        buffer at +56 and vertex buffer at +32, release of its
//        texture at +0, then operator delete)
//   +344 bool spriteDirty (set by LayoutSubviews, the setters, the
//        touch handlers and Update; DrawRect rebuilds the sprite
//        through UpdateSprite and clears it)
//   +345 bool tracking (a touch is down on the peg/track;
//        TouchCancelled/TouchEnded clear it — name UNRECOVERED)
//   +348 Vector2 touchOffset (touch location minus the indicator
//        centre, recorded by TouchBegan — name UNRECOVERED)
//   +356 bool roundValue (value writes go through roundf when set;
//        GameInterfaceBuilder::Slider honours it — name UNRECOVERED)
//   +357 bool field357 (when set, the bar is neither built into the
//        sprite nor drawn as a fallback rect — name UNRECOVERED)
// sizeof = 360 (0x168), proven by the ctor's last store at +357 and
// the desktop SRE field map's size for this class.
#ifndef CAVER_GUISLIDER_H
#define CAVER_GUISLIDER_H

#include <boost/intrusive_ptr.hpp>

#include "../core/Vector2.h"
#include "../render/Texture.h"
#include "GUIControl.h"

namespace Caver {

class Sprite;

class GUISlider : public GUIControl {
public:
    GUISlider();
    ~GUISlider();

    void TouchBegan(const FWTouch& touch) override;      // slot 0
    void TouchMoved(const FWTouch& touch) override;      // slot 1
    void TouchEnded(const FWTouch& touch) override;      // slot 2
    void TouchCancelled(const FWTouch& touch) override;  // slot 3
    void Update(float deltaTime) override;               // slot 9
    void DrawRect(RenderingContext* context, const Rectangle& rect,
                  const Matrix4& transform) override;    // slot 10
    void LayoutSubviews() override;                      // slot 11

    void SetFrameTexture(const boost::intrusive_ptr<Texture>& texture);
    void SetBarTexture(const boost::intrusive_ptr<Texture>& texture);
    void SetPegTexture(const boost::intrusive_ptr<Texture>& texture);
    void UpdateSprite();
    Vector2 IndicatorCenter();
    Vector2 IndicatorSize();
    float MinIndicatorCenterX();
    float MaxIndicatorCenterX();
    void SetValueFromViewLocation(float x);

    int field280;                                // +280 UNRECOVERED
    float value;                                 // +284
    float displayValue;                          // +288 UNRECOVERED name
    float velocity;                              // +292 UNRECOVERED name
    bool field296;                               // +296 UNRECOVERED
    char _pad297[3];                             // +297
    float minValue;                              // +300
    float maxValue;                              // +304
    char _pad308[4];                             // +308
    boost::intrusive_ptr<Texture> frameTexture;  // +312
    boost::intrusive_ptr<Texture> barTexture;    // +320
    boost::intrusive_ptr<Texture> pegTexture;    // +328
    Sprite* sprite;                              // +336 (raw, see above)
    bool spriteDirty;                            // +344
    bool tracking;                               // +345 UNRECOVERED name
    char _pad346[2];                             // +346
    Vector2 touchOffset;                         // +348 UNRECOVERED name
    bool roundValue;                             // +356 UNRECOVERED name
    bool field357;                               // +357 UNRECOVERED
    char _pad358[2];                             // +358
};

} // namespace Caver

#endif
