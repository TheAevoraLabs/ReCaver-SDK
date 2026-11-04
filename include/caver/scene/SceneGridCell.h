// Recovered from libswordigo.so (1.4.13) evidence — layout pinned by
// AddEntry @0x4FE4F4, Resize @0x4FE5B0, RemoveEntry @0x4FE638
// (jni-small SF-1):
//   +0  SceneGridEntry** m_data (malloc'd, freed with delete[] —
//       the golden's allocator mismatch is original)
//   +8  int m_count
//   +12 int m_capacity
// Entries live at the END of the buffer: the occupied slots are
// indices [capacity-count, capacity), and AddEntry prepends by
// growing the free head room. sizeof 16.
#ifndef CAVER_SCENEGRIDCELL_H
#define CAVER_SCENEGRIDCELL_H

namespace Caver {

struct SceneGridEntry;

class SceneGridCell {
public:
    SceneGridCell() : m_data(nullptr), m_count(0), m_capacity(0) {}
    ~SceneGridCell() { delete[] m_data; }

    void AddEntry(SceneGridEntry* entry);
    void Resize(int newCapacity);
    void RemoveEntry(SceneGridEntry* entry);

    SceneGridEntry** m_data;  // +0
    int m_count;              // +8
    int m_capacity;           // +12
};

} // namespace Caver

#endif
