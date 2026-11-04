// Recovered from libswordigo.so (1.4.13) evidence — IDA/Hex-Rays
// exports in ~/workspace/analysis/swordigo/functions/Caver/
// AttributePointsView/, the golden vtable _ZTVN5Caver19AttributePointsViewE
// @0x62FF18 (320 bytes = 16-byte header + exactly 38 slots) and the
// RTTI _ZTIN5Caver19AttributePointsViewE @0x630058
// (__si_class_type_info, single base Caver::GUIView).
//
// AttributePointsView: one attribute row of the character menu (the
// CharacterView creates three, Init()ed with the names "health" /
// "attack" / "energy" and the titles "Health" / "Attack" / "Energy"):
// a dark frame holding a gradient title label, a row of twelve point
// sprites, "+" / "-" icon buttons, a "MAX" label and a help button
// that presents the help text as an info bubble. It is NOT a
// MenuViewPage (the RTTI proves the GUIView-only derivation and the
// vtable has no slot 38): the primary vtable is the 38 GUIView slots,
// overriding only the destructors (7/8), Update (9), DrawRect (10)
// and LayoutSubviews (11).
//
// The delegate interface is RTTI-proven: _ZTIN5Caver28IAttributePoints
// ViewDelegateE @0x630200 is a plain __class_type_info; CharacterView
// implements it (its subobject sits at CharacterView+280 — the golden
// _ZThn280_ thunk to CharacterView::AttributePointsViewValueChanged)
// and the view stores a raw pointer to it at +256 (CharacterView::
// InitWithGameState writes it directly: view+0x100 = characterView
// + 0x118). Its single slot is AttributePointsViewValueChanged,
// called at the end of ButtonPressed for the plus/minus buttons.
//
// Layout (sizeof 1480 = 0x5C8, byte-verified against the operator
// new(0x5C8) in CharacterView::InitWithGameState @0x4A5D44, the ctor
// @0x4A31E0 store map, the dtor @0x4A5620 teardown order — exact
// reverse declaration order — and every method body's accesses):
//   +0..255   GUIView base
//   +256      IAttributePointsViewDelegate* delegate (raw)
//   +264      bool editingEnabled (ctor 0; SetEditingEnabled's guard)
//   +268      int initialPoints (SetInitialPoints stores it and
//             copies it to numPoints; the minus button hides while
//             numPoints <= initialPoints)
//   +272      int maxPoints (Init stores 12)
//   +276      int numPoints (ctor -1)
//   +280      boost::shared_ptr<GUIFrameView> frameView (the
//             "ui_frame_dark" frame; every widget below is added to
//             it, not to this view)
//   +296      boost::shared_ptr<GUILabel> titleLabel (font =
//             DefaultFont, gradient = TextGradient(0.15, 1, 0.95, 1),
//             black shadow at offset (1, -1); Init sets its text)
//   +312      boost::shared_ptr<GUIButton> plusButton ("ui_button_plus"
//             icon button; ButtonPressed increments numPoints)
//   +328      boost::shared_ptr<GUIButton> minusButton
//             ("ui_button_minus"; decrements)
//   +344      boost::shared_ptr<GUILabel> maxLabel (NormalLabel
//             "MAX", recoloured to (255, 230, 160, 255); hidden while
//             initialPoints < 8)
//   +360      boost::shared_ptr<GUIButton> helpButton
//             ("ui_button_help"; its GUIButton +336 matrix is scaled
//             0.6; hidden while the help text is empty)
//   +376      std::string helpText (SetHelpText)
//   +400      boost::intrusive_ptr<Texture> pointTexture
//             ("ui_attrib_" + name, from Init)
//   +408      boost::intrusive_ptr<Texture> glowTexture (the same
//             name + "_glow"; sprites below the displayed count use
//             it, the rest use pointTexture)
//   +416      int displayedNumPoints (mirrors numPoints at every
//             mutation; the sprite loops iterate it — name
//             UNRECOVERED)
//   +424      GUIAnimatedSprite pointSprites[12] (80 bytes each,
//             ending exactly at the sprite below)
//   +1384     Sprite sprite (88 bytes; lazily Init()ed by
//             UpdateSprite from pointTexture's parent texture)
//   +1472     bool spriteDirty (set by every point change / sprite
//             animation activity / layout; DrawRect rebuilds the
//             sprite and clears it — name UNRECOVERED)
// Member NAMES are behaviour-derived and UNRECOVERED unless noted.
#ifndef CAVER_ATTRIBUTEPOINTSVIEW_H
#define CAVER_ATTRIBUTEPOINTSVIEW_H

