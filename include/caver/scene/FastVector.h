// Partial recovery — Caver::FastVector<T>, a minimal growable array
// used for group membership and scene object lists. Layout recovered
// from SceneObject::IsInHiddenGroup / HasGroup / RemoveGroup in
// libswordigo.so (1.4.13): element count (int) at +0, element array
// pointer at +8. The +4 field (capacity) is inferred, not yet proven.
#ifndef CAVER_FASTVECTOR_H
#define CAVER_FASTVECTOR_H

#include <string>

namespace Caver {

class SceneObjectGroup;

template <typename T>
class FastVector {
public:
    // Frees the element buffer (proven by Caver::SceneObjectGroup's
    // destructor: after its RemoveAllObjects body, the buffer is
    // delete[]'d in member-teardown position, before the +56
    // shared_ptr member is released). Added by the core slice.
    ~FastVector() { delete[] m_items; }

    // Zeroes count/capacity/items (proven by SceneObjectGroup's ctor,
    // which zeroes the whole +72..+88 span with vector stores).
    // The non-trivial destructor also explains why following members
    // pack into this class's tail padding in the golden layouts.
    // Added by the core slice.
    FastVector() : m_count(0), m_capacity(0), m_items(nullptr) {}

    void RemoveObject(const T& item) {
        for (int i = 0; i < m_count; ++i) {
            if (m_items[i] == item) {
                for (int j = i; j < m_count - 1; ++j) {
                    m_items[j] = m_items[j + 1];
                }
                --m_count;
                return;
            }
        }
    }

    void InsertObjectAtIndex(int index, const T& item) {
        if (m_count >= m_capacity) {
            int newCapacity = 2 * m_capacity <= 10 ? 10 : 2 * m_capacity;
            if (m_count > newCapacity)
                m_count = newCapacity;
            if (m_capacity != newCapacity) {
                T* newItems = new T[newCapacity];
                for (int i = 0; i < m_count; ++i)
                    newItems[i] = m_items[i];
                delete[] m_items;
                m_items = newItems;
                m_capacity = newCapacity;
            }
        }
        if (index < 0) index = 0;
        if (index > m_count) index = m_count;
        for (int i = m_count; i > index; --i) {
            m_items[i] = m_items[i - 1];
        }
        m_items[index] = item;
        ++m_count;
    }

    void AddObject(const T& item) {
        if (m_count >= m_capacity) {
            int newCapacity = 2 * m_capacity <= 10 ? 10 : 2 * m_capacity;
            if (m_count > newCapacity)
                m_count = newCapacity;
            if (m_capacity != newCapacity) {
                T* newItems = new T[newCapacity];
                for (int i = 0; i < m_count; ++i)
                    newItems[i] = m_items[i];
                delete[] m_items;
                m_items = newItems;
                m_capacity = newCapacity;
            }
        }
        m_items[m_count] = item;
        ++m_count;
    }

    void Resize(int newCapacity) {
        if (m_count > newCapacity)
            m_count = newCapacity;
        if (newCapacity != 0) {
            if (m_capacity != newCapacity) {
                T* newItems = new T[newCapacity];
                if (m_items) {
                    for (int i = 0; i < m_count; ++i)
                        newItems[i] = m_items[i];
                    delete[] m_items;
                }
                m_capacity = newCapacity;
                m_items = newItems;
            }
        } else {
            if (m_items) {
                delete[] m_items;
                m_items = nullptr;
            }
            m_capacity = 0;
        }
    }

    int m_count;    // +0
    int m_capacity; // +4
    T* m_items;     // +8
};

} // namespace Caver

#endif
