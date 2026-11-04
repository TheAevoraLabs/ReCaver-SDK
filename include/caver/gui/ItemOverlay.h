#ifndef CAVER_ITEMOVERLAY_H
#define CAVER_ITEMOVERLAY_H

#include <map>
#include <string>
#include <vector>
#include <boost/shared_ptr.hpp>

#include "FontText.h"
#include "GUIView.h"
#include "../render/Sprite.h"

namespace Caver {

class GameState;
class Item;

class ItemOverlay : public GUIView {
public:
    ItemOverlay();
    ~ItemOverlay() override;

    void InitWithGameState(const boost::shared_ptr<GameState>& gameState);
    bool IsOverlayItem(const boost::shared_ptr<Item>& item);
    void UpdateItemCount(const boost::shared_ptr<Item>& item);
    void Update(float deltaTime) override;
    void UpdateSprites();
    void DrawRect(RenderingContext* context, const Rectangle& rect,
                  const Matrix4& transform) override;

private:
    boost::shared_ptr<GameState> m_gameState;           // +256
    bool m_active;                                      // +272
    char _pad273[7];                                    // +273..279
    std::vector<boost::shared_ptr<Item>> m_items;       // +280
    std::map<std::string, unsigned long> m_itemCounts;  // +304
    Sprite m_sprite1;                                   // +328
    Sprite m_sprite2;                                   // +416
    FontText m_fontText;                                // +504
};

static_assert(sizeof(ItemOverlay) == 744, "ItemOverlay size mismatch");

} // namespace Caver

#endif // CAVER_ITEMOVERLAY_H
