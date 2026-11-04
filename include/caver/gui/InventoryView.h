// Recovered from libswordigo.so (1.4.13) evidence — IDA/Hex-Rays
// exports in ~/workspace/analysis/swordigo/functions/Caver/InventoryView/,
// the golden vtable _ZTVN5Caver13InventoryViewE @0x630E20 (456 bytes,
// ABS64 relocations) and the RTTI _ZTIN5Caver13InventoryViewE @0x630FF8
// (__vmi_class_type_info, 4 bases).
//
// InventoryView: the INVENTORY page of the tabbed menu — three
// InventoryItemPanels (weapon / armor / skill), a striped frame view
// holding the "Trinkets" label and a row of three InventoryTrinketSlots
// (fire / ice / shadow), plus a help button that pops an info bubble.
//
// Derivation (binary-proven): the RTTI lists the bases in order
//   MenuViewPage @0, IInventoryItemPanelDelegate @280,
//   IInventoryTrinketSlotDelegate @288, IMenuItemSlotDelegate @296.
// MenuViewPage is GUIView + the std::string page title at +256, with
// Dismiss PURE virtual at slot 38 (the golden MenuViewPage vtable
// slot holds __cxa_pure_virtual) and no out-of-line ctor (its D2
// @0x4AA3B8 is the string destroy + ~GUIView tail). Because the title
// string lives inside the primary base, the three delegate subobjects
// land at +280/+288/+296 — modelling the base is REQUIRED for the
// layout (a GUIView-direct model, as in QuestsView.h, would put the
// interfaces at +256 and shift the string). HAND-OFF: no
// MenuViewPage.h exists in the tree yet, so the class is defined
// here; when the game-views coordinator lands the real header this
// definition moves there (identical shape). Logged in SHARED.md.
//
// Primary vtable: the 38 GUIView slots (TouchBegan slot 0, Update
// slot 9, DrawRect slot 10 and LayoutSubviews slot 11 overridden),
// Dismiss at slot 38 (MenuViewPage's slot), then the five delegate
// methods appended at slots 39-43 in declaration order. The secondary
// vtables hold the _ZThn280_ / _ZThn288_ / _ZThn296_ thunks.
//
// Layout (sizeof 416, byte-verified against the ctor @0x4B4F28
// zero-store map, the dtor @0x4B7CDC teardown order — exact reverse
// declaration order — and every method body's accesses):
//   +0..255  GUIView base
//   +256     std::string page title (MenuViewPage member; the ctor
//            assigns "INVENTORY")
//   +280     IInventoryItemPanelDelegate subobject (vptr only)
//   +288     IInventoryTrinketSlotDelegate subobject (vptr only)
//   +296     IMenuItemSlotDelegate subobject (vptr only)
//   +304     IInventoryViewDelegate* delegate (raw, non-owning; the
//            InventoryItemPanelTrinketChanged body calls its slot 1,
//            InventoryViewTrinketDidChange, through this pointer;
//            GameMenuViewController implements that interface)
//   +312     boost::shared_ptr<GameState> gameState
//   +328     int item panel count (InitWithGameState stores 3;
//            member name UNRECOVERED)
//   +336     boost::scoped_array<boost::shared_ptr<InventoryItemPanel> >
//            item panels (3 elements, operator new[](0x38))
//   +344     boost::shared_ptr<GUIFrameView> stripe frame view
//            (the "ui_menu_stripe" frame; member name UNRECOVERED)
//   +360     boost::shared_ptr<GUILabel> "Trinkets" label
//            (member name UNRECOVERED)
//   +376     int trinket slot count (InitWithGameState stores 3;
//            member name UNRECOVERED)
//   +384     boost::scoped_array<boost::shared_ptr<InventoryTrinketSlot> >
//            trinket slots (3 elements, operator new[](0x38))
//   +392     boost::shared_ptr<GUIButton> info (help) button
//            (member name UNRECOVERED)
//   +408     bool: when set, Update() clears it and calls
//            ButtonPressed(infoButton) — a deferred button press
//            (member name UNRECOVERED; the setter is outside this
//            class's function list)
#ifndef CAVER_INVENTORYVIEW_H
#define CAVER_INVENTORYVIEW_H

