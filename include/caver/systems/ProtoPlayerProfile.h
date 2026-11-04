// LAYOUT STUBS for the protobuf-generated classes Caver::PlayerProfile
// serialises through (Caver::Proto::PlayerProfile and friends),
// recovered from libswordigo.so (1.4.13) evidence: the bodies of
// Caver::PlayerProfile::SaveToProtobufMessage (0x47C5E0) and
// Caver::PlayerProfile::LoadFromProtobufMessage (0x47CBA8), the
// generated classes' own New() allocation sizes, and the stack
// objects in CreateProfile (0x47BC60) / LoadGameStateFromProtobufMessage
// (0x47BD90). Only the fields those functions touch are modelled, at
// their true offsets. The generated-proto work owns the real classes;
// these stubs exist so the systems slice can build and parity-grade
// PlayerProfile now (same pattern as ProtoItem.h).
//
// Proven sizes: Proto::PlayerProfile 176 (stack object in Save),
// Proto::PlayerProfile_Counter 32 (operator new(0x20) in Save's
// add-counters path), Proto::GameState 288 (operator new(0x120) for
// the game_state field; stack object in CreateProfile),
// Proto::Map 80 (stack object in LoadGameStateFromProtobufMessage),
// Proto::GameData 304 (New() allocation), Proto::DateTime 32
// (operator new(0x20) for the last_played field).
//
// Proto::PlayerProfile field offsets (Save/Load evidence):
//   name_ +16, last_played_ +24, current_level_title_ +32,
//   level_ +40, percent_completed_ +44, time_played_ +48,
//   game_state_ +56, equipped_weapon_name_ +64,
//   equipped_armor_name_ +72, weapon_trinket_name_ +80,
//   armor_trinket_name_ +88, counters_ RepeatedPtrField +96
//   (elements +96, current_size +104, total_size +108,
//   allocated_size +112), cheat_enabled_ +152, identifier_ +160,
//   _has_bits_ +168.
//   The +116..+151 range is NOT touched by PlayerProfile's Load/Save
//   and is unrecovered here (the tail of the counters_ RepeatedPtrField;
//   the generated class's counter_ spans +96..+151).
// Has bits (in Save order): name 0x1, last_played 0x2,
//   current_level_title 0x4, level 0x8, percent_completed 0x10,
//   time_played 0x20, game_state 0x40, equipped_weapon_name 0x80,
//   equipped_armor_name 0x100, weapon_trinket_name 0x200,
//   armor_trinket_name 0x400, cheat_enabled 0x1000, identifier 0x2000.
#ifndef CAVER_PROTO_PLAYERPROFILE_STUB_H
#define CAVER_PROTO_PLAYERPROFILE_STUB_H

#include <string>

#include <google/protobuf/message_lite.h>

// Forward declaration for the friend declarations below (the Reserve
// friend in google::protobuf::internal and the static friends in
// Caver::Proto): a qualified friend requires the class to already be
// declared, and this header is included before PlayerProfile.h in
// some TUs (see ProtoHealthComponent.h for the full note).
namespace Caver {
class PlayerProfile;
} // namespace Caver

namespace google {
namespace protobuf {
namespace internal {

// Only the Reserve entry point used by the inlined generated
// add_counters() path in PlayerProfile::SaveToProtobufMessage.
// Layout-compatible base pointer: the RepeatedPtrFieldBase for
// counters_ begins at Proto::PlayerProfile +96.
class RepeatedPtrFieldBase {
protected:
    // Protected to match the generated protobuf header: MSVC mangles
    // member access, so the public declaration referenced a symbol
    // nothing defines. PlayerProfile is a friend so its inlined
    // add_counters() path keeps calling Reserve.
    friend class ::Caver::PlayerProfile;
    void Reserve(int new_size);
};

} // namespace internal
} // namespace protobuf
} // namespace google

namespace Caver {
namespace Proto {

class DateTime : public google::protobuf::MessageLite {
public:
    DateTime();
    ~DateTime();

    std::string GetTypeName() const;
    DateTime* New() const;
    void Clear();
    bool IsInitialized() const;
    void CheckTypeAndMergeFrom(const google::protobuf::MessageLite& other);
    bool MergePartialFromCodedStream(google::protobuf::io::CodedInputStream* input);
    void SerializeWithCachedSizes(google::protobuf::io::CodedOutputStream* output) const;
    int ByteSize() const;
    int GetCachedSize() const;
private:
    char _bytes[32 - 8];
};

class GameState : public google::protobuf::MessageLite {
public:
    GameState();
    ~GameState();

    std::string GetTypeName() const;
    GameState* New() const;
    void Clear();
    bool IsInitialized() const;
    void CheckTypeAndMergeFrom(const google::protobuf::MessageLite& other);
    bool MergePartialFromCodedStream(google::protobuf::io::CodedInputStream* input);
    void SerializeWithCachedSizes(google::protobuf::io::CodedOutputStream* output) const;
    int ByteSize() const;
    int GetCachedSize() const;
private:
    char _bytes[288 - 8];
};

class Map : public google::protobuf::MessageLite {
public:
    Map();
    ~Map();

    std::string GetTypeName() const;
    Map* New() const;
    void Clear();
    bool IsInitialized() const;
    void CheckTypeAndMergeFrom(const google::protobuf::MessageLite& other);
    bool MergePartialFromCodedStream(google::protobuf::io::CodedInputStream* input);
    void SerializeWithCachedSizes(google::protobuf::io::CodedOutputStream* output) const;
    int ByteSize() const;
    int GetCachedSize() const;
private:
    char _bytes[80 - 8];
};

class GameData : public google::protobuf::MessageLite {
public:
    GameData();
    ~GameData();

