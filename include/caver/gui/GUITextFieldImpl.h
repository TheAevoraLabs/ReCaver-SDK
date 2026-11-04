// Recovered from libswordigo.so (1.4.13) evidence — IDA/Hex-Rays exports
// in ~/workspace/analysis/swordigo/functions/Caver/GUITextFieldImpl/ and
// Caver/GUITextField/, the ABS64 relocations of the golden vtables, and
// the ctor/dtor bodies.
//
// GUITextFieldImpl is the concrete text field: a GUITextField (itself a
// GUIControl) that also implements the platform text-input delegate
// interface as a secondary base at +312.
//
// Vtables:
//   _ZTVN5Caver12GUITextFieldE    @0x6364F8 (488 bytes): GUIControl's 39
//     slots (0-38) plus 20 pure virtuals (39-58, all __cxa_pure_virtual).
//   _ZTVN5Caver16GUITextFieldImplE @0x636280 (536 bytes): primary address
//     point +16 with 61 slots — slots 0-38 as GUIView/GUIControl (with
//     Impl overrides at 2 TouchEnded, 7/8 dtors, 9 Update, 10 DrawRect,
//     11 LayoutSubviews, 24 CanBecomeFirstResponder, 25/26 Become/Resign
//     FirstResponder), slots 39-58 the GUITextField interface below (all
//     Impl), slots 59/60 TextInputTextDidChange / TextInputDidFinish
//     (the ITextInputDelegate overriders, which clang also places in the
//     primary vtable); then the ITextInputDelegate secondary address
//     point @0x636488 (offset-to-top -312) with the two _ZThn312 thunks.
//   RTTI: GUITextField is a __si_class_type_info over GUIControl;
//     GUITextFieldImpl is a __vmi_class_type_info with exactly two bases
//     (GUITextField @0x6364D8, ITextInputDelegate @0x6364E8);
//     ITextInputDelegate and IGUITextFieldDelegate are plain
//     __class_type_info (no bases, no vtable of their own in the binary).
//
// Layout (sizeof 440 = 0x1B8, proven by NewGUITextFieldImpl's
// operator new(0x1B8) @0x53F640 and the ctor/dtor accesses):
//   +0   GUIView/GUIControl/GUITextField primary base (312 bytes total,
//        see below)
//   +312 ITextInputDelegate secondary base (vptr only)
//   +320 boost::shared_ptr<GUILabel> label (member name UNRECOVERED)
//   +336 boost::shared_ptr<Font> font
//   +352 Color textColor (ctor inits it to 0xFF000000)
//   +356 GUIMargins textMargins (20 bytes)
//   +376 bool multiline
//   +377 bool immediateUpdates
//   +384 std::string text
//   +408 std::string placeholder
//   +432 bool editing (name UNRECOVERED) — set/cleared together with
//        +433 as a 16-bit store of 257 / 0
//   +433 bool cursorVisible (name UNRECOVERED) — blinked by Update
//   +436 float cursorTimer (name UNRECOVERED) — blink period 0.5
//
// GUITextField layout (sizeof 312): GUIControl (304: GUIView 256 +
// multimap @+256), then
//   +288 boost::shared_ptr<GUIFrameView> frameView (member name
//        UNRECOVERED; proven by GUITextField::frameView @0x462F68, which
//        lazily creates a GUIFrameView there, and by ~GUITextField
//        @0x5405B4 releasing the counted pointer at +296)
//   +304 IGUITextFieldDelegate* delegate (raw; member name UNRECOVERED;
//        proven by TextInputDidFinish / ResignFirstResponder calling
//        its slots 1 / 0 with this text field)
//
// NOTE on Color: in this class's golden code, Color is NOT trivial for
// calls — textColor() returns through the hidden sret pointer and
// setTextColor(Color) loads its argument through a pointer
// (_ZN...12setTextColorENS_5ColorE @0x5404A8: ldr w8,[x1]). That is
// only possible if Caver::Color has a user-defined copy constructor
// (the same engine pattern proven for Vector2/Vector3/FloatColor by
// the binding slice). The in-tree src/caver/core/Color.h declares a
// trivial Color; the definition below carries the proven copy ctor.
// MERGE NOTE: do not include core/Color.h in the same TU as this
// header until core/Color.h gains the user-defined copy constructor.
#ifndef CAVER_GUITEXTFIELDIMPL_H
#define CAVER_GUITEXTFIELDIMPL_H

#include <string>

#include <boost/shared_ptr.hpp>

#include "GUIControl.h"
#include "GUIEventTypes.h"
#include "GUIMargins.h"
#include "../core/Color.h"
#include "../core/Rectangle.h"
#include "../core/Vector2.h"

