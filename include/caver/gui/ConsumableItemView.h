// Recovered from libswordigo.so (1.4.13) evidence — IDA/Hex-Rays exports
// in ~/workspace/analysis/swordigo/functions/Caver/ConsumableItemView/
// (ctor @0x482CBC, InitWithGameState @0x482FD4, SetItem @0x48324C,
// dtors @0x483894/@0x48396C), arbitrated against the Ghidra decompile
// in ~/workspace/analysis/ghidra64/g_64.c (Ghidra addrs +0x100000).
//
// ConsumableItemView: the HUD button showing the currently equipped
// consumable (the healing potion) — a direct GUIButton subclass (ctor
// calls GUIButton::GUIButton with type 0 and stores the
// ConsumableItemView vtable @0x62E518; the golden vtable's slot 0 is
// GUIButton::TouchBegan, i.e. no new virtual slots, only the dtors
// differ from GUIButton's). GameOverlayView owns one at +464 and its
// header stub (m_shared488) is this class's m_item member below —
// reconcile at merge: GameOverlayView.h's TU-local stub must be
// replaced by this header, and its ConsumableItemView::InitWithGameState
// declaration there is the same function recovered here.
//
// Layout (GUIButton = 472 bytes, then; every offset byte-verified
// against the ctor zero-store map, the dtor @0x483894 member-release
// order and the method bodies):
//   +472 boost::shared_ptr<GameState> m_gameState (assigned by
//        InitWithGameState; the GameState whose CharacterState at
//        +16 supplies the consumable list)
//   +488 boost::shared_ptr<Item> m_item (assigned by SetItem; its
//        emptiness gates this view's hidden flag in
//        GameOverlayView::SetControlsHidden)
// sizeof = 504 (0x1F8).
#ifndef CAVER_CONSUMABLEITEMVIEW_H
#define CAVER_CONSUMABLEITEMVIEW_H

#include <boost/shared_ptr.hpp>

#include "GUIButton.h"

namespace Caver {

class GameState;
class Item;

class ConsumableItemView : public GUIButton {
public:
    ConsumableItemView();
    ~ConsumableItemView() override;

    void InitWithGameState(const boost::shared_ptr<GameState>& gameState);
    void SetItem(const boost::shared_ptr<Item>& item);

    boost::shared_ptr<GameState> m_gameState;  // +472
    boost::shared_ptr<Item> m_item;            // +488
};

} // namespace Caver

#endif
