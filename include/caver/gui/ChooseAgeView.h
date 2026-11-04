// Recovered from libswordigo.so (1.4.13) evidence — IDA/Hex-Rays
// exports in ~/workspace/analysis/swordigo/functions/Caver/
// ChooseAgeView/, the golden vtable _ZTVN5Caver13ChooseAgeViewE
// @0x62C8D8 (320 bytes = 16-byte header + exactly 38 slots, ABS64
// relocations) and the RTTI _ZTIN5Caver13ChooseAgeViewE @0x62CA18
// (__si_class_type_info, single base Caver::GUIView).
//
// ChooseAgeView: the age-picker panel the main menu's neutral-age
// flow presents (MainMenuViewController::ShowNeutralAgeScreenDialog
// allocates it with operator new(0x168) and installs itself as the
// delegate at +256). A slider frame plate, a content frame, an age
// label, a 0..65 slider and two arrow frame views showing the
// slider's position.
//
// Vtable: the 38 GUIView slots, overriding only
//   7/8  ~ChooseAgeView (D1/D2 @0x455524, D0 @0x45574C)
//   11   LayoutSubviews (@0x4552A0)
// SliderValueChanged and UpdateArrowFrames are non-virtual (the
// slider reaches the former through a boost::bind target action).
//
// Layout (sizeof 360 = 0x168, proven by the operator new(0x168) in
// MainMenuViewController::ShowNeutralAgeScreenDialog, the ctor
// @0x4542E8 store map, the dtor @0x455524 teardown order — the six
// shared_ptrs release in exact reverse declaration order — and
// every method body's accesses):
//   +0..255  GUIView base. The AGE itself is an int at +252, inside
//            the GUIView tail (it overlaps the top byte of the
//            unaligned colour dword at +249 and the pad at +253):
//            the ctor zeroes it with the same 16-byte store that
//            zeroes the delegate, SliderValueChanged writes it
//            (str w20, [this+0xFC]) before notifying the delegate,
//            and MainMenuViewController::ChooseAgeViewAgeChanged
//            reads it back at +252. There is no room for a named
//            member there, so the .cpp accesses it by raw offset.
//   +256     IChooseAgeViewDelegate* delegate (raw; the interface
//            is declared in MainMenuViewController.h — its single
//            slot is ChooseAgeViewAgeChanged(ChooseAgeView*))
//   +264     boost::shared_ptr<GUIFrameView> frameView — the back
//            plate ("ui_slider_frame", frame colour
//            (0x5A, 0x64, 0x82, 0xFF)); LayoutSubviews sets its
//            frame to this view's bounds.
//   +280     boost::shared_ptr<GUIFrameView> contentFrameView — a
//            second, identical plate; LayoutSubviews sets its
//            frame to the content rect (bounds inset by (10, 45)
//            origin / (20, 55) size).
//   +296     boost::shared_ptr<GUILabel> ageLabel — both text
//            alignments 4, font "megalopolis_36", a white->white
//            GradientColor, shadow field352/353/360 = true /
//            (0, 0, 0, 255) / (1, -1), initial text "_ _";
//            SliderValueChanged replaces the text with the age.
//   +312     boost::shared_ptr<GUISlider> slider — built by
//            GameInterfaceBuilder::Slider(0, 65), roundValue and
//            field357 both set, and a target action for control
//            event 4096 bound to SliderValueChanged.
//   +328     boost::shared_ptr<GUIFrameView> leftArrow — frame
//            texture "arro" (the name is exactly four chars in the
//            golden ctor: length byte 8, terminator stored at +5 —
//            bug-for-bug), sized to the texture's subrect, user
//            interaction disabled; UpdateArrowFrames positions it
//            left of the slider indicator.
//   +344     boost::shared_ptr<GUIFrameView> rightArrow — frame
//            texture "arro_r", otherwise identical; positioned
//            right of the indicator.
// Member NAMES are behaviour-derived and UNRECOVERED.
#ifndef CAVER_CHOOSEAGEVIEW_H
#define CAVER_CHOOSEAGEVIEW_H

#include <boost/shared_ptr.hpp>

#include "GUIView.h"
#include "MainMenuViewController.h"

namespace Caver {

class GUIFrameView;
class GUILabel;
class GUISlider;
class GUIEvent;

class ChooseAgeView : public GUIView {
public:
    ChooseAgeView();
    ~ChooseAgeView() override;  // slots 7, 8

    void LayoutSubviews() override;  // slot 11

    void SliderValueChanged(void* sender, GUIEvent* event);
    void UpdateArrowFrames();

    IChooseAgeViewDelegate* delegate;                 // +256
    boost::shared_ptr<GUIFrameView> frameView;        // +264
    boost::shared_ptr<GUIFrameView> contentFrameView; // +280
    boost::shared_ptr<GUILabel> ageLabel;             // +296
    boost::shared_ptr<GUISlider> slider;              // +312
    boost::shared_ptr<GUIFrameView> leftArrow;        // +328
    boost::shared_ptr<GUIFrameView> rightArrow;       // +344
};

// Golden aarch64 NDK-libc++ layout anchor; MSVC STL member sizes differ,
// so this assert is Android/Linux-only (the Windows build is self-consistent
// under MSVC STL and does not claim golden byte-layout parity).
#ifndef _MSC_VER
static_assert(sizeof(ChooseAgeView) == 360,
              "ChooseAgeView layout: operator new(0x168) in "
              "MainMenuViewController::ShowNeutralAgeScreenDialog, golden");
#endif

} // namespace Caver

#endif