namespace Caver {

class Font;
class GUIFrameView;
class GUILabel;
class RenderingContext;

class GUITextField;

// Delegate notified when editing ends / the return key is pressed.
// Slot order proven by GUITextFieldImpl::ResignFirstResponder (calls
// slot 0) and TextInputDidFinish (calls slot 1, tests the bool), and
// by the GUIAlertView / ProfilePanelView implementors + their thunks.
class IGUITextFieldDelegate {
public:
    virtual void TextFieldDidEndEditing(GUITextField* textField) = 0;  // slot 0
    virtual bool TextFieldShouldReturn(GUITextField* textField) = 0;   // slot 1
};

// Platform text-input delegate (secondary base of GUITextFieldImpl at
// +312). Slot order proven by the secondary vtable @0x636488:
// _ZThn312 TextInputTextDidChange, then _ZThn312 TextInputDidFinish.
// TextInputTextDidChange returns the filtered text by value (the
// golden body @0x53FCA8 writes through the hidden sret pointer in x8
// and takes only the const std::string& argument in x1).
class ITextInputDelegate {
public:
    virtual std::string TextInputTextDidChange(const std::string& text) = 0;  // slot 0
    virtual void TextInputDidFinish() = 0;                                    // slot 1
};

// Platform text-input entry points (defined by the platform slice,
// @0x508FC4 / @0x5090FC). Start is declared with a bool result
// because every golden caller tail-branches to it and lets its x0
// flow out as their own return value (StartEditing,
// BecomeFirstResponder, TouchEnded); its body returns whatever the
// JNI CallStaticVoidMethod wrapper leaves in x0.
void StartTextInputWithDelegate(ITextInputDelegate* delegate,
                                const std::string& text);
void StopTextInputWithDelegate(ITextInputDelegate* delegate);

class GUITextField : public GUIControl {
public:
    static GUITextField* New();
    GUIFrameView* frameView();

    // Inline in the original (the golden ~GUITextField D2 @0x5405B4 is
    // a WEAK 132-byte symbol): releases the frameView counted pointer
    // at +296, then tears down GUIControl.
    virtual ~GUITextField() {}

    // GUITextField interface — vtable slots 39-58, pure in GUITextField
    // (its vtable holds __cxa_pure_virtual in every one of these).
    virtual const boost::shared_ptr<Font>& font() const = 0;              // 39
    virtual void setFont(const boost::shared_ptr<Font>& font) = 0;        // 40
    virtual Color textColor() const = 0;                                  // 41
    virtual void setTextColor(Color color) = 0;                           // 42
    virtual const std::string& text() const = 0;                          // 43
    virtual void setText(const std::string& text) = 0;                    // 44
    virtual const std::string& placeholder() const = 0;                   // 45
    virtual void setPlaceHolder(const std::string& placeholder) = 0;      // 46
    virtual GUITextAlignment horizontalTextAlignment() const = 0;         // 47
    virtual void setHorizontalTextAlignment(GUITextAlignment alignment) = 0;  // 48
    virtual GUITextAlignment verticalTextAlignment() const = 0;           // 49
    virtual void setVerticalTextAlignment(GUITextAlignment alignment) = 0;    // 50
    virtual GUIMargins textMargins() const = 0;                           // 51
    virtual void setTextMargins(const GUIMargins& margins) = 0;           // 52
    virtual Rectangle textBounds() const = 0;                             // 53
    virtual Vector2 TextSize() const = 0;                                 // 54
    virtual bool isMultiline() const = 0;                                 // 55
    virtual void setMultiline(bool multiline) = 0;                         // 56
    virtual bool immediateUpdates() const = 0;                            // 57
    virtual void setImmediateUpdates(bool immediate) = 0;                 // 58

    // The golden GUIControl subobject occupies 288 bytes: its
    // std::multimap tree writes a dword at +280 and a byte at +284 in
    // derived-class ctors (e.g. GUIButton's ctor @0x51DE90), i.e. the
    // golden libc++ __tree spans +256..+287. The NDK libc++ in this
    // toolchain has a 24-byte __tree (+256..+279), so 8 bytes of the
    // golden tree tail are modelled explicitly here to keep every
    // proven offset in this class (and in GUITextFieldImpl) exact.
    // MERGE NOTE: when GUIControl itself is rebuilt over a
    // golden-layout tree type, this pad goes away.
    // The word carries a zero NSDMI because the golden tree init
    // emits that store in derived ctors (Impl ctor, +280) and ours
    // must emit it too.
    struct GoldenTreeTail {
        int _word = 0;
        char _rest[4];
    };
    GoldenTreeTail _goldenTreeTail;               // +280 (golden tree tail)
    boost::shared_ptr<GUIFrameView> m_frameView;  // +288 (name UNRECOVERED)
    IGUITextFieldDelegate* m_delegate;            // +304 (name UNRECOVERED)
};

class GUITextFieldImpl : public GUITextField, public ITextInputDelegate {
public:
    GUITextFieldImpl();
    virtual ~GUITextFieldImpl();

