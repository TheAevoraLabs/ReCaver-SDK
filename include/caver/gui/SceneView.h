// Recovered from libswordigo.so (1.4.13) evidence — IDA/Hex-Rays
// exports in ~/workspace/analysis/swordigo/functions/Caver/SceneView/,
// the golden disassembly of every body, and the golden vtable
// relocations.
//
// SceneView: a GUIView that renders a Caver::Scene through its own
// embedded CameraController. Callers: MainMenuViewController::
// LoadView, CreditsView's ctor and ProfilePanelView::InitWithProfile
// (operator new(0x1B8) at those sites).
//
// RTTI @0x62BDB8 is a __si_class_type_info on GUIView. Vtable
// _ZTVN5Caver9SceneViewE @0x62BC78 (address point 0x62BC88), the 38
// GUIView slots with only these overrides:
//   7/8  ~SceneView D1/D2 (@0x444B04, WEAK) / D0 (@0x444C30, WEAK)
//        — the implicit-destructor shape (no user dtor declared)
//   9    Update (@0x4449C4)
//   10   DrawRect (@0x444A10)
//   11   LayoutSubviews (@0x444988)
//
// Layout (sizeof = 440 = 0x1B8, proven by the ctor @0x444738 and the
// TU-local stubs in MainMenuView.cpp / CreditsView.cpp, which this
// header supersedes):
//   +0..255  GUIView base
//   +256     boost::shared_ptr<Scene> — the hosted scene
//   +272     CameraController, embedded by value (160 bytes:
//            its shared_ptr<Camera> lands at +360, which InitWith-
//            Scene also installs into the Scene at Scene+440)
//   +432     bool — gates DrawRect's black FillRect pass (depth
//            func 519/always, blend on, then 513 restored); original
//            member name UNRECOVERED
// All member names below are UNRECOVERED; roles are proven by the
// seven bodies in SceneView.cpp.
#ifndef CAVER_SCENEVIEW_H
#define CAVER_SCENEVIEW_H

#include <boost/shared_ptr.hpp>

#include "GUIView.h"
#include "../scene/CameraController.h"

namespace Caver {

class Scene;

class SceneView : public GUIView {
public:
    SceneView();

    // No user-declared destructor: the golden D2 @0x444B04 (300
    // bytes, WEAK) / D0 @0x444C30 (36 bytes, WEAK) pair is the
    // implicit-destructor shape — members release in reverse
    // declaration order (the CameraController's follow shape,
    // follow object and camera inlined first, then the scene
    // shared_ptr), then ~GUIView.
    // HAND-OFF (scene/core slice): that inlining requires
    // CameraController's own destructor to be implicit/inline —
    // in the golden _ZN5Caver16CameraControllerD2Ev is WEAK, but
    // the in-tree CameraController.h declares it out-of-line, so a
    // tree build currently CALLS it from here (D2 140 bytes, D0
    // inlines that) instead of inlining the teardown. With the
    // declaration made implicit (scratch-stub proven) D0 is
    // byte-exact and D2 is 296/300 with only a cold-block layout
    // difference remaining.

    // Declaration order note: LayoutSubviews is declared first so
    // it is the class key function and the vtable (with the weak
    // implicit dtors) is emitted from SceneView.cpp.
    void LayoutSubviews() override;  // slot 11

    void Update(float deltaTime) override;  // slot 9
    void DrawRect(RenderingContext* context, const Rectangle& rect,
                  const Matrix4& transform) override;  // slot 10

    // Non-virtual: binds the scene, points the scene's camera slot
    // (Scene+440) at the controller's camera, registers the
    // controller's program library on the scene's root ProgramState
    // (Scene+40) and resets the controller's offset/targets.
    void InitWithScene(const boost::shared_ptr<Scene>& scene);

    boost::shared_ptr<Scene> m_scene;            // +256 name UNRECOVERED
    CameraController m_cameraController;         // +272
    bool field432;                               // +432 name UNRECOVERED
    char _pad433[7];                             // +433..439 -> 440
};

} // namespace Caver

#endif