#include <string>

#include <boost/intrusive_ptr.hpp>
#include <boost/shared_ptr.hpp>

#include "../render/Sprite.h"
#include "GUIAnimatedSprite.h"
#include "GUIView.h"

namespace Caver {

class GUIButton;
class GUIEvent;
class GUIFrameView;
class GUILabel;
class AttributePointsView;

// The delegate interface (RTTI-proven name, see the header note).
#ifndef CAVER_IATTRIBUTEPOINTSVIEWDELEGATE_DEFINED
#define CAVER_IATTRIBUTEPOINTSVIEWDELEGATE_DEFINED
class IAttributePointsViewDelegate {
public:
    virtual void AttributePointsViewValueChanged(
        AttributePointsView* view) = 0;  // slot 0
};
#endif

class AttributePointsView : public GUIView {
public:
    AttributePointsView();
    ~AttributePointsView() override;  // slots 7, 8

    void Init(const std::string& name, const std::string& title);
    void ButtonPressed(void* sender, GUIEvent* event);
    void SetEditingEnabled(bool enabled);
    void UpdatePointState();
    void SetInitialPoints(int points);
    void SetNumPoints(int points);
    void SetMaxPoints(int points);
    void SetHelpText(const std::string& text);
    void UpdateSprite();

    void Update(float deltaTime) override;  // slot 9
    void DrawRect(RenderingContext* context, const Rectangle& rect,
                  const Matrix4& transform) override;  // slot 10
    void LayoutSubviews() override;                    // slot 11

    IAttributePointsViewDelegate* delegate;        // +256
    bool editingEnabled;                           // +264
    char _pad265[3];                               // +265
    int initialPoints;                             // +268
    int maxPoints;                                 // +272
    int numPoints;                                 // +276
    boost::shared_ptr<GUIFrameView> frameView;     // +280
    boost::shared_ptr<GUILabel> titleLabel;        // +296
    boost::shared_ptr<GUIButton> plusButton;       // +312
    boost::shared_ptr<GUIButton> minusButton;      // +328
    boost::shared_ptr<GUILabel> maxLabel;          // +344
    boost::shared_ptr<GUIButton> helpButton;       // +360
    std::string helpText;                          // +376
    boost::intrusive_ptr<Texture> pointTexture;    // +400
    boost::intrusive_ptr<Texture> glowTexture;     // +408
    int displayedNumPoints;                        // +416 (name UNRECOVERED)
    char _pad420[4];                               // +420
    GUIAnimatedSprite pointSprites[12];            // +424
    Sprite sprite;                                 // +1384
    bool spriteDirty;                              // +1472 (name UNRECOVERED)
    char _pad1473[7];                              // +1473
};

// Golden aarch64 NDK-libc++ layout anchor; MSVC STL member sizes differ,
// so this assert is Android/Linux-only (the Windows build is self-consistent
// under MSVC STL and does not claim golden byte-layout parity).
#ifndef _MSC_VER
static_assert(sizeof(AttributePointsView) == 1480,
              "AttributePointsView layout: operator new(0x5C8) in "
              "CharacterView::InitWithGameState, golden");
#endif

} // namespace Caver

#endif
