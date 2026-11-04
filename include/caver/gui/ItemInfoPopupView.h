// Recovered from libswordigo.so (1.4.13) evidence — IDA/Hex-Rays
// exports in ~/workspace/analysis/swordigo/functions/Caver/
// ItemInfoPopupView/, the golden vtable _ZTVN5Caver17ItemInfoPopupViewE
// @0x631268 (320 bytes, ABS64 relocations) and the RTTI
// _ZTIN5Caver17ItemInfoPopupViewE @0x6313A8 (__si_class_type_info whose
// single base is Caver::GUIView).
//
// ItemInfoPopupView: the small item/skill info popup GameSceneView
// shows over the game (GameSceneView::ShowItemInfoPopup allocates it
// with operator new(336) and calls InitWithItem). NOT ItemInfoView
// (a different 400-byte class, GUIView direct, with buttons/frames).
// The vtable is exactly the 38 GUIView slots, overriding only the
// destructor (slots 7/8), Update (slot 9), DrawRect (slot 10) and
// LayoutSubviews (slot 11, an empty body). Init, InitWithItem and
// InitWithSkill are non-virtual.
//
// Layout (sizeof 336 = 0x150, byte-verified against the ctor
// @0x4B9BA8 zero-store map — five shared_ptr slots zeroed at
// +256..+335 — the dtor @0x4BA904 teardown order +328/+312/+296/
// +280/+264 (reverse declaration order), the Init @0x4B9BEC reset
// sites, and the DrawRect @0x4BA81C draw order):
//   +0..255  GUIView base
//   +256     boost::shared_ptr<Sprite> backgroundSprite (Init: the
//            "ui_notification_background" texture, one rectangle
//            covering the view bounds)
//   +272     boost::shared_ptr<Sprite> itemSlotSprite (Init: the
//            "ui_menu_itemslot" texture, drawn at 0.7 scale centred
//            on bounds.height * 0.5 on both axes)
//   +288     boost::shared_ptr<Sprite> itemIconSprite (Init: the
//            item/skill texture passed to Init, same placement as
//            the slot sprite)
//   +304     boost::shared_ptr<FontText> titleText (Init: default
//            font, white, title string laid out at (58, 28))
//   +320     boost::shared_ptr<FontText> descriptionText (Init:
//            default font, colour dword 0x00507878, description
//            string laid out at (58, 12))
// Member NAMES are behaviour-derived and UNRECOVERED.
#ifndef CAVER_ITEMINFOPOPUPVIEW_H
#define CAVER_ITEMINFOPOPUPVIEW_H

#include <boost/intrusive_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <string>

#include "GUIView.h"

namespace Caver {

class FontText;
class Item;
class RenderingContext;
class Skill;
class Sprite;
class Texture;

class ItemInfoPopupView : public GUIView {
public:
    ItemInfoPopupView();
    ~ItemInfoPopupView() override;  // slots 7, 8

    void Update(float deltaTime) override;  // slot 9
    void DrawRect(RenderingContext* context, const Rectangle& rect,
                  const Matrix4& transform) override;  // slot 10
    void LayoutSubviews() override;                    // slot 11

    void Init(const boost::intrusive_ptr<Texture>& texture,
              const std::string& title, const std::string& description);
    void InitWithItem(const boost::shared_ptr<Item>& item);
    void InitWithSkill(const boost::shared_ptr<Skill>& skill);

    boost::shared_ptr<Sprite> backgroundSprite;    // +256
    boost::shared_ptr<Sprite> itemSlotSprite;      // +272
    boost::shared_ptr<Sprite> itemIconSprite;      // +288
    boost::shared_ptr<FontText> titleText;         // +304
    boost::shared_ptr<FontText> descriptionText;   // +320
};

// Golden aarch64 NDK-libc++ layout anchor; MSVC STL member sizes differ,
// so this assert is Android/Linux-only (the Windows build is self-consistent
// under MSVC STL and does not claim golden byte-layout parity).
#ifndef _MSC_VER
static_assert(sizeof(ItemInfoPopupView) == 336,
              "ItemInfoPopupView layout: ctor/dtor store maps, golden");
#endif

} // namespace Caver

#endif
