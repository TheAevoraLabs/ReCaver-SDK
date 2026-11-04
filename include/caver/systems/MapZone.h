// Recovered from libswordigo.so (1.4.13) evidence (SF-2, jni-small
// lane): ctor 0x47B864 (28B, zeroes the whole object), dtor 0x479C94
// (236B, member teardown in reverse declaration order), Load
// 0x47B880, Save 0x47B8CC.
//
// Layout (sizeof 104):
//   +0   std::string name
//   +24  std::string title
//   +48  std::vector<boost::shared_ptr<MapNode>> nodes — the dtor
//        walks it backwards releasing 16-byte shared_ptr elements;
//        neither Load nor Save touches it (bug-for-bug: nodes are
//        populated elsewhere)
//   +72  int experience level (proto field 4)
//   +80  std::string music (proto field 5)
#ifndef CAVER_MAPZONE_H
#define CAVER_MAPZONE_H

#include <boost/shared_ptr.hpp>
#include <string>
#include <vector>

namespace Caver {

class MapNode;

namespace Proto {
class MapZone;
}

class MapZone {
public:
    MapZone();
    ~MapZone();

    void LoadFromProtobufMessage(const Proto::MapZone& message);
    void SaveToProtobufMessage(Proto::MapZone* message) const;

    std::string m_name;                                    // +0
    std::string m_title;                                   // +24
    std::vector<boost::shared_ptr<MapNode> > m_nodes;      // +48
    int m_experienceLevel;                                 // +72
    std::string m_music;                                   // +80
};

} // namespace Caver

#endif
