// Recovered from libswordigo.so (1.4.13) evidence — IDA/Hex-Rays
// exports in ~/workspace/analysis/swordigo/functions/Caver/
// ItemInfoView/, the golden vtable _ZTVN5Caver12ItemInfoViewE
// @0x6313C0 (320 bytes, ABS64 relocations) and the RTTI
// _ZTIN5Caver12ItemInfoViewE @0x631500 (__si_class_type_info whose
// single base is Caver::GUIView).
//
// ItemInfoView: the modal info popup for an item or skill — a close
// button, a dark frame holding a MenuItemSlot plus a title label, a
// second dark frame holding the description label, and (added by
// EnablePurchasing) a price frame and a "Buy" button wired to a
// program object. NOT a MenuViewPage and NOT ItemInfoPopupView
// (a different 336-byte class held by GameSceneView): the vtable is
// exactly the 38 GUIView slots, overriding only the destructor
// (slots 7/8) and LayoutSubviews (slot 11). Dismiss, Init,
// ButtonPressed, EnablePurchasing, AddAnimations, SetItem and
// SetSkill are all non-virtual.
//
// Layout (sizeof 400 = 0x190, byte-verified against the ctor
// @0x4BAAFC zero-store map — eight shared_ptr slots zeroed at
// +256..+383, the bool at +384, the intrusive pointer at +392 —
// the dtor @0x4BDAC4 teardown order, and every method body's
// accesses):
//   +0..255  GUIView base
//   +256     boost::shared_ptr<GUIButton> closeButton (Init: type-0
//            button with the "ui_close_button" frame texture; its
//            ButtonPressed branch runs program components whose
//            +108 field == 6)
//   +272     boost::shared_ptr<MenuItemSlot> itemSlot (Init:
//            MenuItemSlot::Init(1.0f), field304 cleared; SetItem /
//            SetSkill forward to it)
//   +288     boost::shared_ptr<GUIFrameView> frameView ("ui_frame_dark"
//            frame, 300x32; super-view of itemSlot and titleLabel)
//   +304     boost::shared_ptr<GUIFrameView> descriptionFrameView
//            ("ui_frame_dark" frame, 300x70; super-view of
//            descriptionLabel)
//   +320     boost::shared_ptr<GUILabel> titleLabel (DefaultFont,
//            white, shadowed; SetItem shows the item title — with a
//            "Got Item: " prefix while field384 is set — SetSkill
//            shows the skill title)
//   +336     boost::shared_ptr<GUILabel> descriptionLabel
//            (DefaultFont, colour (251,176,59,255), shadowed,
//            field344 set; item/skill description text)
//   +352     boost::shared_ptr<GUIFrameView> purchaseFrameView
//            (created lazily by EnablePurchasing: "ui_frame_dark",
//            140x32; holds the price label and the coin frame)
//   +368     boost::shared_ptr<GUIButton> buyButton (created lazily
//            by EnablePurchasing when its bool argument is set:
//            type-1 button titled "Buy"; its ButtonPressed branch
//            runs program components whose +108 field == 5)
//   +384     bool field384 — while set, SetItem prefixes the title
//            with "Got Item: " (name UNRECOVERED; only the ctor's
//            zero store and SetItem's read touch it in this class)
//   +392     boost::intrusive_ptr<SceneObject> object — the program
//            object assigned by EnablePurchasing; ButtonPressed
//            queries its ProgramComponents
// Member NAMES are behaviour-derived and UNRECOVERED.
#ifndef CAVER_ITEMINFOVIEW_H
#define CAVER_ITEMINFOVIEW_H

#include <boost/intrusive_ptr.hpp>
#include <boost/shared_ptr.hpp>

#include "GUIView.h"

namespace Caver {

class GUIButton;
class GUIEvent;
class GUIFrameView;
class GUILabel;
class Item;
class MenuItemSlot;
class SceneObject;
class Skill;

class ItemInfoView : public GUIView {
public:
    ItemInfoView();
    ~ItemInfoView() override;  // slots 7, 8

    void LayoutSubviews() override;  // slot 11

    void Init();
    void ButtonPressed(void* sender, GUIEvent* event);
    void EnablePurchasing(int price,
                          const boost::intrusive_ptr<SceneObject>& object,
                          bool showBuyButton);
    void AddAnimations(bool dismiss);
    void SetItem(const boost::shared_ptr<Item>& item);
    void SetSkill(const boost::shared_ptr<Skill>& skill);
    void Dismiss();

    boost::shared_ptr<GUIButton> closeButton;             // +256
    boost::shared_ptr<MenuItemSlot> itemSlot;             // +272
    boost::shared_ptr<GUIFrameView> frameView;            // +288
    boost::shared_ptr<GUIFrameView> descriptionFrameView; // +304
    boost::shared_ptr<GUILabel> titleLabel;               // +320
    boost::shared_ptr<GUILabel> descriptionLabel;        // +336
    boost::shared_ptr<GUIFrameView> purchaseFrameView;    // +352
    boost::shared_ptr<GUIButton> buyButton;               // +368
    bool field384;                                        // +384 (name UNRECOVERED)
    char _pad385[7];                                      // +385
    boost::intrusive_ptr<SceneObject> object;             // +392
};

// Golden aarch64 NDK-libc++ layout anchor; MSVC STL member sizes differ,
// so this assert is Android/Linux-only (the Windows build is self-consistent
// under MSVC STL and does not claim golden byte-layout parity).
#ifndef _MSC_VER
static_assert(sizeof(ItemInfoView) == 400,
              "ItemInfoView layout: ctor/dtor store maps, golden");
#endif

} // namespace Caver

#endif
