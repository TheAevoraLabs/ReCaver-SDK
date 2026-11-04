// Recovered from libswordigo.so (1.4.13) evidence — IDA/Hex-Rays
// exports in ~/workspace/analysis/swordigo/functions/Caver/
// ItemBuyPopupView/, the golden vtable _ZTVN5Caver16ItemBuyPopupViewE
// @0x6310B0 (320 bytes, ABS64 relocations) and the RTTI
// _ZTIN5Caver16ItemBuyPopupViewE @0x6311F0 (__si_class_type_info whose
// single base is Caver::GUIView).
//
// ItemBuyPopupView: the "Buy <item> for <price> soul shards?"
// confirmation popup — a notification-background sprite, one
// FontText line, two menu buttons ("Buy" / "Don't buy") wired to
// ButtonPressed, and the item slot + item icon sprites centred in
// the slot. The vtable is exactly the 38 GUIView slots, overriding
// only the destructor (slots 7/8), Update (slot 9, a tail branch to
// GUIView::Update), DrawRect (slot 10) and LayoutSubviews (slot 11,
// an empty body). Init, ButtonPressed and Dismiss are non-virtual.
//
// Layout (sizeof 360 = 0x168, byte-verified against the ctor
// @0x4B82A0 zero-store map — six shared_ptr slots zeroed at
// +256..+351 and the intrusive pointer at +352 — the dtor @0x4B97FC
// teardown order, and every method body's accesses):
//   +0..255  GUIView base
//   +256     boost::shared_ptr<Sprite> backgroundSprite (Init: the
//            "ui_notification_background" texture, one rectangle
//            covering the view bounds)
//   +272     boost::shared_ptr<FontText> text (Init: default font,
//            white; "Buy " + item title + " for " + price +
//            " soul shards?" laid out next to the item slot)
//   +288     boost::shared_ptr<GUIButton> buyButton (Init: type-0
//            "ui_menu_button" button titled "Buy"; its
//            ButtonPressed branch runs program components whose
//            +108 field == 5)
//   +304     boost::shared_ptr<GUIButton> dontBuyButton (same
//            construction, titled "Don't buy"; its ButtonPressed
//            branch runs program components whose +108 field == 6)
//   +320     boost::shared_ptr<Sprite> itemSlotSprite (Init: the
//            "ui_menu_itemslot" texture at (buyButton frame x - 2,
//            55))
//   +336     boost::shared_ptr<Sprite> itemIconSprite (Init: the
//            "item_" + item-name texture, centred on the slot)
//   +352     boost::intrusive_ptr<SceneObject> object — the program
//            object ButtonPressed queries for ProgramComponents;
//            never written by Init (assigned externally; name
//            UNRECOVERED)
// Member NAMES are behaviour-derived and UNRECOVERED.
#ifndef CAVER_ITEMBUYPOPUPVIEW_H
#define CAVER_ITEMBUYPOPUPVIEW_H

#include <boost/intrusive_ptr.hpp>
#include <boost/shared_ptr.hpp>

#include "GUIView.h"

namespace Caver {

class FontText;
class GUIButton;
class GUIEvent;
class Item;
class SceneObject;
class Sprite;

class ItemBuyPopupView : public GUIView {
public:
    ItemBuyPopupView();
    ~ItemBuyPopupView() override;  // slots 7, 8

    void Update(float deltaTime) override;  // slot 9
    void DrawRect(RenderingContext* context, const Rectangle& rect,
                  const Matrix4& transform) override;  // slot 10
    void LayoutSubviews() override;                    // slot 11

    void Init(const boost::shared_ptr<Item>& item, int price);
    void ButtonPressed(void* sender, GUIEvent* event);
    void Dismiss();

    boost::shared_ptr<Sprite> backgroundSprite;    // +256
    boost::shared_ptr<FontText> text;              // +272
    boost::shared_ptr<GUIButton> buyButton;        // +288
    boost::shared_ptr<GUIButton> dontBuyButton;    // +304
    boost::shared_ptr<Sprite> itemSlotSprite;      // +320
    boost::shared_ptr<Sprite> itemIconSprite;      // +336
    boost::intrusive_ptr<SceneObject> object;      // +352 (name UNRECOVERED)
};

// Golden aarch64 NDK-libc++ layout anchor; MSVC STL member sizes differ,
// so this assert is Android/Linux-only (the Windows build is self-consistent
// under MSVC STL and does not claim golden byte-layout parity).
#ifndef _MSC_VER
static_assert(sizeof(ItemBuyPopupView) == 360,
              "ItemBuyPopupView layout: ctor/dtor store maps, golden");
#endif

} // namespace Caver

#endif
