// Recovered from libswordigo.so (1.4.13) evidence — IDA/Hex-Rays
// exports in ~/workspace/analysis/swordigo/functions/Caver/
// ProfileManagerLoadingView/ and the operator new(0x130) in
// ProfileManagerLoadingViewController::LoadView @0x46BA1C.
//
// ProfileManagerLoadingView — the "Downloading saves from iCloud..."
// modal view shown while the profile manager syncs. It is NOT a
// LoadingView: the ctor calls GUIView::GUIView directly, the dtor
// releases only its two own shared_ptrs and then calls
// GUIView::~GUIView directly, and its first own member sits at +256
// (a LoadingView base would end at +304 and put its members' control
// blocks at +272/+288, not the +264/+280 the dtor releases).
// The task brief's LoadingView hypothesis is therefore rejected by
// the evidence; this class derives from GUIView directly.
//
// Vtable: GUIView's 38 slots with overrides at
//   7/8  ~ProfileManagerLoadingView (D2 @0x46B824, D0 @0x46B8FC)
//   9    Update (@0x46B624)
//   11   LayoutSubviews (@0x46B4E4)
// No DrawRect / AnimateIn / AnimateOut override.
//
// Layout (sizeof 304 = 0x130, from the LoadView operator new and the
// ctor/dtor store maps):
//   +0..255  GUIView base
//   +256     boost::shared_ptr<GUILabel>  (NormalLabel("", white,
//            black), then setText("Downloading saves from iCloud..."))
//   +272     boost::shared_ptr<GUIButton> (FramedButtonWithTitle(
//            "Skip", false); tint 0xCCCCCCCC, text white; starts
//            hidden, fades in after 10 s in Update)
//   +288     int   dot counter (ctor 0; Update cycles 1,2,3,4,1,...)
//   +292     float dot timer (ctor FLT_MAX, so the first Update
//            fires the dot cycle immediately, then resets to 0)
//   +296     float button timer (ctor 0; the fade-in fires above 10.0)
//   +300     padding to 304
#ifndef CAVER_PROFILEMANAGERLOADINGVIEW_H
#define CAVER_PROFILEMANAGERLOADINGVIEW_H

#include <boost/shared_ptr.hpp>

#include "GUIView.h"

namespace Caver {

class GUIButton;
class GUILabel;

class ProfileManagerLoadingView : public GUIView {
public:
    ProfileManagerLoadingView();
    // No user-declared destructor: the golden D0 @0x46B8FC (224
    // bytes) is a full clone of the D2 body (216 bytes) plus the
    // operator delete tail, the shape clang emits for an implicit
    // destructor (cf. ProfileSelectionViewController) — NOT the
    // 36-byte call-D2 thunk a declared destructor would emit.

    void Update(float deltaTime) override;   // slot 9
    void LayoutSubviews() override;          // slot 11

    boost::shared_ptr<GUILabel> m_label;    // +256 (name UNRECOVERED)
    boost::shared_ptr<GUIButton> m_button;  // +272 (name UNRECOVERED)
    int m_dotCount;                         // +288 (name UNRECOVERED)
    float m_dotTimer;                       // +292 (name UNRECOVERED)
    float m_buttonTimer;                    // +296 (name UNRECOVERED)
};

// Golden aarch64 NDK-libc++ layout anchor; MSVC STL member sizes differ,
// so this assert is Android/Linux-only (the Windows build is self-consistent
// under MSVC STL and does not claim golden byte-layout parity).
#ifndef _MSC_VER
static_assert(sizeof(ProfileManagerLoadingView) == 304,
              "ProfileManagerLoadingView layout: LoadView new(0x130)");
#endif

} // namespace Caver

#endif
