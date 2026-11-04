// CollisionPairSet — the set of reported collision pairs, owned by
// Scene at +272.
//
// Golden shape (RegisterCollision 0x3279E4, Purge 0x327D4C, no
// arguments): a libc++ std::map<CollisionPair, bool> whose keys are
// intrusive-pointer pairs (smaller pointer first) and whose bool value
// is the still-colliding flag. RegisterCollision sets the flag to 1
// (inserting when new) and returns whether the pair was newly
// inserted; Purge clears live flags and keeps those pairs, and sends
// message 8 (on_collision_end) to both components of flag-0 pairs
// before erasing them. The intrusive keys keep both components alive
// while the pair exists, so a colliding object removed via
// Scene::RemoveObject (which never erases pairs) cannot leave a
// dangling pair for the next Purge.
//
// The member below is a real std::map (24 bytes under libc++, exactly
// the Scene +272 slot size, enforced by the static_assert), so the
// Scene constructor constructs it and ~Scene destroys it at the same
// offset.
#ifndef CAVER_COLLISIONPAIRSET_H
#define CAVER_COLLISIONPAIRSET_H

#include <map>
#include <utility>

#include <boost/intrusive_ptr.hpp>

namespace Caver {

class CollisionShapeComponent;

// Golden key type of the pair map (std::__value_type<CollisionPair,
// bool> in the golden's map instantiation): two intrusive references
// to the colliding components, smaller pointer first (golden
// RegisterCollision @0x3279E4 orders them that way, as does the
// tree). Golden ~CollisionPair @0x327CCC releases the second
// member, then the first — exactly the implicit dtor order of this
// struct (reverse declaration order). The earlier
// std::pair<intrusive_ptr, intrusive_ptr> key behaved the same but
// was not the golden type.
struct CollisionPair {
    boost::intrusive_ptr<CollisionShapeComponent> first;
    boost::intrusive_ptr<CollisionShapeComponent> second;

    bool operator<(const CollisionPair& other) const
    {
        if (first != other.first)
            return first < other.first;
        return second < other.second;
    }
};

class CollisionPairSet {
public:
    bool RegisterCollision(CollisionShapeComponent* first, CollisionShapeComponent* second);
    // End-of-frame dispatch (golden 0x327D4C, no arguments): every
    // pair whose still-colliding flag is 0 has ended, so both owners
    // (guarded non-null, as golden guards them) get message 8 with
    // the other shape as data, dispatched BEFORE the pair is erased;
    // flag-1 pairs are kept with the flag cleared for the next frame.
    // Purge is skipped by the caller when deltaTime <= 0.001 or the
    // scene is paused, so a pair that separates during a skipped
    // frame keeps its flag until the next Purge runs, exactly as in
    // the golden.
    void Purge();

private:
    typedef CollisionPair PairKey;
    std::map<PairKey, bool> m_pairs;
};

// Golden aarch64 NDK-libc++ layout anchor; MSVC STL and GNU libstdc++ member sizes differ,
// so this assert is libc++-only (the Windows and GCC builds are self-consistent
// under their respective STLs and do not claim golden byte-layout parity).
#if defined(_LIBCPP_VERSION)
static_assert(sizeof(CollisionPairSet) == 24,
              "CollisionPairSet must stay 24 bytes (Scene +272 slot)");
#endif

} // namespace Caver

#endif
