// Recovered from libswordigo.so (1.4.13) evidence — IDA/Hex-Rays
// exports in ~/workspace/analysis/swordigo/functions/Caver/
// ProfilesHeaderView/ and the golden disassembly of the ctor /
// LayoutSubviews / dtor.
//
// ProfilesHeaderView: the header strip of the profile-selection
// ("Saved Games") screen — a GUIFrameView holding the "Saved Games"
// title label and the arrow close button. ProfileSelectionView
// binds ButtonPressed to the close button; the button itself has
// no target action installed by this ctor.
//
// Base: GUIFrameView (288 bytes).
//
// Layout (sizeof 320; member order proven by the D2 teardown
// sequence @0x4696F0, which releases the members in exact reverse
// order, and by the ctor @0x468EC4 store map):
//   +288 shared_ptr<GUILabel>  m_titleLabel  ("Saved Games")
//   +304 shared_ptr<GUIButton> m_closeButton (arrow IconButton)
//
// Primary vtable overrides proven from the golden: D1/D2/D0
// (slots 7/8) and LayoutSubviews (slot 11). Update, DrawRect and
// every other slot keep the GUIFrameView/GUIView functions.
#ifndef CAVER_PROFILESHEADERVIEW_H
#define CAVER_PROFILESHEADERVIEW_H

#include <boost/shared_ptr.hpp>

#include "GUIFrameView.h"

namespace Caver {

class GUIButton;
class GUILabel;

class ProfilesHeaderView : public GUIFrameView {
public:
    ProfilesHeaderView();
    ~ProfilesHeaderView() override;  // primary slots 7, 8

    void LayoutSubviews() override;  // slot 11

    boost::shared_ptr<GUILabel> m_titleLabel;    // +288
    boost::shared_ptr<GUIButton> m_closeButton;  // +304
};

} // namespace Caver

#endif
