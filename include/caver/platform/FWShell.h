// Recovered from libswordigo.so (1.4.13) evidence — see docs/CORE_HEADERS.md.
// Cross-platform application shell: owns the command-line arguments, the
// shell preferences, the set of live touches and the platform callbacks the
// game drives.
//
// Vtable (address point 0x633F28, RTTI "Caver::FWShell" derives from
// "Caver::IFWTouchResponder"): slots 0-3 TouchBegan/Moved/Ended/Cancelled,
// 4 ~D1, 5 ~D0, 6 InitApplication, 7 QuitApplication,
// 8 SuspendApplication(bool), 9 ResumeApplication, 10 InitView,
// 11 ReleaseView, 12 ApplicationShouldQuit, 13 Update(float),
// 14 Render(RenderingContext*). All confirmed from the ABS64 relocations.
//
// Layout (sizeof 104, verified from the constructor @0x50BCF4):
//   +0   vptr
//   +8   std::vector<std::string> m_arguments  (24 B)
//   +32  FWShellPreferences m_preferences      (40 B)
//   +72  std::set<FWTouch> m_touches           (24 B: begin node, root, size)
//   +96  bool m_applicationInited
#ifndef CAVER_FWSHELL_H
#define CAVER_FWSHELL_H

#include <set>
#include <string>
#include <vector>

#include "../caver/core/FWTouch.h"
#include "FWShellPreferences.h"

namespace Caver {

class RenderingContext;

class FWShell {
public:
    FWShell();
    virtual bool TouchBegan(const FWTouch& touch);
    virtual bool TouchMoved(const FWTouch& touch);
    virtual bool TouchEnded(const FWTouch& touch);
    virtual bool TouchCancelled(const FWTouch& touch);
    virtual ~FWShell();

    virtual void InitApplication();
    virtual void QuitApplication();
    virtual void SuspendApplication(bool saveState);
    virtual void ResumeApplication();
    virtual void InitView();
    virtual void ReleaseView();
    virtual bool ApplicationShouldQuit();
    virtual void Update(float deltaTime);
    virtual void Render(RenderingContext* context);

    void AddArgument(const std::string& argument);

    // Non-virtual debug overlay: draws every live touch as a small quad.
    void DrawTouches(RenderingContext* context);

protected:
    std::vector<std::string> m_arguments;       // +8
    FWShellPreferences m_preferences;           // +32
    std::set<FWTouch> m_touches;                // +72
    bool m_applicationInited;                   // +96
};

} // namespace Caver

#endif
