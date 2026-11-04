// Recovered from libswordigo.so (1.4.13) evidence — IDA/Hex-Rays exports
// in ~/workspace/analysis/swordigo/functions/Caver/DebugInfoOverlay/,
// the ABS64 relocations of the golden vtable, and the ctor/dtor bodies.
//
// DebugInfoOverlay: the in-game debug HUD (FPS / texture memory /
// vertex / triangle counters), owned by GameSceneView. It cycles
// through three display modes each time Toggle() is called.
//
// Vtable _ZTVN5Caver16DebugInfoOverlayE @0x62E668, 320 bytes = the 38
// GUIView slots, with exactly these overrides (ABS64 relocations):
//   7/8  ~DebugInfoOverlay (D2 @0x484C74, D0 @0x484E88)
//   9    Update @0x484938
//   10   DrawRect @0x4849E0
//   11   LayoutSubviews @0x484894
// All other slots keep the GUIView/GUIResponder functions; the class
// adds NO new virtual slots.
//
// RTTI @0x62E7A8 is a __si_class_type_info whose single base is
// Caver::GUIView (@0x62E7B8) — the direct base is GUIView.
//
// Layout (sizeof = 376 = 0x178, proven by the operator new(0x178) at
// GameSceneView::ToggleDebugInfo and by the ctor @0x483A4C stores /
// dtor @0x484C74 teardown order):
//   +0..255  GUIView base (ctor clears userInteractionEnabled at +245)
//   +256     boost::shared_ptr<GUILabel> m_fpsLabel
//   +272     boost::shared_ptr<GUILabel> m_textureMemoryLabel
//   +288     boost::shared_ptr<GUILabel> m_numVerticesLabel
//   +304     boost::shared_ptr<GUILabel> m_numTrianglesLabel
//   +320     std::vector<boost::shared_ptr<GUILabel>> m_labels
//            (the labels currently on screen, in display order)
//   +344     int m_textureMemoryUsage (cached; ctor -1)
//   +348     int m_frameCount (DrawRect increments it in FPS mode)
//   +352     float m_elapsedTime (accumulated in Update, FPS mode)
//   +356     float m_lastFPS (ctor -1.0f; setters skip when the new
//            value is within 0.1f of this)
//   +360     int m_numStaticVertices (cached; ctor -1)
//   +364     int m_numSkinnedVertices (cached; ctor -1)
//   +368     int m_numTriangles (cached; ctor -1)
//   +372     int m_mode (ctor -1; Toggle cycles -1 -> 0 -> 1 -> 2 -> 0;
//            0 = FPS label only, 1 = texture/vertex/triangle labels,
//            2 = no labels)
// Original member names are UNRECOVERED; the names here describe the
// role each offset plays in the golden bodies.
#ifndef CAVER_DEBUGINFOOVERLAY_H
#define CAVER_DEBUGINFOOVERLAY_H

#include <boost/shared_ptr.hpp>
#include <string>
#include <vector>

#include "GUILabel.h"
#include "GUIView.h"

namespace Caver {

class RenderingContext;

class DebugInfoOverlay : public GUIView {
public:
    DebugInfoOverlay();
    ~DebugInfoOverlay() override;  // slots 7, 8

    void Update(float deltaTime) override;               // slot 9
    void DrawRect(RenderingContext* context, const Rectangle& rect,
                  const Matrix4& transform) override;    // slot 10
    void LayoutSubviews() override;                      // slot 11

    // Non-virtual members (golden order is alphabetical in the census;
    // declaration order here follows the layout evidence).
    void Toggle();
    void SetFPS(float fps);
    void SetTextureMemoryUsage(int bytes);
    void SetNumVertices(int staticVertices, int skinnedVertices);
    void SetNumTriangles(int triangles);
    // NOTE: a non-static member in the golden (the int arrives in w1
    // and this in x0 is ignored by the body); it returns a std::string
    // by value and uses no member state.
    std::string FormatByteSize(int bytes);

    boost::shared_ptr<GUILabel> m_fpsLabel;              // +256
    boost::shared_ptr<GUILabel> m_textureMemoryLabel;    // +272
    boost::shared_ptr<GUILabel> m_numVerticesLabel;      // +288
    boost::shared_ptr<GUILabel> m_numTrianglesLabel;     // +304
    std::vector<boost::shared_ptr<GUILabel> > m_labels;  // +320
    int m_textureMemoryUsage;                            // +344
    int m_frameCount;                                    // +348
    float m_elapsedTime;                                 // +352
    float m_lastFPS;                                     // +356
    int m_numStaticVertices;                             // +360
    int m_numSkinnedVertices;                            // +364
    int m_numTriangles;                                  // +368
    int m_mode;                                          // +372 -> 376
};

} // namespace Caver

#endif
