#ifndef CAVER_COINBAR_H
#define CAVER_COINBAR_H

#include <boost/shared_ptr.hpp>

#include "GUIAnimation.h"
#include "GUIView.h"
#include "GUILabel.h"
#include "../render/Sprite.h"

namespace Caver {

class CoinBar : public GUIView {
public:
    CoinBar();
    ~CoinBar() override;

    void LayoutSubviews() override;
    void Update(float deltaTime) override;
    void DrawRect(RenderingContext* context, const Rectangle& rect,
                  const Matrix4& transform) override;

    void CreateIconSprite();
    void SetCurrentCoins(int coins);
    void UpdateText();

    int currentCoins() const { return m_currentCoins; }
    int displayedCoins() const { return m_displayedCoins; }

private:
    int m_currentCoins;                      // golden +252, in-tree +256
    int m_displayedCoins;                    // golden +256, in-tree +260
    float m_timer;                           // golden +260, in-tree +264; golden ctor: FLT_MAX
    Sprite m_iconSprite;                     // golden +264, in-tree +272
    boost::shared_ptr<GUILabel> m_label;     // golden +352, in-tree +360
    float m_field368;                        // golden +368, in-tree +376; golden ctor: 1.0f, name UNRECOVERED
    boost::shared_ptr<GUIAnimation> m_animation;  // golden +376, in-tree +384, label pop animation
};

// Golden size is 0x188 (392). In-tree GUIView is 4 bytes larger than
// the golden base (256 vs 252), and the 8-aligned Sprite needs a
// further 4-byte pad after the three 4-byte ints, so members from the
// sprite onward sit +8 from their golden offsets and the in-tree size
// is 400. Member ORDER matches the golden; shrinking GUIView back is
// the gui slice's job, not this lane's (CoinBar.* only).
// Golden aarch64 NDK-libc++ layout anchor; MSVC STL member sizes differ,
// so this assert is Android/Linux-only (the Windows build is self-consistent
// under MSVC STL and does not claim golden byte-layout parity).
#ifndef _MSC_VER
static_assert(sizeof(CoinBar) == 400, "CoinBar must be 400 bytes in-tree (golden 392 + 4 GUIView drift + 4 alignment pad)");
#endif

} // namespace Caver

#endif // CAVER_COINBAR_H
