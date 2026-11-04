// DesktopPlatform.h - Platform display and window management interface.
// Credit: Quantum Creeper / Aevora Labs.
#ifndef CAVER_DESKTOPPLATFORM_H
#define CAVER_DESKTOPPLATFORM_H

namespace Caver {
namespace Platform {

bool IsDesktop();
bool IsFullscreen();
void SetFullscreen(bool fullscreen);
void ToggleFullscreen();
void GetCurrentResolution(int* width, int* height);
void SetResolution(int width, int height);
bool IsVSync();
void SetVSync(bool vsync);

} // namespace Platform
} // namespace Caver

#endif // CAVER_DESKTOPPLATFORM_H
