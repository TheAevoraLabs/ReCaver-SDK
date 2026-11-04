// Recovered from libswordigo.so (1.4.13) evidence — IDA/Hex-Rays exports
// in ~/workspace/analysis/swordigo/functions/Caver/ConfigureOverlayView/,
// the golden vtable group _ZTVN5Caver20ConfigureOverlayViewE @0x62E2C0
// and the RTTI _ZTIN5Caver20ConfigureOverlayViewE @0x62E460.
//
// ConfigureOverlayView: the "configure controls" overlay presented by
// ConfigureOverlayViewController (see GameMenuViewController.cpp) —
// it hosts a GameOverlayView whose controls the user drags into place
// (through DraggableOverlayItem proxies), plus Done / Reset buttons.
//
// Bases (RTTI-proven, __vmi_class_type_info with 3 bases, in order):
//   GUIView                        @+0   (primary)
//   IDraggableOverlayItemDelegate  @+256 (secondary vtable @0x62E428,
//                                         1 slot: DraggableOverlayItemDidMove,
//                                         _ZThn256_ thunk @0x481E2C)
//   IGUIAlertViewDelegate          @+264 (secondary vtable @0x62E440,
//                                         2 slots: AlertViewClickedButtonAtIndex
//                                         / AlertViewWasDismissed,
//                                         _ZThn264_ thunks @0x48204C/0x48206C)
// Primary vtable: 41 slots = the 38 GUIView slots (overrides: slot 5
// HandleKeyboardEvent, 11 LayoutSubviews, 16 DidMoveToWindow, 24
// CanBecomeFirstResponder, 29 HitTest, 7/8 dtors) plus the three
// delegate methods appended at slots 38/39/40 in declaration order
// (DraggableOverlayItemDidMove, AlertViewClickedButtonAtIndex,
// AlertViewWasDismissed).
//
// Layout (sizeof 384 = 0x180, proven by operator new(0x180) in
// ConfigureOverlayViewController::LoadView, the ctor @0x47FC18
// zero-store map +272..+383, and the dtor @0x482604 teardown order
// alert -> items -> layout -> gameOverlay -> reset -> done):
//   +256 IDraggableOverlayItemDelegate vptr
//   +264 IGUIAlertViewDelegate vptr
//   +272 IConfigureOverlayViewDelegate* m_delegate (RAW; written by
//        ConfigureOverlayViewController::LoadView as controller+72;
//        slot 0 = ConfigureOverlayViewLayoutDidChange,
//        slot 1 = ConfigureOverlayViewDismissButtonPressed — call
//        sites in ButtonPressed / DraggableOverlayItemDidMove /
//        AlertViewClickedButtonAtIndex)
//   +280 boost::shared_ptr<GUIButton> m_doneButton   ("Done")
//   +296 boost::shared_ptr<GUIButton> m_resetButton  ("Reset")
//   +312 boost::shared_ptr<GameOverlayView> m_gameOverlayView
//   +328 boost::shared_ptr<GUIViewLayout> m_layout
//   +344 std::vector<boost::shared_ptr<DraggableOverlayItem>> m_items
//   +368 boost::shared_ptr<GUIAlertView> m_alertView
#ifndef CAVER_CONFIGUREOVERLAYVIEW_H
#define CAVER_CONFIGUREOVERLAYVIEW_H

#include <string>
#include <vector>

#include <boost/shared_ptr.hpp>

#include "../core/Color.h"
#include "../core/Rectangle.h"
#include "../core/Vector2.h"
#include "GUIView.h"
#include "GUIRoundedRect.h"

