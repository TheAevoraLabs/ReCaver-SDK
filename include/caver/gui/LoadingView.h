// Recovered from libswordigo.so (1.4.13) evidence — IDA/Hex-Rays exports
// in ~/workspace/analysis/swordigo/functions/Caver/LoadingView/ and the
// golden vtable _ZTVN5Caver11LoadingViewE @0x631690 (40 slots).
//
// Derivation (RTTI-proven: _ZTIN5Caver11LoadingViewE is a
// __si_class_type_info whose base is _ZTIN5Caver14GUILoadingViewE):
//   LoadingView -> GUILoadingView -> GUIView.
// Vtable overrides vs the GUIView/GUILoadingView slot map:
//   slot 7/8   ~LoadingView (D2 @0x4BF1C8, D0 @0x4BF2A0)
//   slot 9     Update         @0x4BF1C0 (4-byte thunk to GUIView::Update)
//   slot 10    DrawRect       @0x4BF1C4 (4-byte thunk to GUIView::DrawRect)
//   slot 11    LayoutSubviews @0x4BF1BC (4-byte thunk to
//              GUIView::LayoutSubviews)
//   slots 38/39 AnimateIn/AnimateOut stay GUILoadingView's (inherited).
// LoadingView therefore exists to skip GUILoadingView::DrawRect: its
// own DrawRect tail-calls GUIView's, not GUILoadingView's.
//
// Layout (sizeof 304 = 0x130, proven by the ctor/dtor store maps):
//   GUILoadingView base                      0 .. 263
//   +264 shared_ptr<GUIEffectView> (created by the ctor, faded out
//        with FadeOut(0.0) and added as a subview)
//   +280 shared_ptr<GUILabel> (zeroed by the ctor, released by the
//        dtor; pointee type PROVEN by the ctor's unwind landing pad
//        at 0x4BF188, which calls
//        _ZN5boost10shared_ptrIN5Caver8GUILabelEED2Ev on +280 —
//        purpose/name UNRECOVERED: no LoadingView body ever reads
//        or writes it beyond zero/release, and neither derived
//        class touches it)
//   +296 float (= 0.0, ctor)
//   +300 float (= FLT_MAX, ctor)
//
// This header SUPERSEDES the minimal LoadingView stub in
// SceneLoadingView.h (HAND-OFF: that stub must be replaced by
// #include "LoadingView.h" before any TU includes both headers, or
// the two class definitions collide — see the LoadingView lane report).
#ifndef CAVER_LOADINGVIEW_H
#define CAVER_LOADINGVIEW_H

#include <boost/shared_ptr.hpp>

#include "GUIEffectView.h"
#include "GUILabel.h"
#include "GUILoadingView.h"

namespace Caver {

class LoadingView : public GUILoadingView {
public:
    LoadingView();
    // No user-declared destructor, deliberately: the golden D0
    // @0x4BF2A0 is a FULL CLONE of the D2 body plus operator delete
    // (224 bytes vs D2's 216), the shape clang emits for an implicit
    // destructor (cf. ProfileManagerLoadingView, whose golden D0 is
    // likewise a clone). A declared out-of-line dtor would instead
    // emit a 36-byte D0 thunk calling D2. The implicit dtor destroys
    // +280, then +264, then the GUILoadingView/GUIView base.

    void Update(float deltaTime) override;  // slot 9 (thunk)
    void DrawRect(RenderingContext* context, const Rectangle& rect,
                  const Matrix4& transform) override;  // slot 10 (thunk)
    void LayoutSubviews() override;  // slot 11 (thunk)

    boost::shared_ptr<GUIEffectView> m_effectView;  // +264
    boost::shared_ptr<GUILabel> m_field280;         // +280 (purpose
                                                    // UNRECOVERED)
    float m_field296;  // +296 = 0.0 (name UNRECOVERED)
    float m_field300;  // +300 = FLT_MAX (name UNRECOVERED)
};

} // namespace Caver

#endif
