#ifndef CAVER_FWKEYBOARD_H
#define CAVER_FWKEYBOARD_H

#include <list>

namespace Caver {

class IFFWKeyboardListener;
class IFWKeyboardListener {
public:
    virtual void KeyDown(unsigned int key, unsigned int modifiers, double time) = 0;
    virtual void KeyUp(unsigned int key, unsigned int modifiers, double time) = 0;
    virtual void KeyChar(unsigned int character, double time) = 0;
    virtual ~IFWKeyboardListener() {}
};

class FWKeyboard {
public:
    static FWKeyboard* sharedKeyboard();

    ~FWKeyboard();

    void AddListener(IFWKeyboardListener* listener);
    void RemoveListener(IFWKeyboardListener* listener);
    void RemoveAllListeners();

    void SendKeyDownEvent(unsigned int key, unsigned int modifiers, double time);
    void SendKeyUpEvent(unsigned int key, unsigned int modifiers, double time);
    void SendKeyCharEvent(unsigned int character, double time);

private:
    FWKeyboard() {}
    std::list<IFWKeyboardListener*> m_listeners;
};

} // namespace Caver

#endif // CAVER_FWKEYBOARD_H
