// Recovered from libswordigo.so (1.4.13) evidence — see docs/CORE_HEADERS.md.
// Shared declarations for the Android JNI glue TU (NativeJNI.cpp), which
// implements the Java_com_touchfoo_swordigo_Native_* exports declared in
// app/src/main/java/com/touchfoo/swordigo/Native.java plus the Caver::
// Android* bridge helpers and the platform text-input entry points.
//
// The TU's shared state is FILE-STATIC: one contiguous .bss block at
// 0x6516F0..0x651790 in the golden binary (unnamed there — TU-local state
// in the original), defined in golden address order at the top of
// NativeJNI.cpp. Every Native JNI cluster (setup / lifecycle / store /
// text-input) is merged into that one TU and shares this block:
//
//   0x6516F0  boost::shared_ptr<Caver::RenderingContext>          g_renderingContext
//   0x651700  boost::shared_ptr<Caver::CaverShell>                g_shell
//   0x651710  boost::shared_ptr<Caver::OnlineController_Android>  g_onlineController
//   0x651720  boost::shared_ptr<Caver::ProfileManager_Android>    g_profileManager
//   0x651730  boost::shared_ptr<Caver::StoreController_Android>   g_storeController
//   0x651740  JNIEnv*                                             g_jniEnv
//   0x651748  jclass (global ref to com.touchfoo.swordigo.Native) g_nativeClass
//   0x651750  jobject (global ref to the Java AssetManager)       g_assetManagerGlobalRef
//   0x651758  flag byte (re-entry flag)                           g_applicationLaunched
//   0x651759  flag byte (bool semantics, stored 0/1)              g_viewSizeInitialized
//   0x65175A  flag byte (bool semantics, stored 0/1)              g_applicationInactive
//   0x65175C  flag byte (last googleSignInCompleted(bool) value)  g_googleSignInCompleted
//   0x651760  Caver::Rectangle                                    g_windowRect
//   0x651770  Caver::Rectangle                                    g_viewportRect
//   0x651780  Caver::Vector2 (display-cutout pair, floats)        g_displayCutout
//   0x651788  float                                               g_viewScale
//   0x651790  Caver::ITextInputDelegate*                          g_textInputDelegate
//
// Who writes/reads what (from the golden bodies):
//   g_jniEnv / g_nativeClass: set by setupNativeInterface; the env is used
//     by handleApplicationQuit to DeleteGlobalRef(g_nativeClass) and by
//     every Caver::Android* bridge helper to call back into Java.
//   g_assetManagerGlobalRef: set by setAssetManager (never read back by
//     any recovered function — bug-for-bug, it is only kept alive).
//   g_applicationLaunched: re-entry flag set by handleApplicationLaunch,
//     gates setupApplication, cleared by handleApplicationQuit.
//   g_viewSizeInitialized: set by setApplicationViewSize (cluster B) once
//     the view size is known; setupApplication clears it;
//     handleApplicationQuit calls ReleaseView only while it is set, then
//     clears it.
//   g_applicationInactive: set by applicationDidBecomeInactive, cleared
//     by applicationDidBecomeActive and by setupApplication.
//   g_googleSignInCompleted: written by googleSignInCompleted(bool)
//     (cluster C), replayed by setApplicationViewSize at first view-size,
//     cleared by handleApplicationQuit.
//   g_windowRect / g_viewportRect / g_displayCutout / g_viewScale: written
//     by setApplicationViewSize, read by drawApplication / handleTouchEvent.
//   g_textInputDelegate: written by Caver::StartTextInputWithDelegate,
//     cleared by StopTextInputWithDelegate, read by the textInput*
//     JNI callbacks.
//
// The engine classes below are declared minimally — only the surface the
// JNI glue calls, with mangled names verified against the golden symbol
// table. HAND-OFF: when a TU includes the full class headers, retire the
// matching minimal declaration here instead of editing calls. One such
// retirement has happened: StoreController / StoreController_Android now
// come from caver/systems/StoreController_Android.h, which compiles
// cleanly in this TU and keeps the golden sizeof (0x70).
#ifndef CAVER_NATIVEJNI_H
#define CAVER_NATIVEJNI_H

#include <jni.h>

#include <string>

#include <boost/shared_ptr.hpp>

#include "CaverShell.h"
#include "FWShellPreferences.h"
#include "caver/core/Color.h"
#include "caver/core/Matrix4.h"
#include "caver/core/Rectangle.h"
#include "caver/core/Vector2.h"
#include "caver/systems/OnlineController.h"
#include "caver/systems/OnlineController_Android.h"
#include "caver/systems/StoreController_Android.h"

struct AAssetManager;

