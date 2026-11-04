// Recovered from libswordigo.so (1.4.13) evidence — see docs/CORE_HEADERS.md.
// STUB declarations for the engine singletons and libraries the shell
// drives during boot/shutdown. Signatures are verified from the binary's
// symbol table; class layouts are recovered in their own slices. The member
// offsets marked "+N" below are the ones CaverShell::InitView accesses
// directly, so they are pinned here even though the rest of each layout is
// still unknown.
#ifndef CAVER_ENGINESYSTEMS_H
#define CAVER_ENGINESYSTEMS_H

#include <string>
#include <vector>

#include <boost/shared_ptr.hpp>
#include <boost/intrusive_ptr.hpp>

#include <google/protobuf/message_lite.h>

#include "GameOptions.h"
#include "../proto/Common.pb.h"
#include "../../platform/FWKeyboard.h"
#include "../../platform/FWMouse.h"

namespace Caver {

// Caver::Proto::SoundLibrary used to be stubbed in this header with no
// data members at all (sizeof 8), while the generated definition it
// forwards to now (proto/Common.pb.h) is sizeof 80. CaverShell.cpp
// includes this header and constructs Proto::SoundLibrary by value at
// InitView, so the compiler reserved an 8-byte stack slot whose remainder
// Common.pb.cc's constructor overflowed on every boot -- an ODR violation
// that corrupted InitView's frame (ASan: stack-buffer-overflow at
// CaverShell.cpp:165) and crashed later in GameOptions::SaveToFile during
// ReleaseView. The generated class is the single definition; its size is
// pinned to the golden operator new(0x50) by static_asserts in
// proto/ProtoUtil.cpp.

bool LoadProtobufMessageFromFile(google::protobuf::MessageLite* message, const std::string& path);
bool SaveProtobufMessageToFile(const google::protobuf::MessageLite& message, const std::string& path);

class GUIWindow;
class MusicPlaylist;
class GameState;

// ---------------------------------------------------------------- input

// ---------------------------------------------------------------- audio

class MusicPlayer {
public:
    void AddPlaylist(const boost::shared_ptr<MusicPlaylist>& playlist);
};

// Forward declarations for the stub method declarations below.
class SoundEffectSource;
class Texture;

class AudioSystem {
public:
    static AudioSystem* sharedSystem();
    static void SetSharedSystem(AudioSystem* system);
    // The real AudioSystem.h declares bool Setup(); this stub said
    // void, which mismatched the definition at link time.
    bool Setup();
    void Update(float deltaTime);
    void Shutdown();
    // Mirrors AudioSystem.h: non-virtual, so declaring it here changes
    // no layout; it lets shim forwarders in TUs that see only this
    // stub call the real definition (same mangled name).
    boost::intrusive_ptr<SoundEffectSource> PlaySoundEffectWithName(
        const std::string& name, float volume);

    char _pad0[104];             // +0   (UNRECOVERED)
    MusicPlayer* m_musicPlayer;  // +104 (InitView loops over the options'
                                 //      playlists and calls AddPlaylist on it)
};

// ------------------------------------------------------------- libraries

class TextureLibrary {
public:
    static TextureLibrary* sharedLibrary();
    static void SetSharedLibrary(TextureLibrary* library);
    void Clear();
    void LoadTextureAtlasWithName(const std::string& name);
    // Mirrors TextureLibrary.h (see the AudioSystem note above).
    boost::intrusive_ptr<Texture> TextureForName(const std::string& name, bool flag);

    // Fields written directly by CaverShell::InitView (verified offsets).
    char _pad0[33];              // +0
    bool _field33;               // +33
    char _pad1[6];               // +34
    int _field40;                // +40 (texture cache limit, set to 40000000)
};

class FontLibrary {
public:
    static FontLibrary* sharedLibrary();
    static void SetSharedLibrary(FontLibrary* library);
    void Clear();

    char _pad0[24];              // +0
    bool _field24;               // +24
};

class ModelLibrary {
public:
    static ModelLibrary* sharedLibrary();
    static void SetSharedLibrary(ModelLibrary* library);
    void Clear();
};

class SoundLibrary {
public:
    static SoundLibrary* sharedLibrary();
    static void SetSharedLibrary(SoundLibrary* library);
    void LoadFromProtobufMessage(const Proto::SoundLibrary& message);
};

// -------------------------------------------------------------- game data

class AchievementsManager {
public:
    static AchievementsManager* sharedManager();
    void Update(float deltaTime);

    char _pad0[128];                          // +0   (UNRECOVERED)
    boost::shared_ptr<GUIWindow> m_window;    // +128 (InitView assigns the
                                              //       shell's main window)
};

// PlayerProfile and GameState are NOT re-declared here, only forward
// declared (GameState's forward declaration is at the top of this file).
// Narrowed re-declarations of both classes used to stand here
// (PlayerProfile with pads + shared_ptr<GameState> @+240 + bool @+280;
// GameState with a single string @+248, the scene name InitView writes,
// which is the real m_currentLevelName). The anchors agreed with the
// real layouts, but the member sets differed, so a TU that created a
// PlayerProfile through this header bound the shared_ptr deleter to the
// implicit destructor, which skipped the real destructor body (m_data
// delete[], string and GameData destruction) -- a leak -- and the
// 272-byte narrowed GameState invited a repeat of the SoundLibrary
// 8-vs-80 overflow the day any TU constructed one through this header.
// TUs that need these classes include the real headers
// (systems/PlayerProfile.h, core/GameState.h). This is the same shape
// as the GUITextField factory ODR fix: forward-declare here, include
// the real header where the object is used.
class PlayerProfile;

class ProfileManager {
public:
    static ProfileManager* sharedManager();
    void AddProfile(const boost::shared_ptr<PlayerProfile>& profile);
};

} // namespace Caver

#endif
