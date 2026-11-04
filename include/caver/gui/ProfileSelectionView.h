// Recovered from libswordigo.so (1.4.13) evidence — IDA/Hex-Rays
// exports in ~/workspace/analysis/swordigo/functions/Caver/
// ProfileSelectionView/, the golden vtable relocations
// (_ZTVN5Caver20ProfileSelectionViewE @0x62D3C0, secondary vtable
// @0x62D510) and the ctor / dtor / LoadProfiles bodies.
//
// ProfileSelectionView: the "Saved Games" screen — a GUIView hosting
// a GUIScrollView whose content frame holds one ProfilePanelView per
// profile (plus a synthetic fresh profile appended by LoadProfiles)
// and a ProfilesHeaderView with the close button.
//
// Bases (RTTI @0x62D540 is a __vmi_class_type_info; the ctor stores
// the primary vptr at +0 and the secondary vptr at +256):
//   +0   GUIView (256 bytes)
//   +256 IProfilePanelDelegate — the interface declared in
//        ProfilePanelView.h. NOTE: the golden typeinfo name string
//        for this interface is "Caver::IProfilePanelViewDelegate"
//        (_ZTSN5Caver25IProfilePanelViewDelegateE, referenced from
//        its typeinfo at 0x62D530) — ProfilePanelView.h's
//        "IProfilePanelDelegate" spelling is a guess and should be
//        renamed at merge; the slot shape (0 = DidStart,
//        1 = DidDeleteProfile) is identical either way.
//
// Layout (sizeof 320; member order proven by the D2 teardown
// sequence @0x465C3C — header shared_ptr, panels array, scroll
// shared_ptr — and by the ctor @0x464654 zero-store map, which
// notably does NOT initialise +288):
//   +264 IProfileSelectionViewDelegate* m_delegate (raw; the view's
//        owner — ProfileSelectionViewController implements it)
//   +272 shared_ptr<GUIScrollView> m_scrollView
//   +288 int m_panelCount (name UNRECOVERED; written only by
//        LoadProfiles, never by the ctor)
//   +296 boost::scoped_array<shared_ptr<ProfilePanelView>> m_panels
//        (the array is a new[] allocation: LoadProfiles and the
//        dtor read the element count from the new[] cookie at
//        array-8, not from +288)
//   +304 shared_ptr<ProfilesHeaderView> m_headerView
//
// Primary vtable overrides proven from the golden relocations:
// D1/D2/D0 (slots 7/8), HandleKeyboardEvent (slot 5), Update
// (slot 9, a bare tail branch to GUIView::Update), DrawRect
// (slot 10, ditto), LayoutSubviews (slot 11), DidMoveToWindow
// (slot 16), CanBecomeFirstResponder (slot 24). The secondary
// vtable holds exactly the two _ZThn256 thunks to the
// IProfilePanelDelegate overrides.
#ifndef CAVER_PROFILESELECTIONVIEW_H
#define CAVER_PROFILESELECTIONVIEW_H

#include <boost/scoped_array.hpp>
#include <boost/shared_ptr.hpp>

#include "GUIView.h"
#include "ProfilePanelView.h"

namespace Caver {

class GUIEvent;
class GUIScrollView;
class ProfilesHeaderView;
class ProfileSelectionView;

// Delegate notified by this view (raw pointer at +264). Slot order
// and signatures proven by the implementor,
// ProfileSelectionViewController (its mangled methods
// ProfileSelectionViewPlayerDidStart(ProfileSelectionView*,
// shared_ptr<PlayerProfile> const&) and
// ProfileSelectionViewCloseButtonPressed(ProfileSelectionView*)),
// and by the call sites: DidStart forwards through slot 0,
// ButtonPressed and HandleKeyboardEvent through slot 1.
// Interface name UNRECOVERED.
class IProfileSelectionViewDelegate {
public:
    virtual void ProfileSelectionViewPlayerDidStart(
        ProfileSelectionView* view,
        const boost::shared_ptr<PlayerProfile>& profile) = 0;  // slot 0
    virtual void ProfileSelectionViewCloseButtonPressed(
        ProfileSelectionView* view) = 0;                       // slot 1
};

class ProfileSelectionView : public GUIView,
                             public IProfilePanelDelegate {
public:
    ProfileSelectionView();
    ~ProfileSelectionView() override;  // primary slots 7, 8

    void HandleKeyboardEvent(const GUIKeyboardEvent& event) override;  // slot 5
    void Update(float deltaTime) override;                             // slot 9
    void DrawRect(RenderingContext* context, const Rectangle& rect,
                  const Matrix4& transform) override;                  // slot 10
    void LayoutSubviews() override;                                    // slot 11
    void DidMoveToWindow() override;                                  // slot 16
    bool CanBecomeFirstResponder() override;                           // slot 24

    // IProfilePanelDelegate (secondary vtable at +256).
    void ProfilePanelViewDidStart(ProfilePanelView* panel) override;
    void ProfilePanelDidDeleteProfile(ProfilePanelView* panel) override;

    // Non-virtual members.
    void LoadProfiles();
    void AnimateIn();
    void AnimateOut();
    void ButtonPressed(void* sender, GUIEvent* event);

    IProfileSelectionViewDelegate* m_delegate;             // +264 (raw)
    boost::shared_ptr<GUIScrollView> m_scrollView;         // +272
    int m_panelCount;                                      // +288 (name UNRECOVERED)
    boost::scoped_array<boost::shared_ptr<ProfilePanelView> >
        m_panels;                                          // +296
    boost::shared_ptr<ProfilesHeaderView> m_headerView;    // +304
};

} // namespace Caver

#endif