namespace Caver {

// ------------------------------------------------------------ free functions

std::string UniqueIdentifier();

void SetAndroidFilesDir(const std::string& path);
void SetAndroidCacheDir(const std::string& path);
void SetAndroidAssetManager(AAssetManager* manager);
void SetResourcesPath(const std::string& path);

// Device type reported to the engine by setApplicationViewSize
// (_ZN5Caver20SetCurrentDeviceTypeENS_10DeviceTypeE).
enum DeviceType {
    DeviceTypePhone = 0,
    DeviceTypeTablet = 1
};

void SetCurrentDeviceType(DeviceType type);

// ------------------------------------------------- platform text-input API

// Platform text-input delegate (secondary base of GUITextFieldImpl at
// +312 in that slice). Slot order proven by the secondary vtable
// @0x636488 in caver/gui/GUITextFieldImpl.h: slot 0 returns the filtered
// text by value (hidden sret), slot 1 is the finish notification —
// exactly the two calls the golden JNI text-input bodies make.
class ITextInputDelegate {
public:
    virtual std::string TextInputTextDidChange(const std::string& text) = 0;
    virtual void TextInputDidFinish() = 0;
};

// Defined by this TU (golden 0x508FC4 / 0x5090FC).
// DECLARATION CONFLICT (merge note, do not "fix" here): the golden body
// of StartTextInputWithDelegate TAIL-CALLS the void CallStaticVoidMethod
// wrapper, i.e. the function really returns void; it is declared as
// returning bool in caver/gui/GUITextFieldImpl.h (inferred there from
// callers letting x0 flow out). The mangled name is identical either
// way. The correct void declaration lives here, in the platform TU's
// header scope; GUITextFieldImpl.h keeps its bool declaration until
// that slice reconciles it.
void StartTextInputWithDelegate(ITextInputDelegate* delegate,
                                const std::string& text);
void StopTextInputWithDelegate(ITextInputDelegate* delegate);

// ------------------------------------------------------------ engine systems

class AudioSystem {
public:
    static bool hasSharedSystem();
    static AudioSystem* sharedSystem();
    static void SetSharedSystem(AudioSystem* system);
    void Shutdown();
    void AudioInterruptionDidBegin();
    void AudioInterruptionDidEnd();
};

class FontLibrary {
public:
    static void SetSharedLibrary(FontLibrary* library);
};

class ModelLibrary {
public:
    static void SetSharedLibrary(ModelLibrary* library);
};

class TextureLibrary {
public:
    static TextureLibrary* sharedLibrary();
    static void SetSharedLibrary(TextureLibrary* library);
    void ReloadTextures(bool reload);
};

class SoundLibrary {
public:
    static void SetSharedLibrary(SoundLibrary* library);
};

class FWKeyboard {
public:
    static FWKeyboard* sharedKeyboard();
    void RemoveAllListeners();
    void SendKeyDownEvent(unsigned int key, unsigned int modifiers, double time);
    void SendKeyUpEvent(unsigned int key, unsigned int modifiers, double time);
};

class FWMouse {
public:
    static FWMouse* sharedMouse();
    void RemoveAllListeners();
};

// RenderingContext is declared minimally here instead of including
// caver/render/RenderingContext.h: that header's intrusive_ptr<Texture>
// members need intrusive_ptr_add_ref/release declarations for Texture
// that no recovered header provides yet, so it does not compile from a
// fresh TU. HAND-OFF: once the render slice supplies them, include the
// real header and retire this declaration. sizeof must stay 0x128
// (golden operator new size); the class has no virtuals. The methods
// below are the exact surface clusters A/B call (signatures and
// manglings verified against the golden symbol table and against
// caver/render/RenderingContext.h).
enum RenderingAPI {
    RenderingAPIGLES1 = 0,
    RenderingAPIGLES2 = 1
};

class RenderingContext {
public:
    explicit RenderingContext(RenderingAPI api);
    static RenderingContext* SetCurrentContext(RenderingContext* context);

    void SetViewport(const Rectangle& viewport);
    void SetProjectionMatrix(const Matrix4& matrix);
    void SetDepthTestEnabled(bool enabled);
    void FillRect(const Rectangle& rect, const Color& color, float depth);

private:
    char _pad[0x128];
};

// ------------------------------------------------------- platform controllers

// OnlineController_Android now comes from
// caver/systems/OnlineController_Android.h (golden sizeof 0x38, the
// JNI-backed interface): setupApplication only ever constructs it and
// googleSignInCompleted only calls HandleGoogleSignInCompleted.

class ProfileManager {
public:
    static void SetSharedManager(const boost::shared_ptr<ProfileManager>& manager);

private:
    char _basePad[8]; // size placeholder (real layout: ProfileManager slice)
};

// sizeof must stay 0x78 (golden operator new size).
class ProfileManager_Android : public ProfileManager {
public:
    ProfileManager_Android();
    void HandleSnapshotLoaded(const std::string& name, const std::string& data);

private:
    char _pad[0x78 - 0x8];
};

// StoreController / StoreController_Android / StoreProduct: retired
// from here — the declarations now come from
// caver/systems/StoreController_Android.h (included above), whose
// layout reproduces the golden operator-new size 0x70.

} // namespace Caver

// ------------------------------------------------------------ shared JNI state
// The state is FILE-STATIC in the original TU: one contiguous .bss block
// at 0x6516F0..0x651790, addressed directly (adrp + offset) by every
// golden body, never through the GOT, and unnamed in the golden symtab.
// The definitions live at the top of NativeJNI.cpp in golden address
// order (see the map in that file and in the header comment above);
// all JNI clusters merged into the same TU use them from there.

#endif
