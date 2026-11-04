// Recovered from libswordigo.so (1.4.13) evidence.
//
// GUIEffect: a full-frame overlay effect (fade to/from a colour, and
// letterbox bars) drawn as a coloured vertex strip or two fill rects.
// NON-POLYMORPHIC: the golden exports no _ZTV/_ZTI/_ZTS for GUIEffect
// (verified by symbol-table scan), so it has no vtable at all.
//
// Layout (112 bytes, proven by the ctor @0x52EE68, SetFrame @0x52EEA4,
// CreateVertices @0x52EEB0, Update @0x52F358, Draw @0x52F52C and the
// destructor @0x52F774):
//   +0   bool finished (raised when the running effect completes;
//        GUIEffectView::Update reads it to remove the view)
//   +8   GUITargetAction targetAction (48 bytes; fired on completion)
//   +56  int mode (0 = idle, 1 = fade, 2 = bars — set by FadeIn/
//        FadeOut/ShowBars/HideBars, read by Update and Draw)
//   +60  int field60 (selects the CreateVertices layout: 0 = 6-vertex
//        strip, 1 = 24-vertex strip; no function in this class writes
//        it besides the ctor's zero — name/purpose UNRECOVERED)
//   +64  bool reversed (FadeIn/HideBars set it, FadeOut/ShowBars
//        clear it; Update steps progress towards 0 when set, towards
//        1 when clear)
//   +68  float duration (ctor: 0.3f)
//   +72  Color color (ctor: dword 0xFF000000, opaque black)
//   +76  float progress (0..1; stepped by Update, scales the vertex
//        colours and the bar heights)
//   +80  Rectangle frame (written by SetFrame)
//   +96  int vertexCount (6 or 24, set by CreateVertices)
//   +104 void* vertices (vertex buffer, 16-byte stride:
//        float x/y/z + packed colour dword; operator delete[] in the
//        destructor)
#ifndef CAVER_GUIEFFECT_H
#define CAVER_GUIEFFECT_H

#include "../core/Color.h"
#include "../core/Rectangle.h"
#include "GUITargetAction.h"

namespace Caver {

class RenderingContext;

class GUIEffect {
public:
    GUIEffect();
    ~GUIEffect();

    void SetFrame(const Rectangle& frame);
    void CreateVertices();
    void FadeOut(float duration);
    void FadeIn(float duration);
    void ShowBars(float duration);
    void HideBars(float duration);
    void Update(float deltaTime);
    void Draw(RenderingContext* context);

    bool finished;              // +0
    char _pad1[7];              // +1
    GUITargetAction targetAction;  // +8
    int mode;                   // +56
    int field60;                // +60 UNRECOVERED name/purpose
    bool reversed;              // +64
    char _pad65[3];             // +65
    float duration;             // +68
    Color color;                // +72
    float progress;             // +76
    Rectangle frame;            // +80
    int vertexCount;            // +96
    char _pad100[4];            // +100
    void* vertices;             // +104
};

} // namespace Caver

#endif
