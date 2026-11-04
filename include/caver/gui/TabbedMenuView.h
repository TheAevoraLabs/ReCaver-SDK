// Recovered from libswordigo.so (1.4.13) evidence — IDA/Hex-Rays
// exports in ~/workspace/analysis/swordigo/functions/Caver/TabbedMenuView/,
// the golden vtables (primary _ZTVN5Caver14TabbedMenuViewE @0x632B18,
// secondary @0x632C60) and RTTI @0x632C78 recovered from ABS64
// relocations, and the ctor @0x4D77E0 / dtor @0x4D9AE0 bodies.
//
// TabbedMenuView: the in-game menu shell — a left tab stripe
// (GUIFrameView with the TabView inside a tab-frame view), a title
// label on the stripe's tag frame, and the selected page
// (MenuViewPage) as content. Tabs are switched through the TabView;
// "close" is a pseudo-tab that dismisses through the delegate.
//
// Derivation (binary-proven):
// - RTTI is a __vmi_class_type_info with bases GUIView (primary,
//   offset 0) and Caver::ITabViewDelegate (@+256): the secondary
//   vtable at +256 holds exactly one slot, the _ZThn256_ thunk to
//   TabViewSelectionDidChange. TabView's delegate pointer
//   (TabView+256) is set to `this + 256` by the ctor.
// - Primary vtable: the 38 GUIView slots (overrides: slot 5
//   HandleKeyboardEvent, 9 Update, 10 DrawRect, 11 LayoutSubviews,
//   16 DidMoveToWindow, 24 CanBecomeFirstResponder) plus slot 38 =
//   TabViewSelectionDidChange, a NEW virtual that also implements
//   ITabViewDelegate's single method.
// - sizeof = 408 (GameMenuViewController::LoadView's operator
//   new(0x198), the ctor store map and the dtor release order —
//   reverse declaration order — agree).
//
// Layout:
//   +0..255 GUIView base
//   +256    ITabViewDelegate base (vptr only)
//   +264    ITabbedMenuViewDelegate* delegate (raw, non-owning;
//           GameMenuViewController::LoadView stores its own +88
//           subobject here; its DismissButtonPressed clears it)
//   +272    boost::shared_ptr<GUIFrameView> frameViewA — a
//           "ui_tab_frame" view added as a subview of this; the
//           TabView is added as ITS subview, and UpdateContentLayout
//           starts the content at its frame's right edge
//   +288    boost::shared_ptr<GUIFrameView> frameViewB — the second
//           "ui_tab_frame" view; LayoutSubviews places it directly
//           left of frameViewA
//   +304    bool field304 — selects the TabView's LayoutSubviews
//           anchor (right/bottom of frameViewA's bounds + 5 when
//           set, x=0 / centred when clear); never written by any
//           TabbedMenuView function in the golden (UNRECOVERED name)
//   +312    boost::shared_ptr<TabView> tabView
//   +328    boost::shared_ptr<MenuViewPage> contentView (the member
//           SetContentView assigns; GameMenuViewController reads the
//           MapMenuPage through it when the "map" tab is selected)
//   +344    boost::shared_ptr<GUIFrameView> stripe — the
//           "ui_menu_stripe" frame view; its frame size is set from
//           its frame texture's sub-rect in the ctor
//   +360    boost::shared_ptr<GUIFrameView> stripeTag — the
//           "ui_menu_stripe_tag" frame view, a subview of stripe,
//           vertically centred on it in the ctor
//   +376    boost::shared_ptr<GUILabel> titleLabel — a subview of
//           stripeTag; SetContentView sets its text from the
//           content page's title (MenuViewPage+256)
//   +392    boost::shared_ptr<GUIView> stripeAccessoryView — set by
//           SetStripeAccessoryView, added as a subview of stripe
// Member NAMES are behaviour-derived and UNRECOVERED.
//
// HAND-OFFS: no TabView.h / MenuViewPage.h / ITabViewDelegate
// declaration exists in the tree yet; TabbedMenuView.cpp carries
// TU-local layout declarations for TabView and MenuViewPage.
// ITabbedMenuViewDelegate is declared in GameMenuViewController.h
// as well (identical shape) — reconcile at merge.
#ifndef CAVER_TABBEDMENUVIEW_H
#define CAVER_TABBEDMENUVIEW_H

#include <boost/shared_ptr.hpp>
#include <string>

#include "GUIView.h"

namespace Caver {

class GUIFrameView;
class GUILabel;
class MenuViewPage;
class TabView;
class TabbedMenuView;

// The TabView delegate interface (bare vptr, no destructor — the
// golden secondary vtable for this base has exactly one slot).
class ITabViewDelegate {
public:
    virtual void TabViewSelectionDidChange(TabView* tabView) = 0;  // slot 0
};

// The menu controller's callback interface (bare vptr, no
// destructor; identical to the declaration in
// GameMenuViewController.h — see the hand-off note above).
class ITabbedMenuViewDelegate {
public:
    virtual void TabbedMenuViewSelectedTabDidChange(TabbedMenuView* view) = 0;   // slot 0
    virtual void TabbedMenuViewDismissButtonPressed(TabbedMenuView* view) = 0;   // slot 1
};

class TabbedMenuView : public GUIView, public ITabViewDelegate {
public:
    TabbedMenuView();
    ~TabbedMenuView() override;  // slots 7, 8

    // GUIView overrides.
    void HandleKeyboardEvent(const GUIKeyboardEvent& event) override;  // slot 5
    void Update(float deltaTime) override;                             // slot 9
    void DrawRect(RenderingContext* context, const Rectangle& rect,
                  const Matrix4& transform) override;                  // slot 10
    void LayoutSubviews() override;                                    // slot 11
    void DidMoveToWindow() override;                                   // slot 16
    bool CanBecomeFirstResponder() override;                           // slot 24

    // ITabViewDelegate implementation; a new virtual in the primary
    // vtable as well (slot 38).
    void TabViewSelectionDidChange(TabView* tabView) override;         // slot 38

    void AddTab(const std::string& name);
    void SetSelectedTab(const std::string& name);
    void SetContentView(const boost::shared_ptr<MenuViewPage>& view);
    void SetStripeAccessoryView(const boost::shared_ptr<GUIView>& view);
    void UpdateContentLayout();
    void AddAnimations(bool dismiss);
    void Dismiss();

    ITabbedMenuViewDelegate* delegate;                    // +264
    boost::shared_ptr<GUIFrameView> frameViewA;           // +272
    boost::shared_ptr<GUIFrameView> frameViewB;           // +288
    bool field304;                                        // +304 (name UNRECOVERED)
    char _pad305[7];                                      // +305
    boost::shared_ptr<TabView> tabView;                   // +312
    boost::shared_ptr<MenuViewPage> contentView;          // +328
    boost::shared_ptr<GUIFrameView> stripe;               // +344
    boost::shared_ptr<GUIFrameView> stripeTag;            // +360
    boost::shared_ptr<GUILabel> titleLabel;               // +376
    boost::shared_ptr<GUIView> stripeAccessoryView;       // +392
};

} // namespace Caver

#endif