#include <boost/scoped_array.hpp>
#include <boost/shared_ptr.hpp>
#include <string>

#include "GUIView.h"
#include "MenuViewPage.h"
#include "InventoryItemPanel.h"

namespace Caver {

class GameState;
class GUIButton;
class GUIEvent;
class GUIFrameView;
class GUILabel;
class MenuItemSlot;
class RenderingContext;
class InventoryView;

enum InventoryViewItemType {
    InventoryViewItemType0 = 0,
    InventoryViewItemType1 = 1,
    InventoryViewItemType2 = 2,
};

class IInventoryViewDelegate {
public:
    virtual void InventoryViewSelectedItemDidChange(InventoryView* view, InventoryViewItemType type) = 0;
    virtual void InventoryViewTrinketDidChange(InventoryView* view, InventoryViewItemType type) = 0;
};

class InventoryView : public MenuViewPage,
                      public IInventoryItemPanelDelegate,
                      public IInventoryTrinketSlotDelegate,
                      public IMenuItemSlotDelegate {
public:
    InventoryView();
    ~InventoryView() override;  // slots 7, 8

    void InitWithGameState(const boost::shared_ptr<GameState>& gameState);

    void ButtonPressed(void* sender, GUIEvent* event);
    void UpdateTrinketHighlights();
    void AnimateIn();

    void TouchBegan(const FWTouch& touch) override;  // slot 0
    void Update(float deltaTime) override;           // slot 9
    void DrawRect(RenderingContext* context, const Rectangle& rect,
                  const Matrix4& transform) override;  // slot 10
    void LayoutSubviews() override;                    // slot 11
    void Dismiss() override;                           // slot 38

    // Delegate-interface implementations (primary slots 39-43, in
    // golden vtable order).
    void InventoryItemPanelTrinketChanged(InventoryItemPanel* panel) override;              // slot 39
    std::string InventoryItemPanelTextForTrinketBonus(InventoryItemPanel* panel) override;  // slot 40
    void InventoryTrinketSlotDidBeginDragging(InventoryTrinketSlot* slot) override;         // slot 41
    void MenuItemSlotSelected(MenuItemSlot* slot) override;                                 // slot 42
    void MenuItemSlotDeselected(MenuItemSlot* slot) override;                               // slot 43

    // (pageTitle string is the MenuViewPage member at +256; the
    // three delegate subobjects occupy +280/+288/+296.)
    IInventoryViewDelegate* delegate;      // +304
    boost::shared_ptr<GameState> gameState;  // +312
    int itemPanelCount;                    // +328 (name UNRECOVERED)
    boost::scoped_array<boost::shared_ptr<InventoryItemPanel> > itemPanels;    // +336
    boost::shared_ptr<GUIFrameView> stripeFrameView;                         // +344 (name UNRECOVERED)
    boost::shared_ptr<GUILabel> trinketsLabel;                               // +360 (name UNRECOVERED)
    int trinketSlotCount;                  // +376 (name UNRECOVERED)
    boost::scoped_array<boost::shared_ptr<InventoryTrinketSlot> > trinketSlots;  // +384
    boost::shared_ptr<GUIButton> infoButton;                                 // +392 (name UNRECOVERED)
    bool deferredButtonPress;              // +408 (name UNRECOVERED)
};

// Golden aarch64 NDK-libc++ layout anchor; MSVC STL member sizes differ,
// so this assert is Android/Linux-only (the Windows build is self-consistent
// under MSVC STL and does not claim golden byte-layout parity).
#ifndef _MSC_VER
static_assert(sizeof(InventoryView) == 416,
              "InventoryView layout: ctor/dtor store maps, golden");
#endif

} // namespace Caver

#endif
