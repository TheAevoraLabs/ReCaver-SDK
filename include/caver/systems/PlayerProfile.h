// Recovered from libswordigo.so (1.4.13) evidence.
//
// Caver::PlayerProfile is one on-disk save slot. Layout established
// SERIALISATION-FIRST, per the save-data rule: every field below is
// cross-checked by at least two independent readers/writers among
// PlayerProfile::SaveToProtobufMessage (0x47C5E0),
// PlayerProfile::LoadFromProtobufMessage (0x47CBA8), the constructor
// (0x47B9D8), the destructor (0x324A6C) and the desktop-port doc
// (SwordigoDesktop docs/sre13/PlayerProfile.md — binary wins where
// they differed: the doc's +0xE0..+0xF8 "unknown" region is the two
// shared_ptrs at +224/+240).
//
//   +0   IPlayerProfileDelegate* m_delegate — raw, not refcounted.
//        Save() tail-calls vtable slot 0 (PlayerProfileWasSaved);
//        SetValueForCounter / IncreaseCounterValue call slot 1
//        (PlayerProfileCounterValueDidChange). ProfileManager is the
//        implementor: its vtable (0x62D8B0) is exactly
//        IPlayerProfileDelegate's five slots in the order declared
//        below, and its typeinfo is an __si_class_type_info whose
//        base is IPlayerProfileDelegate.
//   +8   unsigned char* m_data — cached serialised .gplayer bytes.
//        Written by Save (operator new[] of the ByteSize) and by
//        LoadFromPath (NewByteBufferFromFile); freed with
//        operator delete[] in the destructor.
//   +16  unsigned int m_dataSize — byte count for m_data. QUIRK: the
//        constructor does NOT initialise +16..+23 (its zero stores
//        cover +0..+15 and +24 on); the value is only ever set by
//        Save / LoadFromPath before the buffer is read.
//   +24  std::string m_identifier — proto identifier_ (+160 in the
//        message; set via InitWithIdentifier, deliberately NOT
//        restored by LoadFromProtobufMessage).
//   +48  std::string m_name — proto name_.
//   +72  DateTime m_lastPlayedTime — proto last_played_. DateTime is
//        an 8-byte double wrapper (Now() returns it in d0).
//   +80  std::string m_currentLevelTitle — proto
//        current_level_title_; refreshed from the current MapNode
//        by currentLevelTitle() before being saved.
//   +104 int m_level — initialised to 1 by the constructor; at save
//        time refreshed from the int at GameState+160
//        (CharacterState+144 — its name belongs to that slice) and
//        written to proto level_.
//   +108 float m_percentCompleted — cached
//        GameState::PercentCompleted() at save time; proto
//        percent_completed_.
//   +112 double m_timePlayed — proto time_played_ (seconds).
//   +120 std::string m_equippedWeaponName — proto
//        equipped_weapon_name_; refreshed by equippedWeaponName().
//   +144 std::string m_equippedArmorName — proto
//        equipped_armor_name_.
//   +168 std::string m_weaponTrinketName — proto
//        weapon_trinket_name_.
//   +192 std::string m_armorTrinketName — proto
//        armor_trinket_name_.
//   +216 bool m_loaded — set true at the end of
//        LoadFromProtobufMessage and by CreateProfile; cleared by
//        DeleteProfile.
//   +224 boost::shared_ptr<GameData> m_gameData.
//   +240 boost::shared_ptr<GameState> m_gameState —
//        SaveToProtobufMessage is a no-op unless this is non-null.
//   +256 std::map<std::string, int> m_counters — proto counters_
//        (repeated PlayerProfile_Counter name/value pairs).
//   +280 bool m_cheatEnabled — proto cheat_enabled (field 13; the
//        byte at message +152, has bit 0x1000). This is the save-file
//        cheat pass: ProfilePanelView's enablecheatpassu handler and
//        SaveToProtobufMessage both touch profile +280 / message +152,
//        CaverShell's scene boot sets it right before AddProfile, and
//        MapMenuPage / PortalView read it to reveal the whole map. It
//        persists in the .gplayer file until disablecheat clears it.
// Object size is 288 bytes.
#ifndef CAVER_PLAYERPROFILE_H
#define CAVER_PLAYERPROFILE_H

#include <map>
#include <string>
#include <vector>

#include <boost/shared_ptr.hpp>

