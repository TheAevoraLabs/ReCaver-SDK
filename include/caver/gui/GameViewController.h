// Recovered from libswordigo.so (1.4.13) evidence — IDA/Hex-Rays exports
// in ~/workspace/analysis/swordigo/functions/Caver/GameViewController/,
// the golden vtable _ZTVN5Caver18GameViewControllerE @0x62A918 (424
// bytes, recovered from ABS64 relocations + raw offset-to-top words)
// and the RTTI _ZTIN5Caver18GameViewControllerE @0x62AD70 (152 bytes,
// __vmi_class_type_info, 8 bases).
//
// NOTE: a pre-recovery stub of this class still lives in
// GUIViewController.h (a 136-byte pad + shared_ptr<PlayerProfile> at
// +136). That header is another slice's file and is NOT edited here;
// this header carries the real declaration. Because GUIViewController.h
// defines its stub GameViewController, this header takes the verified
// GUIViewController base declaration from GUINavigationController.h
// (the two headers cannot be included in one TU until the merge
// reconciles them — same situation GUINavigationController.h notes).
//
// Derivation (RTTI-proven, base order + offset_flags in the RTTI):
//   +0   GUIViewController (public, offset 0) — primary base, sizeof 72
//   +72  IGameEventResponder               (offset_flags 0x4802)
//   +80  IGameControlDelegate              (0x5000)
//   +88  IGameMenuViewControllerDelegate   (0x5800)
//   +96  ILevelUpViewControllerDelegate    (0x6000)
//   +104 IPortalViewControllerDelegate     (0x6800)
//   +112 IProfileManagerDelegate           (0x7000)
//   +120 IStoreViewControllerDelegate      (0x7800)
// Each delegate interface is a bare vptr (no data, no destructor slot):
// the matching secondary vtables in the golden contain exactly the
// interface's methods as _ZThn<adj>_ thunks and nothing else. The
// ProfileManager::RemoveDelegate call in the destructor passes
// `this + 112`, confirming the IProfileManagerDelegate subobject.
//
// Primary vtable (address point 0x62A928, 24 slots):
//   0 ~GameViewController (D1 @0x42C6E4)   1 ~GameViewController (D0 @0x42CA58)
//   2 NewLoadingView (@0x438BD0)           3 LoadView (@0x42D570)
//   4 SuspendView (@0x42FBAC)              5 ResumeView (@0x430040)
//   6 UnloadView (@0x430110)               7 ViewDidAppear (@0x42F650)
//   8 GUIViewController::ViewWillDisappear (not overridden)
//   9 Update(float) (@0x430F8C)
//   10 GUIViewController::PresentModalViewController (not overridden)
//   11 HandleGameEvent                       (@0x431F08)
//   12 ProfileManagerDidDownloadProfile      (@0x430134)
//   13 GameMenuViewControllerDidQuitToMenu   (@0x430998)
//   14 GameMenuViewControllerDismissed       (@0x430BE8)
//   15 LevelUpViewControllerDidFinish        (@0x430CA8)
//   16 PortalViewControllerDidGotoLevel      (@0x430DBC)
//   17 GameMenuViewControllerDidSetGuideEnabled      (@0x430EFC)
//   18 GameMenuViewControllerDidSetCoinDoublerEnabled (@0x430F30)
//   19 PortalViewControllerDidSetGuideEnabled        (@0x430F58)
//   20 StoreViewControllerDismissed          (@0x432CC4)
//   21 StoreViewControllerDidPurchaseProduct (@0x432DCC, empty)
//   22 GameControlButtonDown                  (@0x432EF4)
//   23 GameControlButtonUp                    (@0x4332B4)
// Secondary vtables (offset-to-top / slots):
//   -72  IGameEventResponder:        HandleGameEvent thunk
//   -80  IGameControlDelegate:       GameControlButtonDown, GameControlButtonUp thunks
//   -88  IGameMenuViewControllerDelegate: DidQuitToMenu, Dismissed,
//        DidSetGuideEnabled, DidSetCoinDoublerEnabled thunks
//   -96  ILevelUpViewControllerDelegate: DidFinish thunk
//   -104 IPortalViewControllerDelegate:  DidGotoLevel, DidSetGuideEnabled thunks
//   -112 IProfileManagerDelegate:        DidDownloadProfile thunk
//   -120 IStoreViewControllerDelegate:   Dismissed, DidPurchaseProduct thunks
//
// Layout (sizeof = 352, from the ctor @0x42C64C zero-store map and the
// destructor @0x42C6E4 member-release order, which is exact reverse
// declaration order: string@328, intrusive@312 + shared_ptr@296,
// shared_ptr@272, string@240, shared_ptrs @216..@136, base):
//   +128 void* (8 bytes, zeroed by the ctor, never released or
//        dereferenced in the recovered bodies) — UNRECOVERED
//   +136 shared_ptr<PlayerProfile> m_profile (SaveGameState calls
//        PlayerProfile::Save on it; ResumeView reads the std::string
//        at profile+24)
//   +152 shared_ptr<void> — assigned in LoadGameState from the
//        shared_ptr inside PlayerProfile at +224/+232; pointee type
//        UNRECOVERED
//   +168 shared_ptr<GameState> m_gameState (GuideTargetCompleted is
//        called on it; the guide/coin-doubler flags live in the
//        GameState object at +488..+491)
//   +184 shared_ptr<MapNode> — second argument of
//        SceneLoadingView::InitWithGameState in NewLoadingView;
//        member name UNRECOVERED
//   +200 shared_ptr<GameSceneController> m_sceneController
//        (GameSceneController::InitWithScene / ApplyLevelUp targets)
//   +216 shared_ptr<GameSceneView> m_sceneView (its +272 word is the
//        GameOverlayView* SaveGameState passes to GameOverlayView::Layout
//        and the destructor clears through overlay+256)
//   +232 bool — UNRECOVERED (set in LoadView, read in Update)
//   +233 bool — UNRECOVERED (CheckAndEnableNewPurchases passes
//        (this byte == 0) to UpdateGuideTarget)
//   +240 std::string — UNRECOVERED name
//   +264 bool — UNRECOVERED
//   +272 shared_ptr<void> — pointee type UNRECOVERED
//   +288 bool — UNRECOVERED
//   +296 shared_ptr<GuideTarget> m_guideTarget (passed to
//        GameState::GuideTargetCompleted; ScheduleGuideTargetUpdate
//        tests it for null)
//   +312 intrusive_ptr<SceneObject> m_guideTargetObject — proven
//        intrusive: RemoveGuideTarget zeroes the member, writes byte
//        +248 of the object, decrements the count at object+8 and
//        destroys through vtable slot +8 at zero (the
//        ReferenceCountedObject release shape); the byte's name is
//        UNRECOVERED
//   +320 float m_guideTargetTimer (ScheduleGuideTargetUpdate stores
//        1.5f here when no guide target is set)
//   +328 std::string — UNRECOVERED name
#ifndef CAVER_GAMEVIEWCONTROLLER_H
#define CAVER_GAMEVIEWCONTROLLER_H

