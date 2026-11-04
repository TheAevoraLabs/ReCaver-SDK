// Recovered from libswordigo.so (1.4.13) evidence — see docs/CORE_HEADERS.md.
// The game-side shell: boots the view stack (window, navigation
// controller, libraries, options, profile) and pumps update/render and
// touch conversion between the platform and the GUI layer.
// Object size is 0xA8 bytes, verified from NewCaverShell's allocation.
#ifndef CAVER_CAVERSHELL_H
#define CAVER_CAVERSHELL_H

#include <string>
#include <boost/shared_ptr.hpp>

#include "FWShell.h"

namespace Caver {

class GUIWindow;
class GUINavigationController;
class GUIViewController;
class RenderingContext;

class CaverShell : public FWShell {
public:
    virtual ~CaverShell();

    virtual void InitApplication();
    virtual void QuitApplication();
    virtual void SuspendApplication(bool saveState);
    virtual void ResumeApplication();
    virtual void InitView();
    virtual void ReleaseView();
    virtual void Update(float deltaTime);
    virtual void Render(RenderingContext* context);

    // Non-virtual (absent from the vtable): also called by the platform
    // glue when the surface changes size.
    void UpdateViewSize();

    virtual bool TouchBegan(const FWTouch& touch);
    virtual bool TouchMoved(const FWTouch& touch);
    virtual bool TouchEnded(const FWTouch& touch);
    virtual bool TouchCancelled(const FWTouch& touch);

    FWTouch ConvertTouchToWindow(const FWTouch& touch);

private:
    std::string m_startupScene;                                 // +104
    boost::shared_ptr<GUIWindow> m_mainWindow;                  // +128
    boost::shared_ptr<GUINavigationController> m_navigationController; // +144
    bool m_suspended;                                           // +160
    char _pad[7];                                               // +161
};

// Creates the shell and feeds it the process arguments (argv[1..]).
CaverShell* NewCaverShell(int argc, char** argv);

} // namespace Caver

#endif
