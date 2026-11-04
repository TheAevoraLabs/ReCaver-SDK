// Recovered from libswordigo.so (1.4.13) evidence — IDA/Hex-Rays exports
// in ~/workspace/analysis/swordigo/functions/Caver/LevelUpViewController/,
// the golden vtable _ZTVN5Caver21LevelUpViewControllerE @0x62B5A8
// (136 bytes, recovered from ABS64 relocations) and the __vmi RTTI
// @0x62B630 (56 bytes, 2 bases).
//
// LevelUpViewController is the level-up modal GameViewController::Update
// presents when a level-up is pending (that call site's operator
// new(0x78) fixes sizeof = 120; it writes its own +96
// ILevelUpViewControllerDelegate subobject into this controller's +80
// and copies its shared_ptr<PlayerProfile> (+136) into this
// controller's +88). The controller builds a CharacterView in level-up
// mode; when the view is dismissed after spending points, the delegate
// (GameViewController) applies them via NumPointsForAttribute.
//
// Bases (RTTI order):
//   +0   GUIViewController (public)
//   +72  ICharacterViewDelegate (secondary @0x62B628; its RTTI
//        @0x62B658 is a plain __class_type_info — a bare vptr, no
//        data, no destructor slot; the interface is declared in
//        CharacterView.h, where GameMenuViewController's identical
//        copy also lives)
// Primary vtable (address point 0x62B5B8, 12 slots):
//   0 ~LevelUpViewController (D1/D2 @0x43EF8C)  1 ~ (D0 @0x43F06C)
//   2 GUIViewController::NewLoadingView (not overridden)
//   3 LoadView (@0x43EC18)
//   4 GUIViewController::SuspendView (not overridden)
//   5 GUIViewController::ResumeView (not overridden)
//   6 GUIViewController::UnloadView (not overridden)
//   7 GUIViewController::ViewDidAppear (not overridden)
//   8 GUIViewController::ViewWillDisappear (not overridden)
//   9 GUIViewController::Update (not overridden)
//   10 GUIViewController::PresentModalViewController (not overridden)
//   11 CharacterViewDismissed (@0x43EEF8)
// Secondary vtable (-72): exactly one slot, the _ZThn72_ thunk to
// CharacterViewDismissed (@0x43EF1C).
//
// Layout (ctor @0x43EBD4 zero-store map — 16-byte zero stores at +88
// and +104 only; destructor @0x43EF8C member-release order — the +104
// shared_ptr's block at +112 is released before the +88 shared_ptr's
// block at +96, exact reverse declaration order):
//   +80  ILevelUpViewControllerDelegate* — raw, non-owning; written
//        by the presenting GameViewController (see above). NOT
//        written by the ctor, so it stays uninitialised until then
//        (bug-for-bug, the CharacterView delegate pattern).
//        Name UNRECOVERED.
//   +88  shared_ptr<PlayerProfile> — copied in by the presenting
//        GameViewController; LoadView forwards its gameState member
//        (PlayerProfile+240) to CharacterView::InitWithGameState.
//        Name UNRECOVERED.
//   +104 shared_ptr<CharacterView> — the view built by LoadView
//        (operator new(0x208) = the 520-byte CharacterView); also
//        adopted as the controller's m_view. Name UNRECOVERED.
//
// NOTE: no user-declared destructor — the golden D0 @0x43F06C is a
// full clone of the D2 body plus the operator delete tail, the shape
// clang emits for an implicit destructor (the PauseViewController /
// GameOverViewController pattern).
#ifndef CAVER_LEVELUPVIEWCONTROLLER_H
#define CAVER_LEVELUPVIEWCONTROLLER_H

#include <boost/shared_ptr.hpp>

#include "GUIViewController.h"

// CharacterView.h declares ICharacterViewDelegate (this class's +72
// base) and the complete CharacterView (sizeof 520) whose shared_ptr
// is the +104 member.
#include "CharacterView.h"

namespace Caver {

class PlayerProfile;

class LevelUpViewController;

// The CharacterView attribute selector of NumPointsForAttribute:
// the golden body switches on 0/1/2, selecting the CharacterView's
// health / attack / magic AttributePointsViews respectively (the
// shared_ptrs at CharacterView+376/+392/+408). Enumerator names
// UNRECOVERED (GameViewController.cpp's stub only ever passes the
// literals 0, 1, 2).
enum CharacterAttribute : int {
    CharacterAttribute0 = 0,
    CharacterAttribute1 = 1,
    CharacterAttribute2 = 2
};

// Delegate interface implemented by GameViewController at its +96
// subobject (bare vptr, no destructor — see header note). This
// declaration is token-identical to the one in GameViewController.h;
// the two must be unified at merge.
#ifndef CAVER_ILEVELUPVIEWCONTROLLERDELEGATE_DEFINED
#define CAVER_ILEVELUPVIEWCONTROLLERDELEGATE_DEFINED
class ILevelUpViewControllerDelegate {
public:
    virtual void LevelUpViewControllerDidFinish(LevelUpViewController* controller) = 0; // slot 0
};
#endif

class LevelUpViewController : public GUIViewController,
                              public ICharacterViewDelegate {
public:
    LevelUpViewController();
    // No user-declared destructor: the golden D2 @0x43EF8C / D0
    // @0x43F06C pair is the implicit-destructor shape (see above).

    // GUIViewController override (primary slot 3).
    virtual void LoadView();

    // ICharacterViewDelegate (primary slot 11).
    virtual void CharacterViewDismissed(CharacterView* view);

    // Non-virtual: the points currently shown for one attribute in
    // the CharacterView built by LoadView (0 for any other value).
    int NumPointsForAttribute(CharacterAttribute attribute);

    ILevelUpViewControllerDelegate* m_delegate;       // +80  name UNRECOVERED
    boost::shared_ptr<PlayerProfile> m_profile;       // +88  name UNRECOVERED
    boost::shared_ptr<CharacterView> m_characterView; // +104 name UNRECOVERED
};

static_assert(sizeof(LevelUpViewController) == 120,
              "LevelUpViewController layout: operator new(0x78) in "
              "GameViewController::Update, golden");

} // namespace Caver

#endif
