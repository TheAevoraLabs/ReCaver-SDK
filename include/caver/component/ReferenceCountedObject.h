// Recovered from libswordigo.so (1.4.13) evidence — see docs/CORE_HEADERS.md.
// Base class for objects owned through boost::intrusive_ptr. Its typeinfo
// in the golden binary is a plain __class_type_info with no bases, and no
// out-of-line symbols exist for it anywhere in the binary: every member
// was inline in the original header. Component's RTTI names it as the
// primary (offset 0) base of Component.
// Layout: vptr +0, reference count +8. Size 16.
#ifndef CAVER_REFERENCECOUNTEDOBJECT_H
#define CAVER_REFERENCECOUNTEDOBJECT_H

namespace Caver {

class ReferenceCountedObject {
public:
    ReferenceCountedObject() : m_referenceCount(0) {}
    virtual ~ReferenceCountedObject() {}

    int m_referenceCount; // +8
};

// boost::intrusive_ptr support (ADL). Also inline-only in the golden
// binary (no symbols), so they lived in this header.
inline void intrusive_ptr_add_ref(ReferenceCountedObject* object)
{
    ++object->m_referenceCount;
}

inline void intrusive_ptr_release(ReferenceCountedObject* object)
{
    if (--object->m_referenceCount == 0)
        delete object;
}

} // namespace Caver

#endif
