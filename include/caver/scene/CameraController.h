// Recovered from libswordigo.so (1.4.13) evidence — IDA/Hex-Rays
// exports in ~/workspace/analysis/swordigo/functions/Caver/
// CameraController/ and the ctor @0x4ECC00 store map (cross-checked
// against the SwordigoDesktop sre13 CameraController doc; the binary
// wins where they differed — e.g. FocusAtShape does NOT store the
// shape/rect, it only reads its arguments).
//
// CameraController: the game's camera driver. No vtable. It lerps a
// current camera position / focus point toward targets, optionally
// following a scene object or framing a shape, and pushes the result
// into its owned Caver::Camera (position @Camera+16, rotation
// quaternion @Camera+28 — those offsets are the core/render slice's
// Camera layout, used here only through CameraController's own
// bodies).
//
// Layout (sizeof 160):
//   +0   int m_state — 0 idle, 1 focusing, 2 keep-focus (ResetFocus;
//        Update clears it once both targets are within 100 units)
//   +4   Vector3 m_cameraOffset — direction/offset from the focus
//        target to the camera (set externally, e.g. by
//        GameSceneController::InitWithScene)
//   +16  Vector3 m_targetPos — camera destination
//   +28  float m_lerpFactor — position lerp rate (0.97 in the
//        Focus* methods)
//   +32  Vector3 m_targetFocusPos — look-at destination
//   +44  float m_zoom — focus lerp rate (0.96 in the Focus*
//        methods; name from the desktop doc — the field doubles as
//        the focus-position lerp factor in Update)
//   +48  Vector3 m_currentPos — camera position this frame
//   +60  Vector3 m_currentFocusPos — look-at point this frame
//   +72  Vector3 m_upVector — LookAt up argument (zeroed by the
//        ctor, set externally)
//   +84  int m_field84 — UNRECOVERED; the ctor never writes it
//   +88  boost::shared_ptr<Camera> m_camera
//   +104 boost::intrusive_ptr<SceneObject> m_followObject
//   +112 boost::intrusive_ptr<ShapeComponent> m_followShape
//   +120 Vector3 m_followOffset
//   +132 Rectangle m_followRect
//   +148 float m_rumblePhase
//   +152 float m_rumbleTime — set to 1.0 by Rumble(), counted down
//        by Update while > 0.01
#ifndef CAVER_CAMERACONTROLLER_H
#define CAVER_CAMERACONTROLLER_H

#include <boost/intrusive_ptr.hpp>
#include <boost/shared_ptr.hpp>

#include "../core/Camera.h"
#include "../core/Rectangle.h"
#include "../core/Vector3.h"
#include "../scene/SceneObject.h"
#include "../component/ShapeComponent.h"

namespace Caver {

class ProgramState;

class CameraController {
public:
    CameraController();
    ~CameraController();

    void Update(float deltaTime);
    void FocusAtPoint(const Vector3& point, bool clearFollow);
    void FocusAtShape(const boost::intrusive_ptr<ShapeComponent>& shape,
                      const Rectangle& rect, bool clearFollow);
    void ResetFocus();
    void StopFollowing();
    void FocusAtRectangle(const Rectangle& rect, float z, bool clearFollow);
    void FollowObject(const boost::intrusive_ptr<SceneObject>& object,
                      const Vector3& offset);
    void FollowShape(const boost::intrusive_ptr<ShapeComponent>& shape,
                     const Rectangle& rect);
    void GotoTargetImmediately();
    void Rumble();
    void RegisterProgramLibrary(ProgramState* state);

    int m_state;                                        // +0
    Vector3 m_cameraOffset;                             // +4
    Vector3 m_targetPos;                                // +16
    float m_lerpFactor;                                 // +28
    Vector3 m_targetFocusPos;                           // +32
    float m_zoom;                                       // +44
    Vector3 m_currentPos;                               // +48
    Vector3 m_currentFocusPos;                          // +60
    Vector3 m_upVector;                                 // +72
    int m_field84;                                      // +84 UNRECOVERED
    boost::shared_ptr<Camera> m_camera;                 // +88
    boost::intrusive_ptr<SceneObject> m_followObject;   // +104
    boost::intrusive_ptr<ShapeComponent> m_followShape; // +112
    Vector3 m_followOffset;                             // +120
    Rectangle m_followRect;                             // +132
    float m_rumblePhase;                                // +148
    float m_rumbleTime;                                 // +152
};

} // namespace Caver

#endif
