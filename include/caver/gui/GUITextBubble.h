// Recovered from libswordigo.so (1.4.13) evidence.
//
// GUITextBubble: a text bubble — a GUIRoundedRect frame, a peg sprite,
// an optional "finished" indicator sprite and a FontText — with
// typewriter-style progressive text display.
// NON-POLYMORPHIC: the golden exports no _ZTV/_ZTI/_ZTS for
// GUITextBubble (verified by symbol-table scan).
//
// Layout (168 bytes, proven by the ctor @0x531794, the destructor
// @0x345BB8 and every method's accesses; the string at +40 and the
// shown-character counter at +144 are cross-confirmed by
// GUIBubbleView::TouchBegan):
//   +0   Rectangle maxBounds (ctor: {-1.7e38, -1.7e38, FLT_MAX,
//        FLT_MAX} as stored float bit patterns 0xFEFFFFFF /
//        0x7F7FFFFF — an effectively unbounded default)
//   +16  Rectangle bounds (the laid-out bubble rect, written by
//        UpdateLayout; member name UNRECOVERED)
//   +32  bool layoutDirty (set by every setter, cleared after
//        UpdateLayout runs)
//   +40  std::string text
//   +64  boost::shared_ptr<GUIRoundedRect> frameRect
//   +80  boost::shared_ptr<Sprite> pegSprite (member name UNRECOVERED)
//   +96  boost::shared_ptr<Sprite> indicatorSprite (member name
//        UNRECOVERED)
//   +112 boost::shared_ptr<FontText> fontText
//   +128 Rectangle textInsets {11, 15, 11, 15} (member name
//        UNRECOVERED; consumed by UpdateLayout as left/top/right/
//        bottom insets around the text)
//   +144 long long shownCount (characters shown so far)
//   +152 float charTimer (time accumulated towards the next character)
//   +156 float charInterval (ctor: 0.05f — seconds per character)
//   +160 bool showsIndicator (SetShowsIndicatorWhenFinished)
//   +161 bool indicatorVisible (blink state, toggled every 0.5 s once
//        the text is fully shown)
//   +164 float blinkTimer (ctor stores the bit pattern 0x7F7FFFFF —
//        FLT_MAX — so the indicator blinks on the first Update)
#ifndef CAVER_GUITEXTBUBBLE_H
#define CAVER_GUITEXTBUBBLE_H

#include <boost/intrusive_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <string>

#include "../core/Color.h"
#include "../core/Rectangle.h"

namespace Caver {

class FontText;
class GUIRoundedRect;
class RenderingContext;
class Sprite;
class Texture;

class GUITextBubble {
public:
    GUITextBubble();
    ~GUITextBubble();

    void SetMaxBounds(const Rectangle& bounds);
    void SetFrameTexture(const boost::intrusive_ptr<Texture>& texture,
                         const Color& color);
    void SetPegTexture(const boost::intrusive_ptr<Texture>& texture,
                       const Color& color);
    void SetText(const std::string& text, const Color& color, float scale);
    void SetShowsIndicatorWhenFinished(bool shows);
    void FinishShowingText();
    void Update(float deltaTime);
    void Draw(RenderingContext* context);
    void UpdateLayoutIfNeeded();
    void UpdateLayout();

    Rectangle maxBounds;                        // +0
    Rectangle bounds;                           // +16 UNRECOVERED name
    bool layoutDirty;                           // +32
    char _pad33[7];                             // +33
    std::string text;                           // +40
    boost::shared_ptr<GUIRoundedRect> frameRect;  // +64
    boost::shared_ptr<Sprite> pegSprite;        // +80 UNRECOVERED name
    boost::shared_ptr<Sprite> indicatorSprite;  // +96 UNRECOVERED name
    boost::shared_ptr<FontText> fontText;       // +112
    Rectangle textInsets;                       // +128 UNRECOVERED name
    long long shownCount;                       // +144
    float charTimer;                            // +152
    float charInterval;                         // +156
    bool showsIndicator;                        // +160
    bool indicatorVisible;                      // +161
    char _pad162[2];                            // +162
    float blinkTimer;                           // +164
};

} // namespace Caver

#endif
