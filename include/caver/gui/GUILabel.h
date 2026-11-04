// Recovered from libswordigo.so (1.4.13) evidence — IDA/Hex-Rays exports
// in ~/workspace/analysis/swordigo/functions/Caver/GUILabel/, the ABS64
// relocations of the golden vtable, and the ctor/dtor bodies.
//
// GUILabel: a GUIView that renders a string through a FontText
// (texture-atlas text; there is no separate font engine in the GL layer).
//
// Vtable _ZTVN5Caver8GUILabelE @0x634CA0, 320 bytes = 16-byte header +
// exactly 38 function slots — GUILabel adds NO new virtual slots; it
// overrides only:
//   7/8  ~GUILabel (D1/D2 @0x520CD8, D0 @0x520E14)
//   10   DrawRect @0x520448
//   31   SetBounds @0x520B44
// All other slots keep the GUIView/GUIResponder functions (slot list in
// GUIView.h). RTTI @0x634DE0 is a __si_class_type_info on GUIView.
//
// Layout (sizeof 368 = 0x170, proven by GUITextFieldImpl's
// operator new(0x170) for its embedded label and by the ctor @0x5202CC
// stores / dtor @0x520CD8 teardown order):
//   +0..255  GUIView base
//   +256     boost::shared_ptr<Font> font (filled from Font::SystemFont()
//            by UpdateText when null)
//   +272     std::string text (setText @0x41CC5C assigns exactly here)
//   +296     Color textColor (ctor: rgb zeroed, alpha byte at +299 = 255)
//   +304     boost::shared_ptr<GradientColor> gradient
//   +320     boost::shared_ptr<FontText> fontText (ctor: reset(new
//            FontText) — the FontText ctor is inlined into the label ctor
//            in the golden, so FontText's own ctor is header-implicit)
//   +336     GUITextAlignment horizontalTextAlignment (ctor 0)
//   +340     GUITextAlignment verticalTextAlignment (ctor 4)
//   +344     bool field344 — when set, UpdateText/AddText pass the view
//            bounds width (+156) as the FontText::AddText float argument
//            (a wrap/max width), else 0 (name UNRECOVERED)
//   +348     int field348 — copied into FontText+56 by UpdateText
//            (name UNRECOVERED)
//   +352     bool field352 — when set, the +353/+360 pair is copied into
//            FontText+40/+41/+48 by UpdateText/AddText (a shadow-style
//            triple: enable, colour, offset; names UNRECOVERED)
//   +353     Color field353 (UNALIGNED — Color's alignment is 1)
//   +360     Vector2 field360
// The ctor also clears GUIView's userInteractionEnabled (+245).
#ifndef CAVER_GUILABEL_H
#define CAVER_GUILABEL_H

#include <boost/shared_ptr.hpp>
#include <string>

#include "GUIEventTypes.h"
#include "GUIView.h"
#include "../core/Color.h"
#include "../core/Vector2.h"

namespace Caver {

class Font;
class FontText;
class GradientColor;

class GUILabel : public GUIView {
public:
    GUILabel();
    ~GUILabel() override;  // slots 7, 8

    void DrawRect(RenderingContext* context, const Rectangle& rect,
                  const Matrix4& transform) override;  // slot 10
    void SetBounds(const Rectangle& bounds) override;    // slot 31

    // Weak symbol in the golden (@0x41CC5C) — defined inline in the
    // original header: assign + UpdateText only when the text changes
    // (the golden inlines the std::string compare).
    void setText(const std::string& newText);

    void UpdateText();
    void UpdateBounds();
    void ClearText();
    void AddText(const std::string& text);
    void AddText(const std::string& text, Color color);
    void AddText(const std::string& text,
                 const boost::shared_ptr<GradientColor>& gradient);
    Vector2 TextSize() const;
    Rectangle TextBounds() const;
    Rectangle BoundsForCharacterAtIndex(int index);

    boost::shared_ptr<Font> font;                 // +256
    std::string text;                             // +272
    Color textColor;                              // +296
    char _pad300[4];                              // +300
    boost::shared_ptr<GradientColor> gradient;    // +304
    boost::shared_ptr<FontText> fontText;         // +320
    GUITextAlignment horizontalTextAlignment;     // +336
    GUITextAlignment verticalTextAlignment;       // +340
    bool field344;                                // +344 (name UNRECOVERED)
    char _pad345[3];                              // +345
    int field348;                                 // +348 (name UNRECOVERED)
    bool field352;                                // +352 (name UNRECOVERED)
    Color field353;                               // +353 (name UNRECOVERED)
    char _pad357[3];                              // +357
    Vector2 field360;                             // +360 (name UNRECOVERED)
};

} // namespace Caver

#endif
