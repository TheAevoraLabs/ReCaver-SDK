#ifndef CAVER_GAMEOPTIONS_H
#define CAVER_GAMEOPTIONS_H

#include <string>
#include <vector>
#include <boost/shared_ptr.hpp>

namespace Caver {

namespace Proto {
class GameOptions;
}

class MusicPlaylist;
class GUIViewLayout;

class GameOptions {
public:
    static boost::shared_ptr<GameOptions> defaultOptions();
    static boost::shared_ptr<GameOptions> sharedOptions();

    GameOptions();
    ~GameOptions();

    void LoadFromFile(const std::string& path);
    void SaveToFile();
    void LoadFromProtobufMessage(const Proto::GameOptions& message);
    void SaveToProtobufMessage(Proto::GameOptions* message) const;

    void ApplyAudioOptions();
    void SetControlsLayout(const boost::shared_ptr<GUIViewLayout>& layout);

    std::string m_filePath;                                     // +0
    std::vector<boost::shared_ptr<MusicPlaylist> > m_playlists; // +24
    bool musicEnabled;                                          // +48
    bool soundEnabled;                                          // +49
    char _pad50[2];                                             // +50
    float musicVolume;                                          // +52
    float soundVolume;                                          // +56
    char _pad60[4];                                             // +60
    boost::shared_ptr<GUIViewLayout> m_layout64;                // +64 (phoneControlsLayout)
    boost::shared_ptr<GUIViewLayout> m_layout80;                // +80 (padControlsLayout)
    bool guidePurchased;                                        // +96
    bool coinDoublerPurchased;                                  // +97
    bool noAdsPurchased;                                        // +98
    char _pad99[5];                                             // +99 -> 104 (0x68)
};

// Golden aarch64 NDK-libc++ layout anchor; MSVC STL member sizes differ,
// so this assert is Android/Linux-only (the Windows build is self-consistent
// under MSVC STL and does not claim golden byte-layout parity).
#ifndef _MSC_VER
static_assert(sizeof(GameOptions) == 0x68, "GameOptions size must be 0x68");
#endif

} // namespace Caver

#endif // CAVER_GAMEOPTIONS_H
