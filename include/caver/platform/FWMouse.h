#ifndef CAVER_FWMOUSE_H
#define CAVER_FWMOUSE_H

#include <list>
#include "caver/core/Vector2.h"

namespace Caver {

class IFWMouseListener {
public:
    virtual void MouseDown(const Vector2& position, unsigned int button, double time) = 0;
    // Golden @0x532D8C/@0x52C590: listeners receive (previous
    // position, current position) — NOT (position, delta). The
    // previous argument is the position stored by the last move
    // event (or the current position on the very first move).
    virtual void MouseMoved(const Vector2& previousPosition, const Vector2& position, unsigned int button, double time) = 0;
    virtual void MouseUp(const Vector2& position, unsigned int button, double time) = 0;
    virtual void MouseWheelRotated(float delta, const Vector2& position, unsigned int button, double time) = 0;
    virtual ~IFWMouseListener() {}
};

class FWMouse {
public:
    static FWMouse* sharedMouse();

    ~FWMouse();

    void AddListener(IFWMouseListener* listener);
    void RemoveListener(IFWMouseListener* listener);
    void RemoveAllListeners();

    void SendMouseDownEvent(const Vector2& position, unsigned int button, double time);
    // Golden @0x532D8C passes the caller's buttons through to
    // listeners (a3); the previous signature dropped it and
    // synthesized 0/1 from m_buttonDown instead.
    void SendMouseMovedEvent(const Vector2& position, unsigned int buttons, double time);
    void SendMouseUpEvent(const Vector2& position, unsigned int button, double time);
    void SendMouseWheelRotatedEvent(float delta, double time);

private:
    FWMouse() : m_buttonDown(false), m_positionInitialized(false), m_position() {}
    std::list<IFWMouseListener*> m_listeners;
    bool m_buttonDown;
    bool m_positionInitialized;
    Vector2 m_position;
};

} // namespace Caver

#endif // CAVER_FWMOUSE_H
