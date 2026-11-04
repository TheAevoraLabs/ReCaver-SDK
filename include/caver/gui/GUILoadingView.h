// Recovered from libswordigo.so (1.4.13) evidence.
//
// GUILoadingView: a full-screen loading fade view. Vtable 336 bytes
// @0x634DF8 — the 38 GUIView slots plus two of its own:
//   slot 38 (+304) AnimateIn, slot 39 (+312) AnimateOut
// Only slot 8 (the deleting destructor, D0 @0x521520) and slot 10
// (DrawRect) are overridden among the GUIView slots; slot 7 notably
// points at GUIView's own D2 — this class declares NO destructor in the
// original (its D0 is the implicit one: ~GUIView plus operator delete,
// and the compiler aliased the complete-object destructor to the base).
//
// Layout: GUIView (256 bytes), then
//   +256 float animateInDuration = 0.4 (read by AnimateIn)
//   +260 float animateOutDuration = 0.4 (read by AnimateOut)
//
// CORRECTION (proven 2026-10-23 by the navctl upgrade worker, applied
// here): AnimateIn/AnimateOut do NOT return void. Their golden bodies
// (0x520ED8 / 0x5211B4) take a hidden sret pointer in x8, construct a
// boost::shared_ptr<GUIAnimation> directly into it (operator new(0xC0)
// + GUIAnimation ctor + sp_counted_impl_p<GUIAnimation>), pass that
// same sret shared_ptr to GUIView::AddAnimation, and return it — i.e.
// both return boost::shared_ptr<GUIAnimation> by value (the return
// type is not encoded in their mangled names). The vtable relocs at
// 0x634DF8 slots 38/39 confirm the slot assignment.
// The ctor also writes the GUIView tail: fill colour dword at +249 =
// 0xFF000000 (opaque black), and clears the byte at +245.
// DrawRect fills bounds (+148) with the +249 colour while the byte at
// +252 is set.
#ifndef CAVER_GUILOADINGVIEW_H
#define CAVER_GUILOADINGVIEW_H

#include "GUIView.h"

namespace Caver {

class GUILoadingView : public GUIView {
public:
    GUILoadingView();

    void DrawRect(RenderingContext* context, const Rectangle& rect,
                  const Matrix4& transform) override;  // slot 10

    // Golden slots 38/39 (the GUIView stub's shorter vtable places them
    // earlier in this build — function parity is unaffected). Both
    // return the animation they build and add to this view.
    virtual boost::shared_ptr<GUIAnimation> AnimateIn();
    virtual boost::shared_ptr<GUIAnimation> AnimateOut();

    union {
        float animateInDuration;   // +256
        float m_duration;
    };
    union {
        float animateOutDuration;  // +260
        float m_delay;
    };
};

} // namespace Caver

#endif
