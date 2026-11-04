// Recovered from libswordigo.so (1.4.13) evidence — IDA exports in
// ~/workspace/analysis/swordigo/functions/Caver/GUIButton/, the ABS64
// relocations of the golden vtable, and the ctor/dtor bodies.
//
// GUIButton: a GUIControl push button — up to three GUIRoundedRect
// frame layers (normal / highlighted / background), an optional
// GUITexturedRect image (plus a highlighted variant), a lazily created
// GUILabel title, a click sound, and the GUIControl state word driven
// by its touch handlers.
//
// Vtable _ZTVN5Caver9GUIButtonE @0x6346E0, 328 bytes = the 39
// GUIControl slots (38 GUIView slots + slot 38 StateDidChange) with
// these overrides (ABS64 relocation dump):
//   0 TouchBegan, 1 TouchMoved, 2 TouchEnded, 3 TouchCancelled,
//   7/8 ~GUIButton D1/D0, 10 DrawRect, 11 LayoutSubviews,
//   38 StateDidChange (overrides GUIControl's empty base body).
// All other slots keep the GUIView/GUIResponder functions. RTTI
// @0x634828 is a __si_class_type_info whose single base is
// Caver::GUIControl (@0x634988). GUIButton adds NO new virtual slots.
//
// Layout: GUIControl (280 bytes — see the arbitration note below),
// then (all offsets byte-verified against the ctor @0x51DE90 stores,
// the dtor @0x482F04 teardown order, and every method body):
//   +280 int state (the GUIControl::State word: bit 0 = highlighted /
//        pressed, bit 1 = disabled; the touch handlers rewrite it and
//        dispatch StateDidChange(oldState) through vtable slot 38
//        whenever it changes)
//   +284 bool field284 (DrawRect returns immediately while set; no
//        other reader/writer in the class — name UNRECOVERED)
//   +288 boost::scoped_ptr<GUIRoundedRect> frameRect (the normal-state
//        frame; created by SetButtonType(1) with texture
//        "ui_roundrect", recoloured per-draw with tintColor)
//   +296 boost::scoped_ptr<GUIRoundedRect> highlightedFrameRect
//        (installed by SetFrameTextureForState with the state bit set;
//        drawn instead of frameRect while highlighted)
//   +304 boost::scoped_ptr<GUIRoundedRect> backgroundRect (created by
//        SetButtonType(1) with texture "ui_roundrectbg"; recoloured
//        with backgroundColor, or with the constant 0xFFFF7840 dword
//        while highlighted — see StateDidChange)
//   +312 GUILabel* titleLabel (RAW observer pointer: the label is
//        created by titleLabel(), owned by the subview tree through
//        AddSubview, and merely detached + forgotten by SetTitle(""))
//   +320 boost::scoped_ptr<GUITexturedRect> imageRect (SetImage)
//   +328 boost::scoped_ptr<GUITexturedRect> highlightedImageRect
//        (SetHighlightedImage; drawn instead of imageRect while
//        highlighted)
//   +336 Matrix4 field336 (initialised to the identity matrix by the
//        ctor; DrawRect multiplies it by the image-centre translation
//        — name UNRECOVERED)
//   +400 GUIButtonType buttonType (written by SetButtonType; the ctor
//        zeroes it before tail-calling SetButtonType(type))
//   +404 float field404 (zeroed by the ctor; no reader/writer in any
//        GUIButton body — name/purpose UNRECOVERED)
//   +408 float labelInsets[4]: left (+408), right (+412), top (+416),
//        bottom (+420) — LayoutSubviews insets the title label's frame
//        from the button bounds by these amounts (all zero by default)
//   +424 Vector2 imageOffset (added to the bounds centre when DrawRect
//        positions the image; both components zero by default)
//   +432 Color backgroundColor (ctor: 0)
//   +436 Color tintColor (ctor: -1 = white; halved per channel by
//        DrawRect while the disabled bit is set)
//   +440 std::string soundName (played by TouchBegan through
//        AudioSystem::PlaySoundEffectWithName at volume 1.0 when
//        non-empty; member name UNRECOVERED)
//   +464 unsigned long long touchIdentifier (the tracked FWTouch's
//        identifier; 0 = no active touch)
// sizeof = 472 (0x1D8), proven by the ctor's final zero stores at
// +440..+471 (the string and the touch identifier).
//
// ------------------------------------------------------------------
// ARBITRATION VERDICT — golden GUIControl sizeof is 280, NOT 288.
// (This worker was assigned to arbitrate the dispute between the
// in-tree GUIControl.h model (multimap @+256, sizeof 280) and the
// textfieldimpl worker's model (golden libc++ __tree tail is 32B, so
// GUIControl would end at +288 and the stores below would be tree
// internals). The 280 model WINS; the deciding evidence, all from
// GUIButton's own bodies:)
//
//  1. The ctor @0x51DE90 completes the multimap initialisation with
//     exactly the three stores a 24-byte libc++ __tree needs:
//     +256 = this+264 (begin_node = &end_node), +264 = 0
//     (end_node.__left_), +272 = 0 (size). The +280 dword and the
//     +284 byte are stored by SEPARATE instructions interleaved with
//     the other GUIButton member initialisations (the dword store at
//     0x51DECC precedes the vptr store; the byte store at 0x51DEE4
//     follows the +256 store) — not as part of a tree-init block, and
//     a __tree has no byte-sized field at +284 under any layout.
//  2. GUIButton's own logic reads and writes +280 as its control
//     state word in six functions: TouchBegan (tests bits 0-1, sets
//     bit 0), TouchMoved / TouchEnded / TouchCancelled / CancelPress
//     (set/clear bit 0 and dispatch StateDidChange through vtable
//     slot 38 when the word changes), StateDidChange itself (tests
//     bit 0 of the stored word against bit 0 of its argument) and
//     DrawRect (tests bits 0 and 1). A tree-internal tail field would
//     never be read as state bits by button logic.
//  3. The +284 byte is read by GUIButton::DrawRect @0x51E240 as its
//     first test (`if (this[284]) return;`) — a button draw flag,
//     not allocator/tree data.
//  4. Cross-class consistency: GUISlider reads its +284 float through
//     external consumers (SettingsView::SliderValueChanged reads the
//     slider value at +284) and GUISwitch fires StateDidChange off
//     its +280 state word with the same open-coded idiom — in every
//     GUIControl subclass examined, +280/+284 are the subclass's own
//     first members.
// ------------------------------------------------------------------
#ifndef CAVER_GUIBUTTON_H
#define CAVER_GUIBUTTON_H

