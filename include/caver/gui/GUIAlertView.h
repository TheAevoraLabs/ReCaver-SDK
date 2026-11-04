// Recovered from libswordigo.so (1.4.13) evidence — IDA/Hex-Rays exports
// in ~/workspace/analysis/swordigo/functions/Caver/GUIAlertView/, the
// ABS64 relocations of the golden vtable group, and the ctor/dtor
// bodies.
//
// GUIAlertView: the modal alert panel — a GUIFrameView with a title
// label, a message label, and rows of text fields, link buttons,
// custom views and action buttons, presented modally through
// GUIView::PresentModalView and dismissed through Dismiss().
//
// Vtable group _ZTVN5Caver12GUIAlertViewE @0x6343D0.
// Primary address point 0x6343E0, 38 slots = the GUIView set with
// these overrides (all other slots keep the GUIResponder/GUIView/
// GUIFrameView functions):
//   0  TouchBegan              @0x519AA88... (see .cpp; @0x51AA88)
//   2  TouchEnded              @0x51AB0C
//   3  TouchCancelled          @0x51ABAC
//   5  HandleKeyboardEvent     @0x51AA10
//   7/8 ~GUIAlertView D1/D2    @0x51ACF0 (WEAK), D0 @0x51B080 (WEAK)
//   9  Update                  @0x51A948 (4-byte tail to GUIView::Update)
//   10 DrawRect                @0x51A94C (tail to GUIFrameView::DrawRect)
//   11 LayoutSubviews          @0x5193D8 (tail to GUIFrameView::LayoutSubviews)
//   16 DidMoveToWindow         @0x51B0A4 (WEAK)
//   24 CanBecomeFirstResponder @0x51B0E8 (WEAK; returns true)
// Secondary address point 0x634530 (IGUITextFieldDelegate subobject
// at +288), 2 slots with _ZThn288_ thunk-clones in the golden:
//   0  TextFieldDidEndEditing  @0x51ABC4 (thunk clone @0x51AC54)
//   1  TextFieldShouldReturn   @0x51ACE0 (thunk clone @0x51ACE8)
// RTTI _ZTIN5Caver12GUIAlertViewE @0x634540 is a __vmi_class_type_info
// with exactly two bases, in order: GUIFrameView (@0x634558) and
// IGUITextFieldDelegate (@0x634568).
//
// Layout (sizeof 456 = 0x1C8; proven by the ctor @0x5186B4 store map,
// the dtor @0x51ACF0 teardown order, and every method's accesses):
//   +0..287  GUIFrameView base (GUIView 256 + scoped/intrusive frame
//            members + centered bool, padded to 288)
//   +288     IGUITextFieldDelegate vptr
//   +296     IGUIAlertViewDelegate* delegate (raw; set from outside —
//            no in-class writer; name UNRECOVERED)
//   +304     boost::shared_ptr<GUILabel> titleLabel
//   +320     boost::shared_ptr<GUILabel> messageLabel
//   +336     std::vector<boost::shared_ptr<GUITextField>> textFields
//   +360     std::vector<boost::shared_ptr<GUIButton>> buttons
//   +384     std::vector<boost::shared_ptr<GUIButton>> links
//   +408     std::vector<boost::shared_ptr<GUIView>> customViews
//   +432     int field432 — a button index: GameInterfaceBuilder::
//            AlertView stores its index argument here and
//            HandleKeyboardEvent validates it against buttons.size()
//            (name UNRECOVERED)
//   +436     bool field436 — when set, HandleKeyboardEvent's
//            escape/enter path quits the application instead of
//            pressing the button (name UNRECOVERED)
//   +440     int field440 — the button index TouchEnded reports to
//            the delegate; only the ctor writes it (-1) in-class
//            (name UNRECOVERED)
//   +448     unsigned long long touchIdentifier — the FWTouch
//            identifier captured by TouchBegan (0 = none)
#ifndef CAVER_GUIALERTVIEW_H
#define CAVER_GUIALERTVIEW_H

#include <boost/shared_ptr.hpp>
#include <string>
#include <vector>

#include "GUIFrameView.h"
#include "GUITextFieldImpl.h"

namespace Caver {

class GUIButton;
class GUIEvent;
class GUILabel;
class IGUIAlertViewDelegate;

// The text-field delegate interface GUIAlertView implements as its
// secondary base lives in GUITextFieldImpl.h (canonical declaration:
// 0 = TextFieldDidEndEditing, 1 = TextFieldShouldReturn (bool)).

class GUIAlertView : public GUIFrameView, public IGUITextFieldDelegate {
public:
    GUIAlertView();
    ~GUIAlertView() override;  // primary slots 7, 8

    void TouchBegan(const FWTouch& touch) override;        // slot 0
    void TouchEnded(const FWTouch& touch) override;        // slot 2
    void TouchCancelled(const FWTouch& touch) override;    // slot 3
    void HandleKeyboardEvent(const GUIKeyboardEvent& event) override;  // slot 5
    void Update(float deltaTime) override;                 // slot 9
    void DrawRect(RenderingContext* context, const Rectangle& rect,
                  const Matrix4& transform) override;      // slot 10
    void LayoutSubviews() override;                        // slot 11
    void DidMoveToWindow() override;                       // slot 16
    bool CanBecomeFirstResponder() override;               // slot 24

    // IGUITextFieldDelegate (secondary vtable at +288).
    void TextFieldDidEndEditing(GUITextField* textField) override;
    bool TextFieldShouldReturn(GUITextField* textField) override;

    // Non-virtual members.
    void AddAnimations(bool dismiss);
    void PresentAnimationDidFinish(void* sender, GUIEvent* event);
    void UpdateLayout();
    void SetTitle(const std::string& title);
    void SetMessage(const std::string& message);
    void AddTextField(const boost::shared_ptr<GUITextField>& textField);
    void AddButton(const boost::shared_ptr<GUIButton>& button);
    void ButtonPressed(void* sender, GUIEvent* event);
    int AddLink(const std::string& link);
    void AddCustomView(const boost::shared_ptr<GUIView>& view);
    void PresentFromView(GUIView* view);
    void Dismiss();

    IGUIAlertViewDelegate* delegate;                        // +296
    boost::shared_ptr<GUILabel> titleLabel;                 // +304
    boost::shared_ptr<GUILabel> messageLabel;               // +320
    std::vector<boost::shared_ptr<GUITextField> > textFields;  // +336
    std::vector<boost::shared_ptr<GUIButton> > buttons;        // +360
    std::vector<boost::shared_ptr<GUIButton> > links;          // +384
    std::vector<boost::shared_ptr<GUIView> > customViews;      // +408
    int field432;                                           // +432
    bool field436;                                          // +436
    int field440;                                           // +440
    unsigned long long touchIdentifier;                     // +448
};

} // namespace Caver

#endif
