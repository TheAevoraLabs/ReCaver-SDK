// Recovered from libswordigo.so (1.4.13) evidence.
// Layout (24 bytes): type tag at +0 (0 = none, 1 = float, 2 = Vector2,
// 3 = Rectangle — proven by ValueWithFloat/Vector2/Rectangle at
// 0x42B2FC/0x490828/0x4A99F8, which write 1/2/3 there), then a
// boost::shared_ptr<void> at +8 holding a heap copy of the value: each
// ValueWith* allocates the value, wraps it in a typed
// sp_counted_impl_p (distinct vtables per type) and stores px at +8 /
// counted-base at +16 — exactly a shared_ptr<void> converted from
// shared_ptr<float> / shared_ptr<Vector2> / shared_ptr<Rectangle>.
#ifndef CAVER_GUIPROPERTYVALUE_H
#define CAVER_GUIPROPERTYVALUE_H

#include "../core/Vector2.h"
#include "../core/Rectangle.h"
#include <boost/shared_ptr.hpp>

namespace Caver {

class GUIPropertyValue {
public:
    int type;                    // +0
    boost::shared_ptr<void> value; // +8

    GUIPropertyValue() : type(0) {}
    ~GUIPropertyValue() {}

    static GUIPropertyValue ValueWithFloat(float value);
    static GUIPropertyValue ValueWithVector2(const Vector2& value);
    static GUIPropertyValue ValueWithRectangle(const Rectangle& value);
    static GUIPropertyValue InterpolatedValue(const GUIPropertyValue& a,
                                              const GUIPropertyValue& b,
                                              float t);
};

} // namespace Caver

#endif
