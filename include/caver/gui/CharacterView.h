// Recovered from libswordigo.so (1.4.13) evidence — IDA/Hex-Rays
// exports in ~/workspace/analysis/swordigo/functions/Caver/CharacterView/,
// the golden vtable _ZTVN5Caver13CharacterViewE @0x630080 (address
// point 0x630090) and the RTTI _ZTIN5Caver13CharacterViewE @0x630210
// (__vmi_class_type_info, 2 bases).
//
// CharacterView: the CHARACTER page of the tabbed menu — a close
// (arrow) button, a "Reset" button for attribute points, an
// experience bar, an attributes header frame ("Attributes" title,
// "Points Available:" title + value labels), three
// AttributePointsViews (health / attack / magic) and the Iselon
// shard frame built by CreateIselonView.
//
// Bases (RTTI order, confirmed by the ctor @0x4A5AAC vptr stores and
// the _ZThn280_ thunk to AttributePointsViewValueChanged):
//   MenuViewPage @0 (so the page-title string sits at +256 and
//   Dismiss is slot 38, exactly as in InventoryView.h — the base is
//   modelled the same way, by including that header),
//   IAttributePointsViewDelegate @280 (secondary vtable @0x6301E0
//   with exactly one slot, the thunk above; the interface RTTI
//   _ZTIN5Caver28IAttributePointsViewDelegateE @0x630238 is a plain
//   __class_type_info).
//
// Primary vtable: the 38 GUIView slots (Update slot 9, DrawRect
// slot 10 and LayoutSubviews slot 11 overridden), Dismiss at slot 38,
// AttributePointsViewValueChanged appended at slot 39.
//
// Layout (sizeof 520, byte-verified against the ctor @0x4A5AAC
// zero-store map and the dtor @0x4A9EC4 teardown order — exact
// reverse declaration order):
//   +0..255  GUIView base (inside MenuViewPage)
//   +256     std::string page title (MenuViewPage member; the ctor
//            assigns "CHARACTER")
//   +280     IAttributePointsViewDelegate subobject (vptr only)
//   +288     ICharacterViewDelegate* delegate (raw, non-owning;
//            Dismiss calls its slot 0, CharacterViewDismissed, then
//            clears it; GameMenuViewController implements that
//            interface). NOT written by the ctor — the golden ctor's
//            zero map skips +288, so it stays uninitialised until
//            the presenting controller stores it (bug-for-bug).
//   +296     boost::shared_ptr<GameState> gameState
//   +312     boost::shared_ptr<GUIButton> close button (the arrow
//            IconButton built by InitWithGameState; ButtonPressed
//            compares the sender against it and Dismiss()es)
//   +328     boost::shared_ptr<GUIButton> "Reset" framed button
//            (ButtonPressed resets the three AttributePointsViews
//            to their initial points when it is the sender)
//   +344     boost::shared_ptr<ExperienceBar> experience bar
//   +360     boost::shared_ptr<GUIFrameView> attributes header frame
//            ("ui_frame_dark"; built by CreateAttributesHeaderView)
//   +376     boost::shared_ptr<AttributePointsView> health points
//   +392     boost::shared_ptr<AttributePointsView> attack points
//   +408     boost::shared_ptr<AttributePointsView> magic points
//   +424     int attribute-points-view count (InitWithGameState
//            stores 3; member name UNRECOVERED). NOT written by the
//            ctor either — the golden zero map skips +424.
//   +432     boost::scoped_array<boost::shared_ptr<GUIView> >
//            the three AttributePointsViews, as GUIViews
//   +440     bool level-up mode (SetLevelUpMode sets it;
//            UpdateAvailablePoints / LayoutSubviews branch on it;
//            member name UNRECOVERED)
//   +444     int available attribute points (SetLevelUpMode stores
//            1; member name UNRECOVERED)
//   +448     int spent attribute points (recomputed by
//            UpdateAvailablePoints; member name UNRECOVERED)
//   +456     boost::shared_ptr<GUILabel> label built by
//            GameInterfaceBuilder::NormalLabel in InitWithGameState
//            (member name UNRECOVERED; LayoutSubviews frames it
//            against its font height)
//   +472     boost::shared_ptr<GUILabel> "Points Available:" title
//            label (hidden until SetLevelUpMode)
//   +488     boost::shared_ptr<GUILabel> available-points value
//            label (UpdateAvailablePoints writes the remaining
//            points into it; hidden until SetLevelUpMode)
//   +504     boost::shared_ptr<GUIFrameView> Iselon frame view
//            ("ui_menu_stripe"; built by CreateIselonView, hidden
//            in level-up mode)
#ifndef CAVER_CHARACTERVIEW_H
#define CAVER_CHARACTERVIEW_H

