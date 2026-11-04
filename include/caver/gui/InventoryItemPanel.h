// Recovered from libswordigo.so (1.4.13) evidence — IDA/Hex-Rays exports
// in reference/ida-arm64/functions/Caver/InventoryTrinketSlot/.
//
// InventoryTrinketSlot: the single equipped-trinket cell of the inventory
// screen. It owns a MenuItemSlot (which holds the trinket Item), draws the
// "no trinket" icon when both the slot's item and skill are empty, and
// turns a drag past a 5-point threshold into a DraggableMenuItem handed to
// the drag-and-drop engine.
//
// Layout (sizeof 0x170, from operator new in the caller and from the
// constructor @0x4B3C1C):
//   +0    GUIView (256 bytes)
//   +256  IMenuItemSlotDelegate vptr (secondary base; the golden stores
//         off_630D78 here and emits non-virtual thunks for the two
//         delegate methods)
//   +264  IInventoryTrinketSlotDelegate* m_delegate
//         (InventoryItemPanel::Init writes `panel + 256` here, i.e. the
//         panel's own secondary vptr)
//   +272  boost::shared_ptr<GameState> m_gameState
//   +288  boost::shared_ptr<MenuItemSlot> m_slot
//   +304  boost::shared_ptr<GUILabel> m_levelLabel — the "Level N" text
//         (proven by SetTrinket @0x4B41BC, which calls GUILabel::setText on
//         it and writes GUIView::hidden at +244, and by Init @0x4B3C78,
//         which allocates a 0x170-byte GUILabel there)
//   +320  boost::shared_ptr<GUITexturedRect> m_emptySlotIcon
//   +336  float m_scale           (constructor stores 1.0f)
//   +344  unsigned long long m_activeTouch
//   +352  Vector2 m_touchStartLocation
//   +360  bool m_selected         (constructor stores false)
// The destructor @0x4B3C78 releases the +328/+312/+296/+280 counted
// pointers, i.e. the four shared_ptrs above in reverse order, then
// ~GUIView; the implicit destructor reproduces that.
#ifndef CAVER_INVENTORYITEMPANEL_H
#define CAVER_INVENTORYITEMPANEL_H

#include <boost/shared_ptr.hpp>
#include <boost/intrusive_ptr.hpp>

#include "GUIView.h"
#include "MenuItemSlot.h"

