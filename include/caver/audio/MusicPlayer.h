// Recovered from libswordigo.so (1.4.13) evidence — IDA Pro / Hex-Rays
// exports (~/workspace/analysis/swordigo/functions/Caver/MusicPlayer/),
// arbitrated against the golden binary's disassembly, relocations and
// vtables. Graded with tools/parity (see the systems slice scratch dir).
//
// MusicPlayer is the engine's streaming-music controller: it owns one
// AudioPlayer at a time, crossfades between named MusicPlaylists, and
// exposes a small Lua library ("MusicPlayer": PlayMusic / FadeOut /
// FadeIn).
//
// Object size is 0x70 (112) bytes, verified from the operator new(0x70)
// in Caver::AudioSystem::Setup. Layout from the constructor stores
// (0x50DE94), the destructor's member-release order and every accessor:
//
//   +0   bool m_enabled          (init true)
//   +1   bool m_suspended        (init false)
//   +2   bool m_shouldPlay       (init true; cached m_enabled && !m_suspended)
//   +4   float m_volume          (init 1.0f; set by SetVolume)
//   +8   bool m_looping          (init true; forwarded to the AudioPlayer)
//   +16  boost::shared_ptr<AudioPlayer> m_player
//   +32  int m_fadeState         (0 = steady, 1 = fading in, -1 = fading out)
//   +36  float m_fadeRate        (1 / fade duration)
//   +40  float m_fadeVolume      (init 1.0f; current fade multiplier)
//   +48  boost::shared_ptr<MusicPlaylist> m_currentPlaylist
//   +64  boost::shared_ptr<MusicPlaylist> m_nextPlaylist (pending crossfade)
//   +80  bool m_fadeToNext       (init false; PlayMusicWithName's fade flag:
//                                 fade back in after the pending switch)
//   +88  std::map<std::string, boost::shared_ptr<MusicPlaylist> > m_playlists
#ifndef CAVER_MUSICPLAYER_H
#define CAVER_MUSICPLAYER_H

#include <map>
#include <string>
#include <vector>

#include <boost/shared_ptr.hpp>

namespace Caver {

class ProgramState;

namespace Proto {
class MusicPlaylist;
}

// One track of a playlist: an audio resource name plus its relative
// volume. Size 0x20 verified from the stack copies in PlaylistForName /
// CreatePlayerForPlaylist / UpdatePlayerVolume (string at +0, float
// at +24, default volume 1.0f).
struct MusicTrack {
    std::string resourceName; // +0
    float volume;             // +24

    MusicTrack() : volume(1.0f) {}
};

// A named set of tracks. Size 0x38 verified from the operator new(0x38)
// in PlaylistForName and the constructor stores at 0x50FBA4:
//   +0   std::string name
//   +24  bool looping (init true; name is a reconstruction — the field
//        is not read by any MusicPlayer function and is not serialised
//        by Load/SaveToProtobufMessage, which cover name + tracks only)
//   +32  std::vector<MusicTrack> tracks
// Only the constructor is used by MusicPlayer; Load/Save are declared
// here and defined in their own translation unit.
class MusicPlaylist {
public:
    MusicPlaylist();

    void LoadFromProtobufMessage(const Proto::MusicPlaylist& message);
    void SaveToProtobufMessage(Proto::MusicPlaylist* message);

    std::string name;              // +0
    bool looping;                  // +24
    std::vector<MusicTrack> tracks; // +32
};

// The platform audio player MusicPlayer drives. Abstract interface
// recovered from the AudioPlayerImpl vtable in the golden binary
// (address point 0x6340C8; slot order proven by the relocation table
// and by the call sites in MusicPlayer, which use +16 InitWithFile,
// +24 Update, +32 Play, +40 Pause, +72 setLooping, +104 setVolume):
//   +0  ~AudioPlayer (complete)   +8  ~AudioPlayer (deleting)
//   +16 InitWithFile              +24 Update
//   +32 Play                      +40 Pause
//   +48 Stop                      +56 isPlaying (const)
//   +64 isLooping (const)         +72 setLooping
//   +80 duration (const)          +88 currentTime (const)
//   +96 volume (const)            +104 setVolume
class AudioPlayer {
public:
    virtual ~AudioPlayer() {}

    virtual bool InitWithFile(const std::string& path) = 0;
    virtual void Update(float deltaTime) = 0;
    virtual void Play() = 0;
    virtual void Pause() = 0;
    virtual void Stop() = 0;
    virtual bool isPlaying() const = 0;
    virtual bool isLooping() const = 0;
    virtual void setLooping(bool looping) = 0;
    virtual float duration() const = 0;
    virtual float currentTime() const = 0;
    virtual float volume() const = 0;
    virtual void setVolume(float volume) = 0;

    // Factory (a thunk to NewAudioPlayerImpl in the golden binary).
    static AudioPlayer* NewPlayer();
};

class MusicPlayer {
public:
    MusicPlayer();
    ~MusicPlayer();

    // Stores playlist in m_playlists under playlist->name, REPLACING
    // any existing entry with the same name (the map assignment runs
    // whether or not a new tree node had to be created — verified in
    // the 0x50DED8 disassembly). The shared_ptr is copied into the map;
    // the caller keeps its own reference.
    void AddPlaylist(const boost::shared_ptr<MusicPlaylist>& playlist);

    // Returns the playlist registered for name, looking it up through
    // the map's operator[] (which default-inserts an empty shared_ptr).
    // NOTE, binary-proven quirk: when no playlist exists yet, the new
    // MusicPlaylist is stored only in the RETURNED shared_ptr — the
    // map entry stays null — and a single MusicTrack named `name`
    // (volume 1.0f) is appended to the new playlist's track list.
    boost::shared_ptr<MusicPlaylist> PlaylistForName(const std::string& name);

    // Plays the named playlist. If it is already current and the fade
    // volume is >= 0.01f while not fading out, does nothing. If another
    // playlist is current, the new one becomes m_nextPlaylist and a
    // 1.5 s fade-out starts (rate 2/3); Update() performs the switch,
    // fading back in over 1.5 s only when `fade` is set. If nothing is
    // current, switches immediately, starting from silence when `fade`
    // is set. An empty name resolves to a null playlist.
    void PlayMusicWithName(const std::string& name, bool fade);

    void FadeIn(float duration);
    void FadeOut(float duration);

    void SetCurrentPlaylist(const boost::shared_ptr<MusicPlaylist>& playlist);
    void CreatePlayerForPlaylist(const boost::shared_ptr<MusicPlaylist>& playlist);
    void UpdatePlayerVolume();

    void SetEnabled(bool enabled);
    void UpdateShouldPlayState();
    void SetSuspended(bool suspended);
    void SetVolume(float volume);
    void SetLooping(bool looping);

    void Update(float deltaTime);

    void RegisterProgramLibrary(ProgramState* state);

private:
    bool m_enabled;                                        // +0
    bool m_suspended;                                      // +1
    bool m_shouldPlay;                                     // +2
    float m_volume;                                        // +4
    bool m_looping;                                        // +8
    boost::shared_ptr<AudioPlayer> m_player;               // +16
    int m_fadeState;                                       // +32
    float m_fadeRate;                                      // +36
    float m_fadeVolume;                                    // +40
    boost::shared_ptr<MusicPlaylist> m_currentPlaylist;    // +48
    boost::shared_ptr<MusicPlaylist> m_nextPlaylist;       // +64
    bool m_fadeToNext;                                     // +80
    std::map<std::string, boost::shared_ptr<MusicPlaylist> > m_playlists; // +88
};

} // namespace Caver

#endif
