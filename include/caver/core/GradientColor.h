#ifndef CAVER_GRADIENTCOLOR_H
#define CAVER_GRADIENTCOLOR_H

#include "Color.h"
#include <boost/shared_ptr.hpp>

namespace Caver {

class GradientColor {
public:
    int m_count;
    float* m_positions;
    Color* m_colors;

    GradientColor(int count, float* positions, Color* colors);
    // Header-inline in the golden: no out-of-line
    // _ZN5Caver13GradientColorD2Ev exists in the census, and the
    // shared_ptr dispose body @0x376220 inlines this teardown
    // (delete[] the +16 colours, then delete[] the +8 positions).
    // The out-of-line definition that used to live in
    // GradientColor.cpp deleted them in the opposite order.
    ~GradientColor()
    {
        delete[] m_colors;
        delete[] m_positions;
    }

    boost::shared_ptr<GradientColor> GradientColorByReversingAlphaPremultiplication();
};

} // namespace Caver

#endif // CAVER_GRADIENTCOLOR_H