namespace Caver {

class ConfigureOverlayView;
class DraggableOverlayItem;
class GameOverlayView;
class GameState;
class GUIAlertView;
class GUIButton;
class GUIEvent;
class GUIViewLayout;

// The draggable-item delegate interface (secondary base at +256).
// Its _ZTI @0x62E450 is a plain __class_type_info (no bases).
class IDraggableOverlayItemDelegate {
public:
    virtual void DraggableOverlayItemDidMove(DraggableOverlayItem* item) = 0;  // slot 0
};

// The alert-view delegate interface, reproduced token-identically
// from GuidePurchaseViewController.h (the canonical declaration);
// the duplicates must be unified at merge time. Slot order proven by
// the golden IGUIAlertViewDelegate implementors: 0 = clicked (bool),
// 1 = dismissed.
#ifndef CAVER_IGUIALERTVIEWDELEGATE_DEFINED
#define CAVER_IGUIALERTVIEWDELEGATE_DEFINED
class IGUIAlertViewDelegate {
public:
    virtual bool AlertViewClickedButtonAtIndex(GUIAlertView* alertView, int index) = 0; // slot 0
    virtual void AlertViewWasDismissed(GUIAlertView* alertView) = 0;                    // slot 1
};
#endif

// Delegate notified by this view (raw pointer at +272; implemented
// by ConfigureOverlayViewController as its +72 subobject). Slot
// order proven by the call sites: ButtonPressed's Done path calls
// slot 1, DraggableOverlayItemDidMove / AlertViewClicked call slot 0.
class IConfigureOverlayViewDelegate {
public:
    virtual void ConfigureOverlayViewLayoutDidChange(ConfigureOverlayView* view) = 0;       // slot 0
    virtual void ConfigureOverlayViewDismissButtonPressed(ConfigureOverlayView* view) = 0;  // slot 1
};

// DraggableOverlayItem: the draggable proxy for one GameOverlayView
// control (recovered from functions/Caver/DraggableOverlayItem/;
// declared here until its own slice gives it a header — reconcile
// at merge). A GUIView (256) plus:
//   +256 IDraggableOverlayItemDelegate* m_delegate (RAW; TouchMoved
//        calls its slot 0 with this item)
//   +264 char[16] UNRECOVERED (zeroed by the ctor @0x485050)
//   +280 GUIRoundedRect m_roundedRect1 (168 bytes; drawn normally)
//   +448 GUIRoundedRect m_roundedRect2 (168 bytes; drawn while the
//        item is touched — selected in DrawRect by m_touchIdentifier)
//   +616 std::string m_identifier (the GUIViewLayout subview id)
//   +640 unsigned long long m_touchIdentifier (FWTouch id captured
//        by TouchBegan; cleared by TouchEnded/TouchCancelled)
//   +648 Vector2 m_touchOffset (touch location in the item at
//        TouchBegan; TouchMoved drags the frame by the delta)
// sizeof 656 = 0x290 (operator new(0x290) in InitWithGameState).
class DraggableOverlayItem : public GUIView {
public:
    DraggableOverlayItem();
    void SetColor(Color color);

    IDraggableOverlayItemDelegate* m_delegate;  // +256
    char _pad264[16];                           // +264 UNRECOVERED
    GUIRoundedRect m_roundedRect1;              // +280
    GUIRoundedRect m_roundedRect2;              // +448
    std::string m_identifier;                   // +616
    unsigned long long m_touchIdentifier;       // +640
    Vector2 m_touchOffset;                      // +648
};

class ConfigureOverlayView : public GUIView,
                             public IDraggableOverlayItemDelegate,
                             public IGUIAlertViewDelegate {
public:
    ConfigureOverlayView();
    ~ConfigureOverlayView() override;  // primary slots 7, 8

    void HandleKeyboardEvent(const GUIKeyboardEvent& event) override;  // slot 5
    void LayoutSubviews() override;                                    // slot 11
    void DidMoveToWindow() override;                                   // slot 16
    bool CanBecomeFirstResponder() override;                           // slot 24
    boost::shared_ptr<GUIView> HitTest(const Vector2& point) override;  // slot 29

    // Delegate methods (primary slots 38-40, in this order; also
    // reached through the +256/+264 secondary vtables via thunks).
    void DraggableOverlayItemDidMove(DraggableOverlayItem* item) override;        // slot 38
    bool AlertViewClickedButtonAtIndex(GUIAlertView* alertView, int index) override;  // slot 39
    void AlertViewWasDismissed(GUIAlertView* alertView) override;                // slot 40

    // Non-virtual members.
    void InitWithGameState(const boost::shared_ptr<GameState>& gameState);
    void ButtonPressed(void* sender, GUIEvent* event);
    void LayoutItem(DraggableOverlayItem* item);
    void AddTransitionAnimations(bool dismiss);

    IConfigureOverlayViewDelegate* m_delegate;              // +272
    boost::shared_ptr<GUIButton> m_doneButton;              // +280
    boost::shared_ptr<GUIButton> m_resetButton;             // +296
    boost::shared_ptr<GameOverlayView> m_gameOverlayView;   // +312
    boost::shared_ptr<GUIViewLayout> m_layout;              // +328
    std::vector<boost::shared_ptr<DraggableOverlayItem> > m_items;  // +344
    boost::shared_ptr<GUIAlertView> m_alertView;            // +368
};

} // namespace Caver

#endif