namespace Caver {

namespace Proto {
class DateTime;
class GameState;
class PlayerProfile;
}

class GameData;
class GameState;
class PlayerProfile;

// DateTime is a thin wrapper over a double (seconds since the
// reference date): 8 bytes, trivially copyable — proven by
// DateTime::Now() returning in d0 and by the 8-byte slot at
// PlayerProfile+72 between the strings at +48 and +80. Only the
// surface PlayerProfile uses is declared here; the full recovery of
// this class belongs to its own slice.
class DateTime {
public:
    static DateTime Now();
    static DateTime DateWithSecondsSinceReferenceDate(double seconds);

    void LoadFromProtobufMessage(const Proto::DateTime& message);
    void SaveToProtobufMessage(Proto::DateTime* message);

    // Declared (not defined) here only so ProfilePanelView's MSVC
    // forwarder for these DateTime methods compiles in TUs that see
    // this mirror instead of the real DateTime.h; the definitions
    // live in DateTime.cpp. Signatures verbatim from DateTime.h.
    double SecondsSinceDate(const DateTime& other);
    std::string FormattedTimeIntervalSinceDate(const DateTime& other);

    double m_seconds;  // +0
};

// Delegate interface for profile events. Vtable slot order is proven
// by ProfileManager's vtable (0x62D8B0): WasSaved, CounterValueDid
// Change, DownloadCloudProfiles, GetAllProfiles,
// DeleteProfileWithIdentifier — and there is no virtual destructor
// slot. PlayerProfile itself only calls slots 0 and 1.
class IPlayerProfileDelegate {
public:
    virtual void PlayerProfileWasSaved(PlayerProfile* profile, bool saved) = 0;
    virtual void PlayerProfileCounterValueDidChange(PlayerProfile* profile, const std::string& counter, int oldValue, int newValue) = 0;
    virtual void DownloadCloudProfiles(const std::string& identifier, double delay) = 0;
    virtual void GetAllProfiles(std::vector<boost::shared_ptr<PlayerProfile> >* profiles) = 0;
    virtual void DeleteProfileWithIdentifier(const std::string& identifier) = 0;
};

class PlayerProfile {
public:
    PlayerProfile();
    ~PlayerProfile();

    void InitWithIdentifier(const std::string& identifier);
    bool ProfileExists();
    std::string LocalFilePath();
    void CreateProfile(const std::string& identifier);

    void LoadGameStateFromProtobufMessage(const Proto::GameState& message);
    void Save(bool notify);
    void DeleteProfile();
    bool Load();
    bool LoadFromPath(const std::string& path, bool loadGameState);
    bool LoadGameState();
    void SaveToProtobufMessage(Proto::PlayerProfile* message);
    std::string Filename();
    void LoadFromProtobufMessage(const Proto::PlayerProfile& message, bool loadGameState);
    void UpdateLastPlayedTime();

    std::string FormattedTimePlayed() const;
    static std::string FormattedTimePlayed(double timePlayed, bool includeSeconds);

    std::string currentLevelTitle();
    std::string equippedArmorName();
    std::string equippedWeaponName();
    std::string weaponTrinketName();
    std::string armorTrinketName();

    int ValueForCounter(const std::string& name);
    void SetValueForCounter(const std::string& name, int value);
    int IncreaseCounterValue(const std::string& name, int delta);

    IPlayerProfileDelegate* m_delegate;          // +0
    unsigned char* m_data;                       // +8
    unsigned int m_dataSize;                     // +16 (uninitialised by the ctor — see above)
    std::string m_identifier;                    // +24
    std::string m_name;                          // +48
    DateTime m_lastPlayedTime;                   // +72
    std::string m_currentLevelTitle;             // +80
    int m_level;                                 // +104
    float m_percentCompleted;                    // +108
    double m_timePlayed;                         // +112
    std::string m_equippedWeaponName;            // +120
    std::string m_equippedArmorName;             // +144
    std::string m_weaponTrinketName;             // +168
    std::string m_armorTrinketName;              // +192
    bool m_loaded;                               // +216
    boost::shared_ptr<GameData> m_gameData;      // +224
    boost::shared_ptr<GameState> m_gameState;    // +240
    std::map<std::string, int> m_counters;       // +256
    bool m_cheatEnabled;                         // +280
};

} // namespace Caver

#endif
