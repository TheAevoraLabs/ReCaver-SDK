// Recovered from libswordigo.so (1.4.13) evidence — IDA/Hex-Rays exports
// in ~/workspace/analysis/swordigo/functions/Caver/GameOverViewController/,
// the golden vtable _ZTVN5Caver22GameOverViewControllerE @0x62A4B8
// (136 bytes, recovered from ABS64 relocations) and the __vmi RTTI
// @0x62A550 (56 bytes, 2 bases).
//
// GameOverViewController is the game-over modal GameViewController
// presents on the game-over game event (GameViewController::
// HandleGameEvent @0x431F08, via the PLT stub @0x60AE90): operator
// new(0x80) there and the ctor @0x424894 zero-store map fix
// sizeof = 128. That call site also copies the presenting
// controller's shared_ptr<PlayerProfile> into this controller's +88
// member, and transitions to it with (2.0f, 0.4f, false).
//
// Bases (RTTI order):
//   +0   GUIViewController (public)
//   +72  IGameOverViewDelegate (secondary @0x62A528; its RTTI
//        @0x62A540 is a plain __class_type_info — a bare vptr, no
//        data, no destructor slot)
// Primary vtable (address point 0x62A4C8, 12 slots):
//   0 ~GameOverViewController (D1/D2 @0x424FB4)  1 ~ (D0 @0x425094)
//   2 GUIViewController::NewLoadingView (not overridden)
//   3 LoadView (@0x4248E0)
//   4 SuspendView (@0x424B2C, empty)
//   5 GUIViewController::ResumeView (not overridden)
//   6 UnloadView (@0x424B30, empty)
//   7 GUIViewController::ViewDidAppear (not overridden)
//   8 GUIViewController::ViewWillDisappear (not overridden)
//   9 Update(float) (@0x424B34)
//   10 GUIViewController::PresentModalViewController (not overridden)
//   11 GameOverViewDidContinue (@0x424D08)
// Secondary vtable (-72): exactly one slot, the _ZThn72_ thunk to
// GameOverViewDidContinue (@0x424FAC).
//
// Layout (ctor @0x424894 zero-store map; destructor @0x424FB4
// member-release order — the +104 shared_ptr's block at +112 is
// released before the +88 shared_ptr's block at +96, exact reverse
// declaration order):
//   +80  bool — the "ad already shown" flag: ShowAdMaybe tests and
//        sets it, Update's timed ad path and the copy of the ad code
//        inlined into GameOverViewDidContinue test it too. The ctor
//        zero-stores the byte on its own, ahead of the shared_ptrs.
//        Name UNRECOVERED.
//   +88  shared_ptr<PlayerProfile> — copied in by the presenting
//        GameViewController (see above) and forwarded by
//        GameOverViewDidContinue into the new GameViewController's
//        own profile member (+136). Name UNRECOVERED.
//   +104 shared_ptr<GameOverView> — the view built by LoadView
//        (operator new(0x118) = the 280-byte GameOverView); also
//        adopted as the controller's m_view. Name UNRECOVERED.
//   +120 float — the elapsed-time accumulator Update adds each
//        frame's delta to; the ad fires once it exceeds 4.5 while
//        the +80 flag is clear. Name UNRECOVERED.
//   +124 bool — GameOverViewDidContinue's once-guard: set on entry,
//        a second continue is ignored. Name UNRECOVERED.
// The ctor's final unaligned 8-byte zero store at +117 covers the
// tail of the +104 shared_ptr plus the +120 float and the +124 bool.
//
// NOTE: no user-declared destructor — the golden D0 @0x425094
// (232 bytes) is a full clone of the D2 body (224 bytes) plus the
// operator delete tail, the shape clang emits for an implicit
// destructor (the PurchaseViewController pattern), NOT the 36-byte
// call-D1 thunk a user-declared destructor produces here.
#ifndef CAVER_GAMEOVERVIEWCONTROLLER_H
#define CAVER_GAMEOVERVIEWCONTROLLER_H

#include <boost/shared_ptr.hpp>

// The GUIViewController base declaration is taken from
// GUINavigationController.h (the MainMenuViewController pattern):
// GUIViewController.h additionally defines a stub GameViewController
// whose name the .cpp needs for its own proven-shape declaration of
// the real class. The two base declarations are ABI-identical;
// reconcile at merge.
#include "GUINavigationController.h"

namespace Caver {

class GameOverView;
class PlayerProfile;

class GameOverViewController;

// Delegate interface implemented at +72 (bare vptr, no destructor —
// see header note). GameOverView stores a pointer to it at its +256
// (LoadView writes `this + 72` there; GameOverViewDidContinue clears
// the view's pointer before transitioning away).
class IGameOverViewDelegate {
public:
    virtual void GameOverViewDidContinue(GameOverView* view) = 0; // slot 0
};

class GameOverViewController : public GUIViewController,
                               public IGameOverViewDelegate {
public:
    GameOverViewController();
    // No user-declared destructor: the golden D2 @0x424FB4 / D0
    // @0x425094 pair is the implicit-destructor shape (see above).

    // GUIViewController overrides (primary slots 3, 4, 6, 9).
    virtual void LoadView();
    virtual void SuspendView();
    virtual void UnloadView();
    virtual void Update(float deltaTime);

    // IGameOverViewDelegate (primary slot 11).
    virtual void GameOverViewDidContinue(GameOverView* view);

    // Non-virtual: the one-shot interstitial-ad helper shared by
    // Update and GameOverViewDidContinue (both golden bodies carry
    // its code inlined; the out-of-line copy is @0x424C38).
    void ShowAdMaybe();

    bool m_field80;                                     // +80  UNRECOVERED name
    char _pad81[7];                                     // +81
    boost::shared_ptr<PlayerProfile> m_profile;         // +88  name UNRECOVERED
    boost::shared_ptr<GameOverView> m_gameOverView;     // +104 name UNRECOVERED
    float m_field120;                                   // +120 UNRECOVERED name
    bool m_field124;                                    // +124 UNRECOVERED name
    char _pad125[3];                                    // +125 -> 128
};

} // namespace Caver

#endif
