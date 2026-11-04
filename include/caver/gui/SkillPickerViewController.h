// Recovered from libswordigo.so (1.4.13) evidence — IDA/Hex-Rays exports
// in ~/workspace/analysis/swordigo/functions/Caver/
// SkillPickerViewController/ (LoadView's Hex-Rays export failed; its
// body is recovered from the Ghidra dump in ~/workspace/analysis/
// ghidra64/g_64.c and the golden disassembly), the golden vtable
// _ZTVN5Caver25SkillPickerViewControllerE @0x62BDD0 (152 bytes,
// recovered from ABS64 relocations) and the __vmi RTTI @0x62BE78
// (56 bytes, 2 bases).
//
// SkillPickerViewController is the "choose a spell" modal
// GameViewController::GameControlButtonDown (skill-toggle button,
// case 7 @0x432EF4) presents: that call site's operator new(0x60)
// fixes sizeof = 96, and it copies the presenting controller's
// shared_ptr<PlayerProfile> into this controller's +80 member before
// PresentModalViewController.
//
// Bases (RTTI order):
//   +0   GUIViewController (public)
//   +72  ISkillPickerViewDelegate (secondary @0x62BE48, address point
//        0x62BE58; its RTTI @0x62BE68 is a plain __class_type_info —
//        a bare vptr, no data, no destructor slot). The interface's
//        real name is RTTI-proven by the type-name string @0x26AC23
//        ("Caver::ISkillPickerViewDelegate").
// Primary vtable (address point 0x62BDE0, 13 slots):
//   0 ~SkillPickerViewController (D1/D2 @0x4452D0)  1 ~ (D0 @0x44535C)
//   2 GUIViewController::NewLoadingView (not overridden)
//   3 LoadView (@0x444C90)
//   4 GUIViewController::SuspendView (not overridden)
//   5 GUIViewController::ResumeView (not overridden)
//   6 GUIViewController::UnloadView (not overridden)
//   7 GUIViewController::ViewDidAppear (not overridden)
//   8 GUIViewController::ViewWillDisappear (not overridden)
//   9 GUIViewController::Update (not overridden)
//   10 GUIViewController::PresentModalViewController (not overridden)
//   11 SkillPickerViewDidSelectSkill (@0x44508C)
//   12 SkillPickerViewDidDismiss (@0x4452B8)
// Secondary vtable (-72): exactly two slots, the _ZThn72_ thunks to
// SkillPickerViewDidSelectSkill (@0x4452B0) and
// SkillPickerViewDidDismiss (@0x4452C4), in the
// ISkillPickerViewDelegate declaration order.
//
// Layout (ctor @0x444C54 zero-store map; destructor @0x4452D0
// member-release: the +80 shared_ptr's block at +88 is released
// before the base destructor runs):
//   +80  shared_ptr<PlayerProfile> — copied in by the presenting
//        GameViewController (see above); LoadView feeds the view the
//        profile's shared_ptr<GameState> (PlayerProfile+240), and
//        SkillPickerViewDidSelectSkill writes the picked skill into
//        the game state's current skill (GameState+80 =
//        CharacterState+64). Name UNRECOVERED.
// There are no other members: unlike the sibling controllers there is
// NO view member — LoadView's SkillPickerView lives only in a local
// shared_ptr and in the controller's m_view.
//
// NOTE: no user-declared destructor — the golden D0 @0x44535C is a
// full clone of the D2 body plus the operator delete tail, the shape
// clang emits for an implicit destructor (the PauseViewController /
// ConfigureOverlayViewController pattern), NOT the 36-byte call-D1
// thunk a user-declared destructor produces here.
//
// HAND-OFF (SkillPickerView lane): SkillPickerView.h declares this
// same interface under the descriptive name "SkillPickerViewDelegate"
// (its delegate field at SkillPickerView+264). The RTTI proves the
// real name is ISkillPickerViewDelegate; the two declarations are
// ABI-identical (two slots, same order). Rename at merge; the .cpp
// bridges the field with a reinterpret_cast until then.
// Return-type note: SkillPickerView::Dismiss @0x4D6668 propagates
// the delegate call's x0 as its own bool return, and the view lane
// declared DidDismiss returning bool; but a return type is not
// encoded in any mangled name here, and under the recovered
// toolchain only the VOID form reproduces this controller's golden
// 12-byte tail-call body (and its 12-byte thunk clone) byte-exact —
// a bool body with no return statement keeps a frame and calls
// instead of branching. DidDismiss is therefore declared void in
// this header; the discrepancy is UNRECOVERED and flagged for the
// merge (the binary is identical either way at every call site:
// DismissModalViewController sets no deliberate return value).
#ifndef CAVER_SKILLPICKERVIEWCONTROLLER_H
#define CAVER_SKILLPICKERVIEWCONTROLLER_H

#include <boost/shared_ptr.hpp>

#include "GUIViewController.h"

namespace Caver {

class PlayerProfile;
class Skill;
class SkillPickerView;

class SkillPickerViewController;

// Delegate interface implemented at +72 (bare vptr, no destructor —
// see header note). SkillPickerView stores a pointer to it at its
// +264 (LoadView writes `this + 72` there).
class ISkillPickerViewDelegate {
public:
    virtual void SkillPickerViewDidSelectSkill(
        SkillPickerView* view, const boost::shared_ptr<Skill>& skill) = 0; // slot 0
    virtual void SkillPickerViewDidDismiss(SkillPickerView* view) = 0;     // slot 1
};

class SkillPickerViewController : public GUIViewController,
                                  public ISkillPickerViewDelegate {
public:
    SkillPickerViewController();
    // No user-declared destructor: the golden D2 @0x4452D0 / D0
    // @0x44535C pair is the implicit-destructor shape (see above).

    // GUIViewController override (primary slot 3).
    virtual void LoadView();

    // ISkillPickerViewDelegate (primary slots 11, 12).
    virtual void SkillPickerViewDidSelectSkill(
        SkillPickerView* view, const boost::shared_ptr<Skill>& skill);
    virtual void SkillPickerViewDidDismiss(SkillPickerView* view);

    boost::shared_ptr<PlayerProfile> m_profile; // +80  name UNRECOVERED
};

} // namespace Caver

#endif
