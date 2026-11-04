// Recovered from libswordigo.so (1.4.13) evidence — layout pinned by
// the ctor @0x4BF380 and Draw @0x4BF3F8 (jni-small SF-1):
//   +0   FontText m_text (240 bytes — the ctor's -1 stores land on
//        FontText's colour alpha at +19, shadow alpha at +44 and
//        previous-glyph index at +232, exactly FontText's ctor)
//   +240 Sprite m_sprite (88 bytes; Draw draws it from this+240)
// sizeof 328. Member names follow the Draw call targets; the
// Init arguments' roles are UNRECOVERED (the golden Init is a stub).
#ifndef CAVER_MAPANNOTATION_H
#define CAVER_MAPANNOTATION_H

#include <string>

#include "../core/Vector2.h"
#include "../gui/FontText.h"
#include "../render/Sprite.h"

namespace Caver {

class RenderingContext;

class MapAnnotation {
public:
    MapAnnotation();

    void Init(const std::string& text, const Vector2& position, const Vector2& size);
    void Draw(RenderingContext* context);

    FontText m_text;    // +0
    Sprite m_sprite;    // +240
};

} // namespace Caver

#endif
