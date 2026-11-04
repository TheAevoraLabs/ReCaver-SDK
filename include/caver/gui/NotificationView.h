// Recovered from libswordigo.so (1.4.13) evidence — IDA/Hex-Rays
// exports in ~/workspace/analysis/swordigo/functions/Caver/
// NotificationView/, the golden vtable _ZTVN5Caver16NotificationViewE
// @0x631FC8 (320 bytes, ABS64 relocations) and the RTTI
// _ZTIN5Caver16NotificationViewE @0x632108 (__si_class_type_info whose
// single base is Caver::GUIView @0x635630).
//
// NotificationView: the typewriter text-bubble toast GameSceneView
// pins near the top of the screen (GameSceneView::ShowNotification
// stores it at GameSceneView+304 and centres it on the scene bounds).
// Callers allocate it with operator new(0x1C8) = 456 bytes and feed
// it through Init (the Map / spell-acquisition sites in
// libswordigo_ida.c). NOT a base or subclass of
// AchievementNotificationView (a GUIFrameView sibling) — the two
// share only the GUIView base.
//
// Vtable: exactly the 38 GUIView slots, overriding only the
// destructor (slots 7/8), Update (slot 9), DrawRect (slot 10) and
// LayoutSubviews (slot 11, an empty body). Init is non-virtual.
// The destructor is NOT user-declared: the golden D0 @0x4CAD5C is
// a full D2 clone + operator delete (the implicit-destructor
// emission shape, cf. GameOverView / PauseView), not the
// call-D1 thunk a user-declared destructor produces.
//
// Layout (sizeof 456 = 0x1C8, byte-verified against the ctor
// @0x4CA434 store map, the dtor @0x4CACD0 teardown, Init @0x4CA494
// and Update @0x4CA87C):
//   +0..255  GUIView base
//   +256     GUITextBubble textBubble (168 bytes, embedded by value;
//            its text string sits at +296, shownCount at +400)
//   +424     bool fadeOutStarted (ctor: false; latched by Update
//            when the fade-out animation is created) — name
//            UNRECOVERED
//   +428     float elapsedTime (auto-dismiss timer; ctor: 0.0f;
//            only accumulates once the bubble text is fully shown)
//            — name UNRECOVERED
//   +432     float displayDuration (ctor: 1.0f; Update starts the
//            fade once elapsedTime exceeds it; no other writer in
//            the class) — name UNRECOVERED
//   +440     boost::shared_ptr<GUIAnimation> scaleAnimation (the
//            scale-in animation built by Init; Update steps it
//            manually and clears it once it reports finished, in
//            which case the bubble's own Update is skipped) — name
//            UNRECOVERED
#ifndef CAVER_NOTIFICATIONVIEW_H
#define CAVER_NOTIFICATIONVIEW_H

#include <boost/intrusive_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <string>

#include "GUITextBubble.h"
#include "GUIView.h"

namespace Caver {

class GUIAnimation;
class RenderingContext;
class Texture;

class NotificationView : public GUIView {
public:
    NotificationView();
    // No user-declared destructor (see the vtable note above);
    // the implicit destructor releases scaleAnimation, then the
    // textBubble, then the GUIView base — slots 7, 8.

    void Update(float deltaTime) override;  // slot 9
    void DrawRect(RenderingContext* context, const Rectangle& rect,
                  const Matrix4& transform) override;  // slot 10
    void LayoutSubviews() override;                    // slot 11

    // The intrusive_ptr<Texture> argument is NEVER read by the
    // golden body (every call site passes a null intrusive_ptr);
    // Init always frames the bubble with the "textbubble_border"
    // texture instead. Reproduced as-is, bug-for-bug.
    void Init(const boost::intrusive_ptr<Texture>& texture,
              const std::string& text);

    GUITextBubble textBubble;  // +256
    bool fadeOutStarted;       // +424 UNRECOVERED name
    float elapsedTime;         // +428 UNRECOVERED name
    float displayDuration;     // +432 UNRECOVERED name
    boost::shared_ptr<GUIAnimation> scaleAnimation;  // +440 UNRECOVERED name
};

// Golden aarch64 NDK-libc++ layout anchor; MSVC STL member sizes differ,
// so this assert is Android/Linux-only (the Windows build is self-consistent
// under MSVC STL and does not claim golden byte-layout parity).
#ifndef _MSC_VER
static_assert(sizeof(NotificationView) == 456,
              "NotificationView layout: operator new(0x1C8) at call sites");
#endif

} // namespace Caver

#endif