namespace Caver {

class GameState;
class GUITexturedRect;
class GUILabel;
class GUIEvent;
class Item;
class Skill;
class Texture;

class InventoryItemPanel;
class InventoryTrinketSlot;

// The panel's owner (InventoryView). Golden secondary vtable @0x630E20
// slots 39/40: InventoryItemPanelTrinketChanged, InventoryItemPanelTextForTrinketBonus.
class IInventoryItemPanelDelegate {
public:
    virtual void InventoryItemPanelTrinketChanged(InventoryItemPanel* panel) = 0;
    virtual std::string InventoryItemPanelTextForTrinketBonus(InventoryItemPanel* panel) = 0;
};

// Secondary interface of InventoryTrinketSlot: the single virtual is slot
// 0 of the vptr the golden writes at InventoryTrinketSlot + 264.
class IInventoryTrinketSlotDelegate {
public:
    virtual void InventoryTrinketSlotDidBeginDragging(
        InventoryTrinketSlot* slot) = 0;
};

class InventoryItemPanel : public GUIView,
                          public IInventoryTrinketSlotDelegate {
public:
    InventoryItemPanel();
    virtual ~InventoryItemPanel();

    void Init(const boost::shared_ptr<GameState>& gameState);
    void SetItem(const boost::shared_ptr<Item>& item);
    void SetSkill(const boost::shared_ptr<Skill>& skill);
    void SetTrinket(const boost::shared_ptr<Item>& trinket);

    // _ZN5Caver18InventoryItemPanel12UpdateLabelsEv @0x4B0424, 5,552
    // bytes: rebuilds the detail label (+352) and the subtitle (+368)
    // from the item slot's item/skill and the trinket slot's trinket.
    void UpdateLabels();

    // Drag-and-drop slots 20/21/22 (0x4B1F1C / 0x4B20FC / 0x4B21DC).
    // IDA renders all three as void because the golden never writes the
    // return register; the bool return is forced by GUIView's slots.
    bool DraggableItemDidEnterView(GUIDraggableItem* item) override;
    bool DraggableItemDidLeaveView(GUIDraggableItem* item) override;
    bool DraggableItemDidDropIntoView(GUIDraggableItem* item) override;

    void Update(float deltaTime) override;
    void DrawRect(RenderingContext* context, const Rectangle& rect,
                  const Matrix4& transform) override;   // slot 10
    void LayoutSubviews() override;                     // empty in golden
    boost::shared_ptr<GUIView> HitTest(
        const Vector2& point) override;                 // slot 29

    // Told by the trinket slot (its +264 delegate pointer) when a drag
    // starts; the golden body is a 4-byte thunk.
    void InventoryTrinketSlotDidBeginDragging(
        InventoryTrinketSlot* slot) override;

    // _ZN5Caver18InventoryItemPanel24AddToggleLabelsAnimationEv @0x4B287C,
    // 822 bytes: alpha 1 -> 0 over 0.25 s with a 2 s delay on the label
    // currently on screen (+336), completed by
    // ToggleLabelsFadeOutAnimationFinished.
    void AddToggleLabelsAnimation();

    // _ZN5Caver18InventoryItemPanel36ToggleLabelsFadeOutAnimationFinishedE
    // @0x4B2EAC, 1,208 bytes: the fade-out has run; hide the label that
    // was on screen, swap +336 to the other of the detail (+352) and
    // subtitle (+368) labels, show it, then alpha 0 -> 1 over 0.25 s.
    void ToggleLabelsFadeOutAnimationFinished(void* target, GUIEvent* event);

    // _ZN5Caver18InventoryItemPanel35ToggleLabelsFadeInAnimationFinishedE
    // @0x4B35D4, 4 bytes: a thunk to AddToggleLabelsAnimation, so the
    // toggle cycles for as long as the panel lives.
    void ToggleLabelsFadeInAnimationFinished(void* target, GUIEvent* event);

    // Layout from the constructor @0x4AF97C (a 0x100-byte zero run from
    // +264) and from Init @0x4AF9D0, which addresses every member
    // through a1[34]..a1[47]:
    //   +256 IInventoryTrinketSlotDelegate vptr (secondary base)
    //   +264 one 8-byte member the constructor zeroes; its type and name
    //        are UNRECOVERED, so it is kept as a bare pointer
    //   +272 boost::shared_ptr<GameState> m_gameState
    //   +288 boost::shared_ptr<GUITexturedRect> m_panelView
    //   +304 boost::shared_ptr<MenuItemSlot> m_itemSlot
    //   +320 boost::shared_ptr<InventoryTrinketSlot> m_trinketSlot
    //   +336 boost::shared_ptr<GUILabel> m_currentLabel — NOT a label of
    //        its own: Init ends with `+336 = +352` (the detail label),
    //        and the toggle animation alternates it between +352 and
    //        +368. It is the label currently on screen.
    //   +352 boost::shared_ptr<GUILabel> m_detailLabel
    //   +368 boost::shared_ptr<GUILabel> m_subtitleLabel
    IInventoryItemPanelDelegate* m_delegate;         // +264 (owner callback)
    boost::shared_ptr<GameState> m_gameState;            // +272
    boost::shared_ptr<GUITexturedRect> m_panelView;      // +288
    boost::shared_ptr<MenuItemSlot> m_itemSlot;          // +304
    boost::shared_ptr<InventoryTrinketSlot> m_trinketSlot; // +320
    boost::shared_ptr<GUILabel> m_currentLabel;          // +336
    boost::shared_ptr<GUILabel> m_detailLabel;           // +352
    boost::shared_ptr<GUILabel> m_subtitleLabel;         // +368
};

// Golden aarch64 NDK-libc++ layout anchor; MSVC STL member sizes differ,
// so this assert is Android/Linux-only (the Windows build is self-consistent
// under MSVC STL and does not claim golden byte-layout parity).
#ifndef _MSC_VER
static_assert(sizeof(InventoryItemPanel) == 384,
              "InventoryItemPanel sizeof must be 384 bytes");
#endif

class InventoryTrinketSlot : public GUIView, public IMenuItemSlotDelegate {
public:
    InventoryTrinketSlot();
    ~InventoryTrinketSlot() override;

    void Init(const boost::shared_ptr<GameState>& gameState, float scale);
    void SetTrinket(const boost::shared_ptr<Item>& trinket);
    void SetEmptySlotIconTexture(const boost::intrusive_ptr<Texture>& texture);
    void UpdateHighlightedState();

    void Update(float deltaTime) override;                   // thunk to GUIView
    void DrawRect(RenderingContext* context, const Rectangle& rect,
                  const Matrix4& transform) override;
    void LayoutSubviews() override;                          // empty in golden
    void TouchBegan(const FWTouch& touch) override;
    void TouchMoved(const FWTouch& touch) override;
    void TouchEnded(const FWTouch& touch) override;
    void TouchCancelled(const FWTouch& touch) override;

    void MenuItemSlotSelected(MenuItemSlot* slot) override;   // empty
    void MenuItemSlotDeselected(MenuItemSlot* slot) override; // empty

    IInventoryTrinketSlotDelegate* m_delegate;            // +264
    boost::shared_ptr<GameState> m_gameState;             // +272
    boost::shared_ptr<MenuItemSlot> m_slot;               // +288
    boost::shared_ptr<GUILabel> m_levelLabel;             // +304
    boost::shared_ptr<GUITexturedRect> m_emptySlotIcon;   // +320
    float m_scale;                                        // +336
    unsigned long long m_activeTouch;                     // +344
    Vector2 m_touchStartLocation;                         // +352
    bool m_selected;                                      // +360
};

// Golden aarch64 NDK-libc++ layout anchor; MSVC STL member sizes differ,
// so this assert is Android/Linux-only (the Windows build is self-consistent
// under MSVC STL and does not claim golden byte-layout parity).
#ifndef _MSC_VER
static_assert(sizeof(InventoryTrinketSlot) == 0x170,
              "InventoryTrinketSlot sizeof must be 368 bytes");
#endif

} // namespace Caver

#endif // CAVER_INVENTORYITEMPANEL_H
