// Recovered from libswordigo.so (1.4.13) evidence — IDA/Hex-Rays
// exports in ~/workspace/analysis/swordigo/functions/Caver/
// SkillPickerView/, the golden vtables @0x632818 (primary) and
// @0x632968 (secondary, +256) and the RTTI @0x632978 (__vmi type info
// with bases Caver::GUIView and Caver::IMenuItemSlotDelegate
// @0x630D88).
//
// SkillPickerView: the "Choose a spell" picker — a title label plus
// four MenuItemSlots (skills "bolt", "bomb", "hookshot", "dimension"),
// each slot inside its own plain GUIView container. It is NOT a
// MenuViewPage (no page-title string, no Dismiss in the vtable): the
// primary vtable is the 38 GUIView slots plus slots 38/39 =
// MenuItemSlotSelected / MenuItemSlotDeselected, the two
// IMenuItemSlotDelegate methods (the interface subobject lives at
// +256 and MenuItemSlot stores a pointer to it as its delegate).
// Dismiss() and TouchBegan() share one body (animate out, disable
// interaction, notify the delegate's DidDismiss) and Dismiss is
// non-virtual. Overrides: TouchBegan (0), HandleKeyboardEvent (5),
// Update (9, thunk to GUIView), DrawRect (10, thunk to GUIView),
// LayoutSubviews (11), DidMoveToWindow (16),
// CanBecomeFirstResponder (24).
//
// Layout (sizeof 344, byte-verified against the ctor @0x4D47A4
// store map, the dtor @0x4D6B44 teardown order and every method
// body's accesses):
//   +0..255  GUIView base
//   +256     IMenuItemSlotDelegate subobject (vptr only)
//   +264     SkillPickerViewDelegate* delegate (RAW; slot 0 =
//            SkillPickerViewDidSelectSkill(view, skill), slot 1 =
//            SkillPickerViewDidDismiss(view) — the interface the
//            SkillPickerViewController implements; its real name is
//            UNRECOVERED, "SkillPickerViewDelegate" is descriptive)
//   +272     boost::shared_ptr<GameState> gameState
//   +288     boost::shared_ptr<GUILabel> titleLabel (never added as
//            a subview by InitWithGameState — golden behaviour)
//   +304     int slotCount (= 4)
//   +312     boost::scoped_array<boost::shared_ptr<MenuItemSlot> >
//            slots
//   +320     boost::scoped_array<boost::shared_ptr<GUILabel> >
//            slotLabels
//   +328     boost::scoped_array<boost::shared_ptr<GUIView> >
//            slotContainers
//   +336     int selectedIndex (ctor: -1)
// Member NAMES are behaviour-derived and UNRECOVERED.
#ifndef CAVER_SKILLPICKERVIEW_H
#define CAVER_SKILLPICKERVIEW_H

#include <string>

#include <boost/scoped_array.hpp>
#include <boost/shared_ptr.hpp>

#include "GUITexturedRect.h"
#include "GUIView.h"
#include "../core/Skill.h"

namespace Caver {

class GameState;
class GUILabel;
class Item;
class MenuItemSlot;
class SkillPickerView;

// Skill now lives in core/Skill.h (extracted from here so the systems and
// gui slices share one definition instead of three forward declarations).

// The slot-delegate interface (RTTI-proven name). No virtual
// destructor in the golden: the secondary vtable has exactly the
// two method slots.
class IMenuItemSlotDelegate {
public:
    virtual void MenuItemSlotSelected(MenuItemSlot* slot) = 0;
    virtual void MenuItemSlotDeselected(MenuItemSlot* slot) = 0;
};

// Implemented by SkillPickerViewController; see the layout note
// above for the (descriptive) naming.
class SkillPickerViewDelegate {
public:
    virtual void SkillPickerViewDidSelectSkill(
        SkillPickerView* view, const boost::shared_ptr<Skill>& skill);
    virtual void SkillPickerViewDidDismiss(SkillPickerView* view);
};

// MenuItemSlot: minimal declaration carrying only what
// SkillPickerView's bodies prove (full layout from the MenuItemSlot
// ctor @0x4C88CC / SetSkill @0x4C8FD0 / TouchBegan @0x4C93F0).
// HAND-OFF: the owning slice should replace this with a real
// MenuItemSlot.h. sizeof 656.
class MenuItemSlot : public GUIView {
public:
    MenuItemSlot();
    ~MenuItemSlot() override;  // slots 7, 8

    void Init(float scale);
    void SetSkill(const boost::shared_ptr<Skill>& skill, bool icon);
    // Mirrors MenuItemSlot.h: non-virtual, so declaring it here changes
    // no layout; it lets ItemInfoView's forwarder link to the real
    // MenuItemSlot.cpp definition (same mangled name).
    void SetItem(const boost::shared_ptr<Item>& item);
    void SetSelected(bool selected);

    IMenuItemSlotDelegate* delegate;   // +256
    float field264;                    // +264 (Init scale; name UNRECOVERED)
    boost::shared_ptr<Item> item;      // +272
    boost::shared_ptr<Skill> skill;    // +288
    bool field304;                     // +304 (set when the slot is
                                       // usable; name UNRECOVERED)
    bool selected;                     // +305
    bool field306;                     // +306 (name UNRECOVERED)
    char _pad307[5];                   // +307
    GUITexturedRect texturedRect312;   // +312 (slot background)
    GUITexturedRect texturedRect424;   // +424 (glow)
    GUITexturedRect texturedRect536;   // +536 (skill icon)
    bool field648;                     // +648 (glow flag; name UNRECOVERED)
    char _pad649[3];                   // +649
    float field652;                    // +652 (glow timer; name UNRECOVERED)
};

class SkillPickerView : public GUIView, public IMenuItemSlotDelegate {
public:
    SkillPickerView();
    ~SkillPickerView() override;  // slots 7, 8

    void InitWithGameState(const boost::shared_ptr<GameState>& gameState);
    void SetSlotSelected(int index, bool selected);
    void AddAnimations(bool dismiss);

    void TouchBegan(const FWTouch& touch) override;              // slot 0
    void HandleKeyboardEvent(const GUIKeyboardEvent& event) override;  // 5
    void Update(float deltaTime) override;                       // slot 9
    void DrawRect(RenderingContext* context, const Rectangle& rect,
                  const Matrix4& transform) override;            // slot 10
    void LayoutSubviews() override;                              // slot 11
    void DidMoveToWindow() override;                            // slot 16
    bool CanBecomeFirstResponder() override;                     // slot 24

    // IMenuItemSlotDelegate (primary slots 38/39; thunks on the
    // +256 secondary vtable).
    void MenuItemSlotSelected(MenuItemSlot* slot) override;
    void MenuItemSlotDeselected(MenuItemSlot* slot) override;

    // Non-virtual (not in either golden vtable).
    void Dismiss();

    SkillPickerViewDelegate* delegate;                            // +264
    boost::shared_ptr<GameState> gameState;                       // +272
    boost::shared_ptr<GUILabel> titleLabel;                       // +288
    int slotCount;                                                // +304
    boost::scoped_array<boost::shared_ptr<MenuItemSlot> > slots;  // +312
    boost::scoped_array<boost::shared_ptr<GUILabel> > slotLabels;  // +320
    boost::scoped_array<boost::shared_ptr<GUIView> > slotContainers;  // +328
    int selectedIndex;                                            // +336
};

} // namespace Caver

#endif
