// Recovered from libswordigo.so (1.4.13) evidence — IDA/Hex-Rays
// exports in ~/workspace/analysis/swordigo/functions/Caver/
// BadgeCountView/, the golden vtable @0x62C780 and RTTI @0x62C8C0.
//
// BadgeCountView: the little count badge (e.g. the news badge on the
// main menu). Direct GUIView subclass (RTTI @0x62C8C0 is a
// __si_class_type_info on GUIView @0x635630).
// Vtable 320 bytes = the 38 GUIView slots with only these overrides:
//   7/8  ~BadgeCountView D1/D2 (@0x454130), D0 (@0x454208)
//   11   LayoutSubviews (@0x453EF4)
// All other slots keep the GUIView/GUIResponder functions; the class
// adds NO new virtual slots.
//
// Layout (sizeof = 288 = 0x120, proven by the operator new(0x120)
// for the badge in MainMenuView::MainMenuView and by the D2
// teardown order, which releases the members in exact reverse
// declaration order):
//   +252 int count — inside the GUIView base tail (it overlaps the
//        top byte of the unaligned colour dword at +249 and the
//        pad at +253), exactly as in ChooseAgeView (age at +252)
//        and GUIScrollView (contentBounds at +252). There is no
//        room for a named member there, so the .cpp accesses it
//        by raw offset; the ctor zeroes it with the same stores
//        that zero the two shared_ptrs below.
//   +256 boost::shared_ptr<GUIFrameView> frameView — the badge
//        plate, frame texture "countbadge", hidden until the
//        count is non-zero (SetCount drives its hidden flag)
//   +272 boost::shared_ptr<GUILabel> label — the count text,
//        NormalLabel("", white, black shadow), shadow disabled
//        in the ctor (field352 = false)
// Member names are UNRECOVERED; the roles above are proven by the
// ctor / SetCount / LayoutSubviews bodies cited in the .cpp.
#ifndef CAVER_BADGECOUNTVIEW_H
#define CAVER_BADGECOUNTVIEW_H

#include <boost/shared_ptr.hpp>

#include "GUIView.h"

namespace Caver {

class GUIFrameView;
class GUILabel;

class BadgeCountView : public GUIView {
public:
    BadgeCountView();
    // No user-declared destructor: the golden D0 (@0x454208, 224B)
    // is the implicit-dtor full clone of the D2 body plus
    // operator delete (the CreditsView/NotificationView pattern),
    // not the 36B call-D1 thunk a declared dtor produces. The
    // implicit dtor is virtual via the GUIView base and fills
    // slots 7/8.

    void LayoutSubviews() override;  // slot 11

    // Non-virtual member.
    void SetCount(int count);

    boost::shared_ptr<GUIFrameView> m_frameView;  // +256
    boost::shared_ptr<GUILabel> m_label;          // +272
};

// Golden aarch64 NDK-libc++ layout anchor; MSVC STL member sizes differ,
// so this assert is Android/Linux-only (the Windows build is self-consistent
// under MSVC STL and does not claim golden byte-layout parity).
#ifndef _MSC_VER
static_assert(sizeof(BadgeCountView) == 288,
              "BadgeCountView layout: operator new(0x120) in "
              "MainMenuView::MainMenuView, golden D2 teardown");
#endif

} // namespace Caver

#endif
