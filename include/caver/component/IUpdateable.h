// Recovered from libswordigo.so (1.4.13) evidence — see docs/CORE_HEADERS.md.
// Per-frame update interface. Component's RTTI names it as the offset-24
// base; its vtable (recovered from the secondary vtable inside
// Component's _ZTV, in slot order) has exactly the two methods below
// and no destructor entries.
#ifndef CAVER_IUPDATEABLE_H
#define CAVER_IUPDATEABLE_H

namespace Caver {

class IUpdateable {
public:
    // Interface token, registered with Scene's second ComponentManager
    // by Scene's constructor (golden function at 0x4F5DB4, returns its
    // own address like the component Interface() functions). Declared
    // by the core slice 2026-10-24; the body is the components slice's.
    static long Interface();

    virtual bool UpdateWhenPaused() const = 0;
    virtual void Update(float deltaTime) = 0;
};

} // namespace Caver

#endif
