// Recovered from libswordigo.so (1.4.13) evidence — IDA 0x5309EC ctor.
// Layout (80 bytes, no vtable):
//   +0  boost::intrusive_ptr<Texture> (raw px; refcount lives in the
//       Texture at +8 — SetTexture 0x530A1C increments it, the
//       destructor releases it via the vtable slot at +8)
//   +8  Rectangle frame (SetFrame 0x530A84)
//   +24 int, initialised to -1, copied to +76 by Update
//   +32 std::list<boost::shared_ptr<GUIAnimation>> (sentinel at +32/+40,
//       size at +48; nodes are 32 bytes — AddAnimation 0x531030)
//   +56 bool, initialised to 1 (with +57 as one 0x0101 word store);
//       set by SetTexture/SetFrame, cleared by Update
//   +57 bool, initialised to 1; driven by Update
//   +60 Rectangle drawFrame (Update copies frame here)
//   +76 int (Update copies the +24 field here)
#ifndef CAVER_GUIANIMATEDSPRITE_H
#define CAVER_GUIANIMATEDSPRITE_H

#include "../core/Rectangle.h"
#include "../render/Texture.h"
#include <boost/intrusive_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <list>

namespace Caver {

class GUIAnimation;

class GUIAnimatedSprite {
public:
    GUIAnimatedSprite();
    ~GUIAnimatedSprite();

    void SetTexture(const boost::intrusive_ptr<Texture>& texture);
    void SetFrame(const Rectangle& frame);
    void Update(float deltaTime);
    void AddAnimation(const boost::shared_ptr<GUIAnimation>& animation);
    void RemoveAllAnimations();

    boost::intrusive_ptr<Texture> texture;                  // +0
    Rectangle frame;                                        // +8
    int field24;                                            // +24
    std::list<boost::shared_ptr<GUIAnimation> > animations; // +32
    bool frameDirty;                                        // +56
    bool animationsActive;                                  // +57
    Rectangle drawFrame;                                    // +60
    int field76;                                            // +76
};

} // namespace Caver

#endif