    std::string GetTypeName() const;
    GameData* New() const;
    void Clear();
    bool IsInitialized() const;
    void CheckTypeAndMergeFrom(const google::protobuf::MessageLite& other);
    bool MergePartialFromCodedStream(google::protobuf::io::CodedInputStream* input);
    void SerializeWithCachedSizes(google::protobuf::io::CodedOutputStream* output) const;
    int ByteSize() const;
    int GetCachedSize() const;
private:
    char _bytes[304 - 8];
};

class PlayerProfile_Counter : public google::protobuf::MessageLite {
public:
    PlayerProfile_Counter();
    ~PlayerProfile_Counter();

    int _cached_size_;       // +8
    int _pad12;              // +12
    std::string* name_;      // +16
    int value_;              // +24
    int _has_bits_;          // +28

private:
    friend class ::Caver::PlayerProfile;
    static const std::string _default_name_;

public:
    std::string GetTypeName() const;
    PlayerProfile_Counter* New() const;
    void Clear();
    bool IsInitialized() const;
    void CheckTypeAndMergeFrom(const google::protobuf::MessageLite& other);
    bool MergePartialFromCodedStream(google::protobuf::io::CodedInputStream* input);
    void SerializeWithCachedSizes(google::protobuf::io::CodedOutputStream* output) const;
    int ByteSize() const;
    int GetCachedSize() const;
};

class PlayerProfile : public google::protobuf::MessageLite {
public:
    PlayerProfile();
    ~PlayerProfile();

    int _cached_size_;                          // +8
    int _pad12;                                 // +12
    std::string* name_;                         // +16
    DateTime* last_played_;                     // +24
    std::string* current_level_title_;          // +32
    int level_;                                 // +40
    float percent_completed_;                   // +44
    double time_played_;                        // +48
    GameState* game_state_;                     // +56
    std::string* equipped_weapon_name_;         // +64
    std::string* equipped_armor_name_;          // +72
    std::string* weapon_trinket_name_;          // +80
    std::string* armor_trinket_name_;           // +88
    PlayerProfile_Counter** counters_elements_; // +96
    int counters_current_size_;                 // +104
    int counters_total_size_;                   // +108
    int counters_allocated_size_;               // +112 (ends +116)
    // 36 bytes, not 32: allocated_size_ ends at +116, so a pad counted
    // from +120 starts at +116 here and drops the bool to +148 — four
    // bytes off the golden (SaveToProtobufMessage stores profile +280
    // at message +152, and the generated class agrees). That mismatch
    // wrote the save's cheat flag to the wrong byte and read it back
    // from inside counters_, which is why enablecheatpassu never
    // persisted and the map randomly came up unlocked.
    char _pad116[36];                           // +116..+151 (UNRECOVERED)
    bool cheat_enabled_;                        // +152 (proto field 13)
    char _pad153[7];                            // +153..+159
    std::string* identifier_;                   // +160
    int _has_bits_;                             // +168
    int _pad172;                                // +172

private:
    // Private and const-correct to match the generated .pb class
    // exactly (MSVC mangles static access and const-ness; Itanium
    // ignores both). The component class is a friend so its Load/Save
    // methods keep accessing these.
    friend class ::Caver::PlayerProfile;
    static const std::string _default_identifier_;
    static const std::string _default_name_;
    static const std::string _default_current_level_title_;
    static const std::string _default_equipped_weapon_name_;
    static const std::string _default_equipped_armor_name_;
    static const std::string _default_weapon_trinket_name_;
    static const std::string _default_armor_trinket_name_;
    static PlayerProfile* default_instance_;

public:

    std::string GetTypeName() const;
    PlayerProfile* New() const;
    void Clear();
    bool IsInitialized() const;
    void CheckTypeAndMergeFrom(const google::protobuf::MessageLite& other);
    bool MergePartialFromCodedStream(google::protobuf::io::CodedInputStream* input);
    void SerializeWithCachedSizes(google::protobuf::io::CodedOutputStream* output) const;
    int ByteSize() const;
    int GetCachedSize() const;
};

// Save (stub stores) and load (generated parser) must agree byte for
// byte or a profile field round-trips through the wrong offset. This
// class derives from MessageLite (virtual), so it is not standard-layout
// and clang's pedantic offsetof check fires on the member access even
// though pinning an offset this way is the point; libc++ wraps its own
// datasizeof offsetof uses the same way. The assertions stay active.
#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Winvalid-offsetof"
#elif defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Winvalid-offsetof"
#endif
static_assert(__builtin_offsetof(PlayerProfile, cheat_enabled_) == 152,
              "cheat_enabled_ must sit at message +152");
static_assert(__builtin_offsetof(PlayerProfile, identifier_) == 160,
              "identifier_ must sit at message +160");
static_assert(__builtin_offsetof(PlayerProfile, _has_bits_) == 168,
              "_has_bits_ must sit at message +168");
static_assert(sizeof(PlayerProfile) == 176,
              "Proto::PlayerProfile must stay 176 bytes");
#if defined(__clang__)
#pragma clang diagnostic pop
#elif defined(__GNUC__)
#pragma GCC diagnostic pop
#endif

} // namespace Proto
} // namespace Caver

#endif
