// Recovered from libswordigo.so (1.4.13) evidence — IDA 0x446E68 / 0x52EC1C.
// Layout: a boost::weak_ptr<void> at +0 (Send locks it: CAS loop on the
// counted-base use count at +8, px at +0; the destructor releases the
// weak count at counted-base +12) followed by a
// boost::function<void(void*, GUIEvent*)> at +16 (its destructor logic
// and its storage at +24 appear verbatim in ~GUITargetAction / Send).
#ifndef CAVER_GUITARGETACTION_H
#define CAVER_GUITARGETACTION_H

#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

namespace Caver {

class GUIEvent;

class GUITargetAction {
public:
    boost::weak_ptr<void> target;                       // +0
    boost::function<void(void*, GUIEvent*)> action;      // +16

    ~GUITargetAction();
    void Send(void* sender, const GUIEvent& event);
};

} // namespace Caver

#endif
