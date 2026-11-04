// Recovered from libswordigo.so (1.4.13) evidence.
//
// GUIEffectView: a GUIView hosting a GUIEffect (letterbox/fade bars).
// Vtable 320 bytes @0x635B18 — the 38 GUIView slots with slot 7/8
// (~GUIEffectView), slot 9 (Update), slot 10 (DrawRect) and slot 11
// (LayoutSubviews) overridden (ABS64 relocation dump).
//
// Layout: GUIView (256 bytes), then
//   +256 GUIEffect effect (112 bytes — layout in GUIEffectView.cpp)
//   +368 bool removeWhenFinished (name UNRECOVERED; when set and the
//        effect's +0 flag is raised, Update removes the view from its
//        superview via vtable slot 15)
#ifndef CAVER_GUIEFFECTVIEW_H
#define CAVER_GUIEFFECTVIEW_H

#include "../core/Rectangle.h"
#include "GUIView.h"

namespace Caver {

class GUIEffect;

class GUIEffectView : public GUIView {
public:
    GUIEffectView();
    ~GUIEffectView();

    void Update(float deltaTime) override;  // slot 9
    void DrawRect(RenderingContext* context, const Rectangle& rect,
                  const Matrix4& transform) override;  // slot 10
    void LayoutSubviews() override;  // slot 11

    // The GUIEffect member lives at +256; it is laid out in the .cpp
    // (GUIEffect itself belongs to another GUI slice).
    char effectStorage[112];  // +256 GUIEffect
    bool removeWhenFinished;  // +368 UNRECOVERED name
};

} // namespace Caver

#endif
