// Recovered from libswordigo.so (1.4.13) evidence — IDA/Hex-Rays
// exports in ~/workspace/analysis/swordigo/functions/Caver/
// MapPopupView/ plus golden disassembly for the constants the
// decompiler garbles (the SetBounds rectangle (0, 0, 340, 220) read
// from the golden .rodata @0x25A040, the Sprite::Init(2, 4, false)
// arguments and the -1 colour dword, both read from the instruction
// stream).
//
// MapPopupView: the small map popup shown over the map (the sibling
// of ItemInfoPopupView, which shares the same background-sprite
// idiom: "ui_notification_background" texture, one rectangle covering
// the view bounds, white colour). It is a DIRECT GUIView subclass —
// the base check was decided by RTTI shape and the ctor body: the
// ctor calls only GUIView::GUIView, stores the MapPopupView vtable,
// and touches no member other than the single shared_ptr at +256;
// the D2 releases exactly that one shared_ptr before ~GUIView. The
// task brief's "PortalView is the parent" is context only (PortalView
// owns the MapView this popup sits over); MapPopupView does NOT
// derive from PortalView.
//
// The vtable is exactly the 38 GUIView slots, overriding only the
// destructor (slots 7/8), Update (slot 9, a tail branch to
// GUIView::Update), DrawRect (slot 10) and LayoutSubviews (slot 11,
// an empty body). DrawRect's first call is the virtual slot-37
// SetOrthoProjection, i.e. GUIView's implementation (no
// MapPopupView SetOrthoProjection symbol exists in the census).
//
// Layout (sizeof 272 = 0x110, byte-verified against the ctor
// @0x4C1670 store map — the shared_ptr zeroed at +256 — and the
// dtor @0x4C191C teardown, which releases only +256 before
// ~GUIView):
//   +0..255  GUIView base
//   +256     boost::shared_ptr<Sprite> backgroundSprite (ctor: the
//            "ui_notification_background" texture, Init(2, 4, false),
//            one rectangle covering the view bounds, colour -1)
// Member NAME is behaviour-derived and UNRECOVERED.
#ifndef CAVER_MAPPOPUPVIEW_H
#define CAVER_MAPPOPUPVIEW_H

#include <boost/shared_ptr.hpp>

#include "GUIView.h"

namespace Caver {

class RenderingContext;
class Sprite;

class MapPopupView : public GUIView {
public:
    MapPopupView();
    // NOTE: the destructor is HEADER-INLINE in the original — proven
    // by the golden D0 @0x4C19A0, which duplicates the full D2 body
    // (140B) instead of wrapping D2 (36B, what an out-of-line dtor
    // compiles to with this toolchain; verified experimentally).
    ~MapPopupView() {}  // slots 7, 8

    void Update(float deltaTime) override;  // slot 9
    void DrawRect(RenderingContext* context, const Rectangle& rect,
                  const Matrix4& transform) override;  // slot 10
    void LayoutSubviews() override;                    // slot 11

    boost::shared_ptr<Sprite> backgroundSprite;  // +256
};

// Golden aarch64 NDK-libc++ layout anchor; MSVC STL member sizes differ,
// so this assert is Android/Linux-only (the Windows build is self-consistent
// under MSVC STL and does not claim golden byte-layout parity).
#ifndef _MSC_VER
static_assert(sizeof(MapPopupView) == 272,
              "MapPopupView layout: ctor/dtor store maps, golden");
#endif

} // namespace Caver

#endif