#include <string>
#include <boost/intrusive_ptr.hpp>
#include <boost/shared_ptr.hpp>

#include "GUINavigationController.h"

namespace Caver {

class GameEvent;
class GameState;
class GuideTarget;
class Item;
class MapNode;
class PlayerProfile;
class ProfileManager;
class Scene;
class SceneObject;
class Skill;

class GameMenuViewController;
class LevelUpViewController;
class PortalViewController;
class StoreViewController;

enum GameControlButton : int;

// Delegate interfaces (bare vptrs, no destructor — see header note).
class IGameEventResponder {
public:
    // Return type corrected to bool from the golden bodies
    // (GameViewController::HandleGameEvent @0x431F08 returns 1 on
    // handled paths and tail-returns GameSceneController's result;
    // the mangled name does not encode it).
    virtual bool HandleGameEvent(const GameEvent& event) = 0;
};

class IGameControlDelegate {
public:
    virtual void GameControlButtonDown(GameControlButton button) = 0;
    virtual void GameControlButtonUp(GameControlButton button) = 0;
};

#ifndef CAVER_IGAMEMENUVIEWCONTROLLERDELEGATE_DEFINED
#define CAVER_IGAMEMENUVIEWCONTROLLERDELEGATE_DEFINED
class IGameMenuViewControllerDelegate {
public:
    virtual void GameMenuViewControllerDidQuitToMenu(GameMenuViewController* controller) = 0;
    virtual void GameMenuViewControllerDismissed(GameMenuViewController* controller) = 0;
    virtual void GameMenuViewControllerDidSetGuideEnabled(GameMenuViewController* controller, bool enabled) = 0;
    virtual void GameMenuViewControllerDidSetCoinDoublerEnabled(GameMenuViewController* controller, bool enabled) = 0;
};
#endif

#ifndef CAVER_ILEVELUPVIEWCONTROLLERDELEGATE_DEFINED
#define CAVER_ILEVELUPVIEWCONTROLLERDELEGATE_DEFINED
class ILevelUpViewControllerDelegate {
public:
    virtual void LevelUpViewControllerDidFinish(LevelUpViewController* controller) = 0;
};
#endif

#ifndef CAVER_IPORTALVIEWCONTROLLERDELEGATE_DEFINED
#define CAVER_IPORTALVIEWCONTROLLERDELEGATE_DEFINED
class IPortalViewControllerDelegate {
public:
    virtual void PortalViewControllerDidGotoLevel(PortalViewController* controller, const std::string& levelName) = 0;
    virtual void PortalViewControllerDidSetGuideEnabled(PortalViewController* controller, bool enabled) = 0;
};
#endif

#ifndef CAVER_IPROFILEMANAGERDELEGATE_DEFINED
#define CAVER_IPROFILEMANAGERDELEGATE_DEFINED
class IProfileManagerDelegate {
public:
    virtual void ProfileManagerDidDownloadProfile(ProfileManager* manager, const std::string& profileName) = 0;
};
#endif

#ifndef CAVER_ISTOREVIEWCONTROLLERDELEGATE_DEFINED
#define CAVER_ISTOREVIEWCONTROLLERDELEGATE_DEFINED
class IStoreViewControllerDelegate {
public:
    virtual void StoreViewControllerDismissed(StoreViewController* controller) = 0;
    virtual void StoreViewControllerDidPurchaseProduct(StoreViewController* controller, const std::string& productIdentifier) = 0;
};
#endif

class GameViewController : public GUIViewController,
                           public IGameEventResponder,
                           public IGameControlDelegate,
                           public IGameMenuViewControllerDelegate,
                           public ILevelUpViewControllerDelegate,
                           public IPortalViewControllerDelegate,
                           public IProfileManagerDelegate,
                           public IStoreViewControllerDelegate {
public:
    GameViewController();
    virtual ~GameViewController();

    // GUIViewController overrides (primary slots 2-7, 9).
    virtual boost::shared_ptr<GUILoadingView> NewLoadingView();   // slot 2
    virtual void LoadView();                                      // slot 3
    virtual void SuspendView();                                   // slot 4
    virtual void ResumeView();                                    // slot 5
    virtual void UnloadView();                                    // slot 6
    virtual void ViewDidAppear();                                 // slot 7
    virtual void Update(float deltaTime);                         // slot 9

    // New virtuals, in golden primary-vtable order (slots 11-23).
    virtual bool HandleGameEvent(const GameEvent& event);         // slot 11
    virtual void ProfileManagerDidDownloadProfile(ProfileManager* manager, const std::string& profileName); // slot 12
    virtual void GameMenuViewControllerDidQuitToMenu(GameMenuViewController* controller); // slot 13
    virtual void GameMenuViewControllerDismissed(GameMenuViewController* controller);     // slot 14
    virtual void LevelUpViewControllerDidFinish(LevelUpViewController* controller);       // slot 15
    virtual void PortalViewControllerDidGotoLevel(PortalViewController* controller, const std::string& levelName); // slot 16
    virtual void GameMenuViewControllerDidSetGuideEnabled(GameMenuViewController* controller, bool enabled);       // slot 17
    virtual void GameMenuViewControllerDidSetCoinDoublerEnabled(GameMenuViewController* controller, bool enabled); // slot 18
    virtual void PortalViewControllerDidSetGuideEnabled(PortalViewController* controller, bool enabled);           // slot 19
    virtual void StoreViewControllerDismissed(StoreViewController* controller);           // slot 20
    virtual void StoreViewControllerDidPurchaseProduct(StoreViewController* controller, const std::string& productIdentifier); // slot 21
    virtual void GameControlButtonDown(GameControlButton button); // slot 22
    virtual void GameControlButtonUp(GameControlButton button);   // slot 23

    // Non-virtual methods (census).
    void LoadGameState();
    void SaveGameState(bool saveProfile);
    void ResetView();
    void BackgroundLoad();
    void RegisterSceneLibrary(Scene* scene);
    void CheckAndEnableNewPurchases();
    void UpdateGuideTarget(bool guideEnabled);
    void CheckRevokedPurchases();
    void ShowMenu();
    void TransitionToViewController(const boost::shared_ptr<GUIViewController>& controller, float duration, float delay, bool animate);
    void GotoCredits();
    void GotoLevel(const std::string& levelName, const std::string& portalName);
    // Return types for the next four corrected to bool from the
    // golden bodies (they return the CharacterState::Add* result /
    // constant 1); the mangled names do not encode return types.
    bool HandleDidEnterPortalGameEvent(const class DidEnterPortalGameEvent& event);
    void RemoveItemFromCharacter(const boost::shared_ptr<Item>& item);
    void ScheduleGuideTargetUpdate();
    void RemoveGuideTargetIfCompleted();
    void RemoveGuideTarget();
    bool AddItemToCharacter(const boost::shared_ptr<Item>& item);
    bool AddSkillToCharacter(const boost::shared_ptr<Skill>& skill);

    void* m_unrecovered128 = 0;                                   // +128
    boost::shared_ptr<PlayerProfile> m_profile;                   // +136
    boost::shared_ptr<void> m_unrecovered152;                     // +152
    boost::shared_ptr<GameState> m_gameState;                     // +168
    boost::shared_ptr<MapNode> m_mapNode;                         // +184 (name UNRECOVERED)
    boost::shared_ptr<class GameSceneController> m_sceneController; // +200
    boost::shared_ptr<class GameSceneView> m_sceneView;           // +216
    bool m_unrecovered232 = false;                                // +232
    bool m_unrecovered233 = false;                                // +233
    std::string m_string240;                                      // +240 (name UNRECOVERED)
    bool m_bool264 = false;                                       // +264 (name UNRECOVERED)
    boost::shared_ptr<void> m_unrecovered272;                     // +272
    bool m_bool288 = false;                                       // +288 (name UNRECOVERED)
    boost::shared_ptr<GuideTarget> m_guideTarget;                 // +296
    boost::intrusive_ptr<SceneObject> m_guideTargetObject;        // +312
    float m_guideTargetTimer = 0.0f;                              // +320
    std::string m_string328;                                      // +328 (name UNRECOVERED)
};

// Golden aarch64 NDK-libc++ layout anchor; MSVC STL member sizes differ,
// so this assert is Android/Linux-only (the Windows build is self-consistent
// under MSVC STL and does not claim golden byte-layout parity).
#ifndef _MSC_VER
static_assert(sizeof(GameViewController) == 352,
              "GameViewController layout: last member ends at +352 (ctor/dtor-proven)");
#endif

} // namespace Caver

#endif
