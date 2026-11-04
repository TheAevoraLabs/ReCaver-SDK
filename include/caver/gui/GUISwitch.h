// Recovered from libswordigo.so (1.4.13) evidence.
//
// GUISwitch: a GUIControl on/off switch — a GUIFrameView showing one
// of two frame textures, a state bit in the GUIControl state word,
// an optional click sound, and touch tracking by touch identifier.
// Vtable _ZTVN5Caver9GUISwitchE @0x635390, 328 bytes = the 39
// GUIControl slots (38 GUIView slots + StateDidChange) with slot 0-3
// (TouchBegan/Moved/Ended/Cancelled), slot 7/8 (~GUISwitch) and
// slot 11 (LayoutSubviews) overridden (ABS64 relocation dump).
// RTTI @0x6354D8 is a __si_class_type_info whose single base is
// Caver::GUIControl (@0x634988).
//
// Layout: GUIControl (280 bytes), then
//   +280 int state (bit 0 set while the switch is pressed-in; every
//        change fires StateDidChange through vtable slot 38)
//   +288 boost::intrusive_ptr<Texture> onTexture
//   +296 boost::intrusive_ptr<Texture> offTexture
//   +304 boost::shared_ptr<GUIFrameView> frameView (created and added
//        as a subview by the ctor)
//   +320 bool on
//   +328 std::string soundName (played by TouchBegan when non-empty;
//        member name UNRECOVERED)
//   +352 unsigned long long touchIdentifier (the tracked FWTouch's
//        identifier; 0 = no active touch)
#ifndef CAVER_GUISWITCH_H
#define CAVER_GUISWITCH_H

#include <boost/intrusive_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <string>

#include "../render/Texture.h"
#include "GUIControl.h"

namespace Caver {

class GUIFrameView;

class GUISwitch : public GUIControl {
public:
    GUISwitch();
    ~GUISwitch();

    void TouchBegan(const FWTouch& touch) override;      // slot 0
    void TouchMoved(const FWTouch& touch) override;      // slot 1
    void TouchEnded(const FWTouch& touch) override;      // slot 2
    void TouchCancelled(const FWTouch& touch) override;  // slot 3
    void LayoutSubviews() override;                      // slot 11

    void SetOnFrameTexture(const boost::intrusive_ptr<Texture>& texture);
    void SetOffFrameTexture(const boost::intrusive_ptr<Texture>& texture);
    void UpdateFrameTexture();
    void CancelPress();

    int state;                                    // +280
    boost::intrusive_ptr<Texture> onTexture;      // +288
    boost::intrusive_ptr<Texture> offTexture;     // +296
    boost::shared_ptr<GUIFrameView> frameView;    // +304
    bool on;                                      // +320
    char _pad321[7];                              // +321
    std::string soundName;                        // +328 UNRECOVERED name
    unsigned long long touchIdentifier;           // +352
};

} // namespace Caver

#endif
