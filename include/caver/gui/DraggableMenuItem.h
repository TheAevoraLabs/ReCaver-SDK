// Recovered from libswordigo.so (1.4.13) evidence.
//
// DraggableMenuItem: the icon that is handed to the drag-and-drop engine
// when the player drags an item out of the inventory trinket slot. It is
// a GUIDraggableItem that owns an embedded GUITexturedRect with the
// "item_<name>_nobg" texture.
//
// Vtable _ZTVN5Caver17DraggableMenuItemE @0x630440, 320 bytes: 40 slots,
// all GUIDraggableItem's, plus DrawRect (slot 10). Everything else is
// inherited, so the only override is DrawRect.
//
// Layout (sizeof 0x190, the operator new size in InventoryTrinketSlot's
// TouchMoved @0x4B479C) from the constructor @0x4AA868:
//   +0    GUIView (256 bytes, its vptr is the primary one)
//   +256  GUIDraggableItem::dropTargetView   (shared_ptr)
//   +272  boost::shared_ptr<Item> m_item     (InitWithItem stores it at
//         +272/+280, the destructor releases the +280 counted pointer)
//   +288  GUITexturedRect m_iconView         (ctor runs at this+288,
//         DrawRect calls Draw on this+288, InitWithTexture and
//         CreateWithBounds on this+0x120)
#ifndef CAVER_DRAGGABLEMENUITEM_H
#define CAVER_DRAGGABLEMENUITEM_H

#include <boost/shared_ptr.hpp>

#include "../core/Rectangle.h"
#include "GUITexturedRect.h"
#include "GUIDraggableItem.h"

namespace Caver {

class Item;
class RenderingContext;

class DraggableMenuItem : public GUIDraggableItem {
public:
    DraggableMenuItem();
    ~DraggableMenuItem() override;

    void InitWithItem(const boost::shared_ptr<Item>& item);
    void DrawRect(RenderingContext* context, const Rectangle& rect,
                  const Matrix4& transform) override;  // slot 10

    boost::shared_ptr<Item> m_item;   // +272
    GUITexturedRect m_iconView;       // +288
};

// Golden aarch64 NDK-libc++ layout anchor; MSVC STL member sizes differ,
// so this assert is Android/Linux-only (the Windows build is self-consistent
// under MSVC STL and does not claim golden byte-layout parity).
#ifndef _MSC_VER
static_assert(sizeof(DraggableMenuItem) == 0x190,
              "DraggableMenuItem sizeof must be 400 bytes");
#endif

} // namespace Caver

#endif // CAVER_DRAGGABLEMENUITEM_H
