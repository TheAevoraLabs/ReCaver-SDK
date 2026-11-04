// Recovered from libswordigo.so (1.4.13) evidence — IDA 0x5315D8 / 0x531604.
// Layout (44 bytes, no vtable): the constructor initialises
//   +0  bool  = false
//   +4  float = 0        (current location)
//   +8  bool  = false    (has target)
//   +12 float = 0        (target location)
//   +16 float = 0        (velocity)
//   +20 float = 1000.0   (velocity clamp)
//   +24 float = 5.0      (friction coefficient)
//   +28 float = 182.0    (spring coefficient)
//   +32 float = 20.0     (extra damping coefficient)
//   +36 int   = 0
//   +40 int   = 0        (sign-change counter, incremented in Update)
#ifndef CAVER_GUISCROLLLOCATIONCONTROLLER_H
#define CAVER_GUISCROLLLOCATIONCONTROLLER_H

namespace Caver {

class GUIScrollLocationController {
public:
    GUIScrollLocationController();
    float Update(float deltaTime);

    bool scrolling;        // +0
    float location;        // +4
    bool hasTarget;        // +8
    float targetLocation;  // +12
    float velocity;        // +16
    float maxVelocity;     // +20
    float friction;        // +24
    float spring;          // +28
    float damping;         // +32
    int field36;           // +36
    int counter;           // +40
};

} // namespace Caver

#endif
