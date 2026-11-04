// Recovered from libswordigo.so (1.4.13) evidence — MapNode
// ctor @0x47ADD4 (zeroes fields), LoadFromProtobufMessage @0x47AE18,
// SaveToProtobufMessage @0x47B0BC, AddPortal @0x47B028, HasPortalTo @0x47B3C4,
// Title @0x47B480, ExperienceLevel @0x47B4FC, MusicName @0x47B524.
//
// sizeof 0xA8 (168 bytes) is proven by:
//   operator new(0xA8u) in Map::LoadFromProtobufMessage @0x47790C (offset 434).
//
// Field layout:
//   +0   unsigned long m_index            (8 bytes, set in Map::Load)
//   +8   Vector2 m_position               (8 bytes, proto field 1)
//   +16  std::string m_name               (24 bytes, proto field 2: level_name)
//   +40  std::string m_title              (24 bytes, proto field 10: title)
//   +64  int m_type                       (4 bytes, proto field 4: type)
//   +68  char _pad68[4]                   (4 bytes)
//   +72  MapZone* m_zone / m_field72      (8 bytes, raw pointer to parent MapZone)
//   +80  std::vector<Portal> m_paths      (24 bytes, 40-byte element stride)
//   +104 bool m_flag104                   (1 byte, proto field 5: hidden)
//   +105 char _pad105[7]                  (7 bytes)
//   +112 MapNode* m_parent / m_field112   (8 bytes, raw parent link)
//   +120 int m_experienceLevel            (4 bytes, proto field 6: experience_level)
//   +124 bool m_flag124                   (1 byte, visited/scratch flag)
//   +125 char _pad125[3]                  (3 bytes)
//   +128 std::string m_musicName          (24 bytes, proto field 7: music)
//   +152 bool m_field152                  (1 byte, proto field 8: has_portal)
//   +153 char _pad153[3]                  (3 bytes)
//   +156 int m_field156                   (4 bytes, proto field 9: num_treasures)
//   +160 bool m_field160                  (1 byte, proto field 11: ignore_in_statistics)
//   +161 char _pad161[7]                  (7 bytes, pad to 168)
//
// Credits: Quantum Creeper / Aevora Labs.
#ifndef CAVER_MAPNODE_H
#define CAVER_MAPNODE_H

#include <string>
#include <vector>
#include <boost/shared_ptr.hpp>

#include "../core/Vector2.h"

namespace Caver {

class MapZone;

namespace Proto {
class MapNode;
}

class MapNode {
public:
    struct Portal {
        Portal()
            : m_name()
            , m_field24(0)
            , m_field28(0)
            , m_field32(false)
            , m_field33(false)
            , _pad34{0, 0, 0, 0, 0, 0}
        {
        }

        std::string m_name;   // +0
        int m_field24;        // +24 (direction: 1..8)
        int m_field28;        // +28 (pass direction)
        bool m_field32;       // +32 (ignore_in_node_positioning)
        bool m_field33;       // +33 (bidirectional link flag)
        char _pad34[6];       // +34
    };

    MapNode();
    ~MapNode();

    void LoadFromProtobufMessage(const Proto::MapNode& message);
    void SaveToProtobufMessage(Proto::MapNode* message) const;
    void AddPortal(const Portal& portal);
    bool HasPortalTo(const std::string& destination) const;
    std::string Title() const;
    int ExperienceLevel() const;
    std::string MusicName() const;

    unsigned long m_index;                // +0
    Vector2 m_position;                   // +8
    std::string m_name;                   // +16
    std::string m_title;                  // +40
    int m_type;                           // +64
    char _pad68[4];                       // +68
    union {
        MapZone* m_zone;                  // +72
        unsigned long long m_field72;     // +72
    };
    std::vector<Portal> m_paths;          // +80
    bool m_flag104;                       // +104
    char _pad105[7];                      // +105
    union {
        MapNode* m_parent;                // +112
        MapNode* m_field112;              // +112
    };
    int m_experienceLevel;                // +120
    bool m_flag124;                       // +124
    char _pad125[3];                      // +125
    std::string m_musicName;              // +128
    bool m_field152;                      // +152
    char _pad153[3];                      // +153
    int m_field156;                       // +156
    bool m_field160;                      // +160
    char _pad161[7];                      // +161
};

typedef MapNode::Portal MapPathElement;

// Golden aarch64 NDK-libc++ layout anchor; MSVC STL member sizes differ,
// so this assert is Android/Linux-only (the Windows build is self-consistent
// under MSVC STL and does not claim golden byte-layout parity).
#ifndef _MSC_VER
static_assert(sizeof(MapNode::Portal) == 40, "MapNode::Portal sizeof must be 40 bytes");
static_assert(sizeof(MapNode) == 0xA8, "MapNode sizeof must be 168 bytes");
#endif

} // namespace Caver

#endif // CAVER_MAPNODE_H
