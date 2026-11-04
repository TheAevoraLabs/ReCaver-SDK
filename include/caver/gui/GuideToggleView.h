// Recovered from libswordigo.so (1.4.13) evidence.
//
// GuideToggleView: a settings row pairing a GUILabel ("TOGGLE") with
// a GUISwitch; switch value-changed events are forwarded to a delegate
// through IGuideToggleViewDelegate.
// Vtable _ZTVN5Caver15GuideToggleViewE @0x630858, 320 bytes = the 38
// GUIView slots with slot 7/8 (~GuideToggleView) and slot 11
// (LayoutSubviews) overridden (ABS64 relocation dump). RTTI
// @0x630998 is a __si_class_type_info whose single base is GUIView.
//
// Layout: GUIView (256 bytes), then
//   +256 IGuideToggleViewDelegate* delegate (raw, non-owning;
//        member name UNRECOVERED)
//   +264 boost::shared_ptr<GUILabel> label (created by the ctor via
//        GameInterfaceBuilder::SmallLabel)
//   +280 boost::shared_ptr<GUISwitch> toggleSwitch (created by the
//        ctor via GameInterfaceBuilder::Switch)
#ifndef CAVER_GUIDETOGGLEVIEW_H
#define CAVER_GUIDETOGGLEVIEW_H

#include <boost/shared_ptr.hpp>

#include "GUIEvent.h"
#include "GUIView.h"

namespace Caver {

class GUILabel;
class GUISwitch;
class GuideToggleView;

#ifndef CAVER_IGUIDETOGGLEVIEWDELEGATE_DEFINED
#define CAVER_IGUIDETOGGLEVIEWDELEGATE_DEFINED
class IGuideToggleViewDelegate {
public:
    virtual void GuideToggleViewEnableStateChanged(
        GuideToggleView* view) = 0;  // slot 0
};
#endif

class GuideToggleView : public GUIView {
public:
    GuideToggleView();
    ~GuideToggleView();

    void LayoutSubviews() override;  // slot 11

    void SwitchToggled(void* sender, GUIEvent* event);

    IGuideToggleViewDelegate* delegate;          // +256 UNRECOVERED name
    boost::shared_ptr<GUILabel> label;           // +264
    boost::shared_ptr<GUISwitch> toggleSwitch;   // +280
};

} // namespace Caver

#endif