#include <boost/scoped_array.hpp>
#include <boost/shared_ptr.hpp>

#include "MenuViewPage.h"

namespace Caver {

class AttributePointsView;
class CharacterView;
class ExperienceBar;
class GameState;
class GUIButton;
class GUIEvent;
class GUIFrameView;
class GUILabel;
class RenderingContext;

// Identical declaration to GameMenuViewController.h (that slice
// owns the implementation side); only slot 0 is called from here.
class ICharacterViewDelegate {
public:
    virtual void CharacterViewDismissed(CharacterView* view) = 0;  // slot 0
};

// The AttributePointsView delegate interface (RTTI-proven name):
// exactly one method, no destructor in the golden secondary vtable.
#ifndef CAVER_IATTRIBUTEPOINTSVIEWDELEGATE_DEFINED
#define CAVER_IATTRIBUTEPOINTSVIEWDELEGATE_DEFINED
class IAttributePointsViewDelegate {
public:
    virtual void AttributePointsViewValueChanged(AttributePointsView* view) = 0;  // slot 0
};
#endif

class CharacterView : public MenuViewPage,
                      public IAttributePointsViewDelegate {
public:
    CharacterView();
    ~CharacterView() override;  // slots 7, 8

    void InitWithGameState(const boost::shared_ptr<GameState>& gameState);

    void ButtonPressed(void* sender, GUIEvent* event);
    void CreateAttributesHeaderView();
    void CreateIselonView();
    void UpdateAvailablePoints();
    void SetLevelUpMode();
    void AddAnimations(bool dismiss);

    void Update(float deltaTime) override;            // slot 9
    void DrawRect(RenderingContext* context, const Rectangle& rect,
                  const Matrix4& transform) override;  // slot 10
    void LayoutSubviews() override;                    // slot 11
    void Dismiss() override;                           // slot 38

    // IAttributePointsViewDelegate implementation (primary slot 39,
    // secondary-vtable thunk _ZThn280_).
    void AttributePointsViewValueChanged(AttributePointsView* view) override;  // slot 39

    // (pageTitle string is the MenuViewPage member at +256; the
    // IAttributePointsViewDelegate subobject occupies +280.)
    ICharacterViewDelegate* delegate;                 // +288 (see note)
    boost::shared_ptr<GameState> gameState;           // +296
    boost::shared_ptr<GUIButton> closeButton;         // +312
    boost::shared_ptr<GUIButton> resetButton;         // +328
    boost::shared_ptr<ExperienceBar> experienceBar;   // +344
    boost::shared_ptr<GUIFrameView> attributesFrameView;  // +360
    boost::shared_ptr<AttributePointsView> healthPointsView;  // +376
    boost::shared_ptr<AttributePointsView> attackPointsView;  // +392
    boost::shared_ptr<AttributePointsView> magicPointsView;   // +408
    int attributePointsViewCount;                     // +424 (name UNRECOVERED)
    boost::scoped_array<boost::shared_ptr<GUIView> > attributePointsViews;  // +432
    bool levelUpMode;                                 // +440 (name UNRECOVERED)
    int availablePoints;                              // +444 (name UNRECOVERED)
    int spentPoints;                                  // +448 (name UNRECOVERED)
    boost::shared_ptr<GUILabel> label456;             // +456 (name UNRECOVERED)
    boost::shared_ptr<GUILabel> pointsTitleLabel;     // +472
    boost::shared_ptr<GUILabel> pointsValueLabel;     // +488
    boost::shared_ptr<GUIFrameView> iselonView;       // +504
};

// Golden aarch64 NDK-libc++ layout anchor; MSVC STL member sizes differ,
// so this assert is Android/Linux-only (the Windows build is self-consistent
// under MSVC STL and does not claim golden byte-layout parity).
#ifndef _MSC_VER
static_assert(sizeof(CharacterView) == 520,
              "CharacterView layout: ctor/dtor store maps, golden");
#endif

} // namespace Caver

#endif
