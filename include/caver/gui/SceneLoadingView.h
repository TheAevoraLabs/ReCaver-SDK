// Recovered from libswordigo.so (1.4.13) evidence — IDA/Hex-Rays exports
// in ~/workspace/analysis/swordigo/functions/Caver/SceneLoadingView/,
// the golden vtable _ZTVN5Caver16SceneLoadingViewE @0x632530 (336 bytes,
// 40 slots) and its RTTI chain.
//
// Derivation (RTTI-proven by the ABS64 relocations of the typeinfos):
//   SceneLoadingView -> LoadingView -> GUILoadingView -> GUIView.
// _ZTIN5Caver16SceneLoadingViewE @0x632680 is a __si_class_type_info
// whose base is _ZTIN5Caver11LoadingViewE, whose base is in turn
// _ZTIN5Caver14GUILoadingViewE.
//
// Vtable overrides vs the GUIView/GUILoadingView slot map:
//   slot 7/8   ~SceneLoadingView (D2 @0x4D147C WEAK, D0 @0x4D1804)
//   slot 9     Update         @0x4D0F48
//   slot 10    DrawRect       @0x4D131C
//   slot 11    LayoutSubviews @0x4D0D70
//   slot 38    AnimateIn      @0x4D1368 (returns shared_ptr<GUIAnimation>)
//   slot 39    AnimateOut stays GUILoadingView::AnimateOut (inherited)
// No new virtuals are added.
//
// Layout (sizeof 0x1C8 = 456, proven by the operator new(0x1C8) in
// GameViewController::NewLoadingView @0x438BD0 and the ctor/dtor
// store/release maps):
//   LoadingView base                       0 .. 303
//   +304 shared_ptr<GameState>  (InitWithGameState argument, kept)
//   +320 shared_ptr<MapView>    (created by InitWithGameState)
//   +336 shared_ptr<MapNode>    (the InitWithGameState node argument)
//   +352 shared_ptr<MapNode>    (the game state's current node)
//   +368 shared_ptr<StripeView> (created by the ctor; bottom stripe)
//   +384 shared_ptr<StripeView> (created by the ctor; top/title stripe)
//   +400 shared_ptr<GUILabel>   (created by the ctor via NormalLabel)
//   +416 shared_ptr<GUILabel>   (created by the ctor; starts hidden)
//   +432 float   elapsed timer (crossfade fires above 0.2)
//   +436 bool    (set when the crossfade has fired / single-title mode;
//                 name UNRECOVERED)
//   +437 bool    (set when the two labels' texts differ; selects the
//                 label the dot animation appends to; name UNRECOVERED)
//   +440 float   dot-animation timer (ctor initialises to 0.1)
//   +444 int     dot counter (caps at 3 dots)
//   +448 bool    (set when +336/+352 name different nodes; raises the
//                 dot threshold in Update from 0.0 to 0.2; UNRECOVERED)
//
// HAND-OFF (LoadingView lane): no LoadingView header exists in the tree
// yet, so the base is declared minimally below, to exactly the extent
// the golden bodies prove (its own ctor @0x4BEFDC, dtor @0x4BF1C8 and
// the 4-byte Update/LayoutSubviews/DrawRect thunks @0x4BF1C0/C4/BC,
// which simply tail-call the GUIView versions — LoadingView exists to
// skip GUILoadingView::DrawRect). Layout: GUILoadingView (264) +
// shared_ptr<GUIEffectView> @+264 (created by its ctor, faded out and
// added as a subview), an unidentified shared_ptr @+280 (zeroed by its
// ctor, released by its dtor; pointee UNRECOVERED), float @+296
// (= 0.0) and float @+300 (= FLT_MAX) — sizeof 304. Replace this stub
// with the real header when the LoadingView slice lands.
#ifndef CAVER_SCENELOADINGVIEW_H
#define CAVER_SCENELOADINGVIEW_H

#include <boost/shared_ptr.hpp>

#include "GUILoadingView.h"

namespace Caver {

class GameState;
class GUIEffectView;
class GUILabel;
class MapNode;
class MapView;
class StripeView;

class LoadingView : public GUILoadingView {
public:
    LoadingView();
    // Defined inline here (weak in the golden, and inlined into
    // ~SceneLoadingView's body there: vptr store, the +280 then +264
    // shared_ptr releases, then ~GUIView) purely so the derived
    // destructor reproduces that shape; the LoadingView slice's real
    // header replaces this stub wholesale.
    ~LoadingView() override {}

    void Update(float deltaTime) override;        // slot 9 (thunk)
    void DrawRect(RenderingContext* context, const Rectangle& rect,
                  const Matrix4& transform) override;  // slot 10 (thunk)
    void LayoutSubviews() override;                    // slot 11 (thunk)

    boost::shared_ptr<GUIEffectView> m_effectView;  // +264
    boost::shared_ptr<GUIView> m_field280;          // +280 (pointee
                                                    // UNRECOVERED)
    float m_field296;                               // +296 = 0.0
    float m_field300;                               // +300 = FLT_MAX
};

class SceneLoadingView : public LoadingView {
public:
    SceneLoadingView();
    // Weak in the golden and pure member teardown (the eight own
    // shared_ptrs in reverse order, then the inlined LoadingView
    // teardown, then ~GUIView) — defined in-class so it emits weak
    // and the base teardown inlines the way the golden's does.
    ~SceneLoadingView() override {}  // slots 7, 8

    void InitWithGameState(const boost::shared_ptr<GameState>& gameState,
                           const boost::shared_ptr<MapNode>& node);

    void Update(float deltaTime) override;        // slot 9
    void DrawRect(RenderingContext* context, const Rectangle& rect,
                  const Matrix4& transform) override;  // slot 10
    void LayoutSubviews() override;                    // slot 11

    // Slot 38. Returns GUILoadingView::AnimateIn's animation, unless a
    // map view and both nodes are present, in which case it returns
    // MapView::AnimateLocationMarkerFromNodeToNode's animation instead.
    boost::shared_ptr<GUIAnimation> AnimateIn() override;

    boost::shared_ptr<GameState> m_gameState;    // +304
    boost::shared_ptr<MapView> m_mapView;        // +320
    boost::shared_ptr<MapNode> m_targetNode;     // +336
    boost::shared_ptr<MapNode> m_currentNode;    // +352
    boost::shared_ptr<StripeView> m_stripe1;     // +368
    boost::shared_ptr<StripeView> m_stripe2;     // +384
    boost::shared_ptr<GUILabel> m_label1;        // +400
    boost::shared_ptr<GUILabel> m_label2;        // +416
    float m_elapsed = 0.0f;                      // +432
    bool m_field436 = false;                     // +436
    bool m_field437 = false;                     // +437
    float m_dotTimer = 0.1f;                     // +440
    int m_dotCount = 0;                          // +444
    bool m_field448 = false;                     // +448
};

} // namespace Caver

#endif
