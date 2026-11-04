// ProgramSystem.h - Native cross-platform system subsystem and LNI bridge.
// Provides native URL opening, clipboard I/O, device info, high-res timing, and game speed.
// Credit: Quantum Creeper / Aevora Labs.

#ifndef CAVER_PROGRAM_SYSTEM_H
#define CAVER_PROGRAM_SYSTEM_H

#include <cstdint>

namespace Caver {

class ProgramState;

class ProgramSystem {
public:
    static void RegisterLibrary(ProgramState* state);

    static float GetGameSpeed();
    static void SetGameSpeed(float speed);

    static bool OpenURL(const char* url);
    static bool SetClipboard(const char* text);
    static const char* GetClipboard();
    static const char* GetPlatformName();
    static const char* GetDeviceModel();
    static const char* GetLocale();
    static double GetTimeMS();
    static int64_t GetUnixTime();
    static void Vibrate(int milliseconds);
    static void ShowToast(const char* message);
    static void KeepScreenOn(bool keepOn);
};

} // namespace Caver

#endif // CAVER_PROGRAM_SYSTEM_H