    // GUIResponder / GUIView / GUIControl overrides.
    virtual void TouchEnded(const FWTouch& touch);                 // slot 2
    virtual void Update(float deltaTime);                          // slot 9
    virtual void DrawRect(RenderingContext* context,
                          const Rectangle& rect,
                          const Matrix4& transform);               // slot 10
    virtual void LayoutSubviews();                                 // slot 11
    virtual bool CanBecomeFirstResponder();                        // slot 24
    virtual bool BecomeFirstResponder();                           // slot 25
    virtual bool ResignFirstResponder();                           // slot 26

    // GUITextField interface. The ones defined inline here are WEAK
    // symbols in the golden (defined in the original header); the
    // rest are defined in GUITextFieldImpl.cpp.
    virtual const boost::shared_ptr<Font>& font() const {         // slot 39
        return m_font;
    }
    virtual void setFont(const boost::shared_ptr<Font>& font);    // slot 40
    virtual Color textColor() const {                             // slot 41
        return m_textColor;
    }
    virtual void setTextColor(Color color) {                      // slot 42
        m_textColor = color;
        UpdateText();
    }
    virtual const std::string& text() const {                     // slot 43
        return m_text;
    }
    virtual void setText(const std::string& text) {               // slot 44
        m_text = text;
        UpdateText();
    }
    virtual const std::string& placeholder() const {              // slot 45
        return m_placeholder;
    }
    virtual void setPlaceHolder(const std::string& placeholder) {  // slot 46
        m_placeholder = placeholder;
        UpdateText();
    }
    virtual GUITextAlignment horizontalTextAlignment() const;     // slot 47
    virtual void setHorizontalTextAlignment(GUITextAlignment alignment);  // 48
    virtual GUITextAlignment verticalTextAlignment() const;       // slot 49
    virtual void setVerticalTextAlignment(GUITextAlignment alignment);    // 50
    virtual GUIMargins textMargins() const {                      // slot 51
        return m_textMargins;
    }
    virtual void setTextMargins(const GUIMargins& margins) {      // slot 52
        m_textMargins = margins;
        LayoutSubviews();
    }
    virtual Rectangle textBounds() const;                         // slot 53
    virtual Vector2 TextSize() const;                             // slot 54
    virtual bool isMultiline() const {                            // slot 55
        return m_multiline;
    }
    virtual void setMultiline(bool multiline) {                   // slot 56
        m_multiline = multiline;
    }
    virtual bool immediateUpdates() const {                       // slot 57
        return m_immediateUpdates;
    }
    virtual void setImmediateUpdates(bool immediate) {            // slot 58
        m_immediateUpdates = immediate;
    }

    // ITextInputDelegate overrides (also primary vtable slots 59/60).
    virtual std::string TextInputTextDidChange(const std::string& text);
    virtual void TextInputDidFinish();

    // Non-virtual members.
    void StartEditing(bool animated);
    void StopEditing();
    void SendValueChangedEvent();
    void UpdateText();
    void UpdateFont();

    boost::shared_ptr<GUILabel> m_label;  // +320 (name UNRECOVERED)
    boost::shared_ptr<Font> m_font;       // +336
    Color m_textColor;                    // +352
    GUIMargins m_textMargins;             // +356
    bool m_multiline;                     // +376
    bool m_immediateUpdates;              // +377
    std::string m_text;                   // +384
    std::string m_placeholder;            // +408
    bool m_editing;                       // +432 (name UNRECOVERED)
    bool m_cursorVisible;                 // +433 (name UNRECOVERED)
    float m_cursorTimer;                  // +436 (name UNRECOVERED)

private:
    // The start-editing sequence inlined into StartEditing,
    // BecomeFirstResponder and TouchEnded in the golden: set the
    // editing/cursor byte pair, reset the blink timer, and hand the
    // current text to the platform text input.
    void BeginEditing() {
        m_editing = true;
        m_cursorVisible = true;
        m_cursorTimer = 0.0f;
        StartTextInputWithDelegate(static_cast<ITextInputDelegate*>(this),
                                   m_text);
    }
};

} // namespace Caver

#endif
