// Recovered from libswordigo.so (1.4.13) evidence — IDA/Hex-Rays
// exports in ~/workspace/analysis/swordigo/functions/Caver/TabView/,
// the golden vtable _ZTVN5Caver7TabViewE @0x632D00 (address point
// 0x632D10, 38 GUIView slots; overrides: slot 0 TouchBegan, slots
// 7/8 the destructor pair, slot 9 Update, slot 10 DrawRect, slot 11
// LayoutSubviews — no new slots), RTTI @0x632E40 (single base
// GUIView), the ctor @0x4DA060 store map and the dtor @0x4DAA28
// teardown order (reverse declaration order).
//
// TabView: the vertical tab strip inside TabbedMenuView's tab
// frame. It draws one background tab per label (the selected tab
// uses the "ui_tab_selected" texture, the others "ui_tab"), plus a
// per-tab icon texture named "ui_tab_" + label, into an embedded
// Sprite; tapping selects the nearest tab centre, plays the "click"
// sound and notifies the delegate.
//
// Layout (sizeof 560 = 0x230, operator new in
// TabbedMenuView::TabbedMenuView):
//   +0..255 GUIView base
//   +256    ITabViewDelegate* delegate (raw, non-owning;
//           TabbedMenuView's ctor stores its own +256 subobject
//           here; TouchBegan calls its slot 0)
//   +264    boost::intrusive_ptr<Texture> tabTexture
//           ("ui_tab"; its sub-rect height at Texture+120 is the
//           tab height every layout formula uses)
//   +272    boost::intrusive_ptr<Texture> selectedTabTexture
//           ("ui_tab_selected")
//   +280    GUIRoundedRect roundedRect (168 bytes; member name
//           UNRECOVERED — constructed by the ctor, never otherwise
//           touched by any TabView function in the golden)
//   +448    Sprite sprite (88 bytes; Init'ed by the ctor with the
//           tab texture, 4 faces / 8 vertices / colour stream)
//   +536    int selectedTabIndex (ctor stores -1)
//   +544    FastVector<std::string> tabs (count +544, capacity
//           +548, items +552 — proven by AddTab @0x4DA47C)
// Member NAMES are behaviour-derived and UNRECOVERED, except
// selectedTabIndex / tabs, whose shapes TabbedMenuView's recovered
// code already uses.
#ifndef CAVER_TABVIEW_H
#define CAVER_TABVIEW_H

#include <boost/intrusive_ptr.hpp>
#include <string>

#include "../render/Sprite.h"
#include "../scene/FastVector.h"
#include "GUIRoundedRect.h"
#include "GUIView.h"
#include "TabbedMenuView.h"

namespace Caver {

class RenderingContext;
class Texture;

class TabView : public GUIView {
public:
    TabView();
    ~TabView() override;  // slots 7, 8

    // GUIView overrides.
    void TouchBegan(const FWTouch& touch) override;  // slot 0
    void Update(float deltaTime) override;           // slot 9
    void DrawRect(RenderingContext* context, const Rectangle& rect,
                  const Matrix4& transform) override;  // slot 10
    void LayoutSubviews() override;                    // slot 11

    float TabHeight();
    float BottomForTabAtIndex(int index);
    float CenterForTabAtIndex(int index);
    void AddTab(const std::string& name);
    void UpdateSprite();
    void SetSelectedTabIndex(int index);

    ITabViewDelegate* tabDelegate;                        // +256
    boost::intrusive_ptr<Texture> tabTexture;             // +264
    boost::intrusive_ptr<Texture> selectedTabTexture;     // +272
    GUIRoundedRect roundedRect;                           // +280
    Sprite sprite;                                        // +448
    int selectedTabIndex;                                 // +536
    FastVector<std::string> tabs;                         // +544
};

} // namespace Caver

#endif
