// Recovered from libswordigo.so (1.4.13) evidence — Map
// ctor @0x4778E8, LoadFromProtobufMessage @0x47790C,
// UnflagAllNodes @0x478290, RecursivelySetNodePositions @0x4782FC,
// NodeForName @0x4784D8, SaveToProtobufMessage @0x478550, FindPath @0x478868.
//
// sizeof 0x48 (72 bytes).
// Field layout:
//   +0   std::vector<boost::shared_ptr<MapNode>> m_nodes    (24 bytes)
//   +24  std::map<std::string, boost::shared_ptr<MapNode>> m_nodeMap (24 bytes)
//   +48  std::map<std::string, boost::shared_ptr<MapZone>> m_zones   (24 bytes)
//
// Credits: Quantum Creeper / Aevora Labs.
#ifndef CAVER_MAP_H
#define CAVER_MAP_H

#include <string>
#include <map>
#include <vector>
#include <boost/shared_ptr.hpp>

#include "MapZone.h"
#include "MapNode.h"

namespace Caver {

class MapPath {
public:
    std::vector<std::string> m_names;  // +0
};

namespace Proto {
class Map;
}

class Map {
public:
    Map();
    ~Map();

    void LoadFromProtobufMessage(const Proto::Map& message);
    void SaveToProtobufMessage(Proto::Map* message) const;
    boost::shared_ptr<MapNode> NodeForName(const std::string& name);
    void UnflagAllNodes();
    boost::shared_ptr<MapPath> FindPath(const std::string& from, const std::string& to);
    void RecursivelySetNodePositions(MapNode* node);

    std::vector<boost::shared_ptr<MapNode> > m_nodes;                     // +0
    std::map<std::string, boost::shared_ptr<MapNode> > m_nodeMap;         // +24
    std::map<std::string, boost::shared_ptr<MapZone> > m_zones;           // +48
};

// Golden aarch64 NDK-libc++ layout anchor; MSVC STL member sizes differ,
// so this assert is Android/Linux-only (the Windows build is self-consistent
// under MSVC STL and does not claim golden byte-layout parity).
#ifndef _MSC_VER
static_assert(sizeof(Map) == 72, "Map sizeof must be 72 bytes");
#endif

} // namespace Caver

#endif // CAVER_MAP_H