#include <boost/intrusive_ptr.hpp>
#include <boost/scoped_ptr.hpp>
#include <string>

#include "../core/Color.h"
#include "../core/Matrix4.h"
#include "../core/Vector2.h"
#include "GUIControl.h"
#include "GUIEventTypes.h"

namespace Caver {

class GUILabel;
class GUIRoundedRect;
class GUITexturedRect;
class RenderingContext;
class Texture;

class GUIButton : public GUIControl {
public:
    GUIButton(GUIButtonType type);
    ~GUIButton();

    void TouchBegan(const FWTouch& touch) override;      // slot 0
    void TouchMoved(const FWTouch& touch) override;      // slot 1
    void TouchEnded(const FWTouch& touch) override;      // slot 2
    void TouchCancelled(const FWTouch& touch) override;  // slot 3
    void DrawRect(RenderingContext* context, const Rectangle& rect,
                  const Matrix4& transform) override;    // slot 10
    void LayoutSubviews() override;                      // slot 11
    void StateDidChange(GUIControl::State state) override;  // slot 38

    void SetButtonType(GUIButtonType type);
    GUILabel* titleLabel();
    void SetTitle(const std::string& title);
    void SetImage(const std::string& name);
    void SetImage(const boost::intrusive_ptr<Texture>& texture);
    void SetHighlightedImage(const boost::intrusive_ptr<Texture>& texture);
    void SetFrameTextureForState(
        GUIControl::State state,
        const boost::intrusive_ptr<Texture>& texture);
    void SetTextColor(Color color);
    void SetBackgroundColor(Color color);
    void SetTintColor(Color color);
    void CancelPress();

    int state;          // +280 (see header note)
    bool field284;      // +284 UNRECOVERED name (DrawRect early-out)
    char _pad285[3];    // +285
    boost::scoped_ptr<GUIRoundedRect> frameRect;             // +288
    boost::scoped_ptr<GUIRoundedRect> highlightedFrameRect;  // +296
    boost::scoped_ptr<GUIRoundedRect> backgroundRect;        // +304
    GUILabel* titleLabelPtr;                                 // +312
    boost::scoped_ptr<GUITexturedRect> imageRect;            // +320
    boost::scoped_ptr<GUITexturedRect> highlightedImageRect; // +328
    Matrix4 field336;                                        // +336 UNRECOVERED name
    GUIButtonType buttonType;                                // +400
    float field404;                                          // +404 UNRECOVERED
    float labelInsets[4];  // +408 left, +412 right, +416 top, +420 bottom
    Vector2 imageOffset;   // +424
    Color backgroundColor; // +432
    Color tintColor;       // +436
    std::string soundName; // +440 UNRECOVERED name
    unsigned long long touchIdentifier;  // +464
};

} // namespace Caver

#endif
