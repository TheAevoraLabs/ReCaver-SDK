// Recovered from libswordigo.so (1.4.13) evidence — IDA/Hex-Rays exports
// for the ten Caver::GUIViewLayout functions plus golden dynsym analysis.
//
// GUIViewLayout is the serialisable layout record for one view in a
// controls layout (see Proto::GUIViewLayout in Common.proto: identifier,
// repeated subview, margins). GameOptions holds phone/pad controls
// layouts as shared_ptr<GUIViewLayout>; GameOverlayView applies a layout
// tree to live GUIViews.
//
// NON-POLYMORPHIC (proven): the golden exports no _ZTV/_ZTI/_ZTS for
// Caver::GUIViewLayout and only a D2 destructor symbol (no D1/D0 pair),
// so the class has no vtable and a non-virtual destructor. It is held by
// boost::shared_ptr (sp_counted_impl_p machinery present in the golden).
//
// Layout (sizeof 104 = 0x68, proven by operator new(0x68) in
// LoadFromProtobufMessage and LayoutForViewWithFrameInBounds, and by the
// copy-ctor/dtor member walks):
//   +0   std::string m_identifier                    (24 bytes)
//   +24  std::vector<boost::shared_ptr<GUIViewLayout>> m_subviews
//   +48  std::map<std::string, boost::shared_ptr<GUIViewLayout>>
//        m_subviewsByIdentifier                      (24-byte __tree)
//   +72  GUIMargins m_margins                        (20 bytes: anchor
//        flags at +72, floats at +76/+80/+84/+88 — left/right/bottom/top
//        in Proto::GUIMargins field order)
//   +92  float m_width   (frame width  captured by Update/LayoutFor)
//   +96  float m_height  (frame height captured by Update/LayoutFor)
//   +100 float m_scale   (ctor default 1.0f; divisor in Update..., the
//        scale argument stored by LayoutFor...; multiplier in
//        FrameForViewInBounds)
// Member names are unrecovered; offsets and types are binary-proven.
// The copy constructor and destructor are implicit in the original: both
// are WEAK symbols in the golden (copy ctor emitted in the
// GameOverlayView TU at 0x48CBD8, dtor at 0x48CDCC), i.e. no user-declared
// copy ctor/dtor exists in this header.
#ifndef CAVER_GUIVIEWLAYOUT_H
#define CAVER_GUIVIEWLAYOUT_H

#include <map>
#include <string>
#include <vector>

#include <boost/shared_ptr.hpp>

#include "../core/Rectangle.h"
#include "GUIMargins.h"

namespace Caver {

namespace Proto {
class GUIViewLayout;
} // namespace Proto

class GUIViewLayout {
public:
    GUIViewLayout();

    void LoadFromProtobufMessage(const Proto::GUIViewLayout& message);
    void SaveToProtobufMessage(Proto::GUIViewLayout* message) const;

    void AddSubview(const boost::shared_ptr<GUIViewLayout>& subview);
    boost::shared_ptr<GUIViewLayout> SubviewWithIdentifier(
        const std::string& identifier);

    Rectangle FrameForViewInBounds(const Rectangle& bounds);
    void UpdateWithViewWithFrameinBounds(const Rectangle& frame,
                                         const Rectangle& bounds);
    static boost::shared_ptr<GUIViewLayout> LayoutForViewWithFrameInBounds(
        const Rectangle& frame, const Rectangle& bounds, float scale);

    std::string m_identifier;                                        // +0
    std::vector<boost::shared_ptr<GUIViewLayout> > m_subviews;       // +24
    std::map<std::string, boost::shared_ptr<GUIViewLayout> >
        m_subviewsByIdentifier;                                      // +48
    GUIMargins m_margins;                                            // +72
    float m_width;                                                   // +92
    float m_height;                                                  // +96
    float m_scale;                                                   // +100
};

} // namespace Caver

#endif
