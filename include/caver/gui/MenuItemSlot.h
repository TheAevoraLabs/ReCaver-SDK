#ifndef CAVER_MENUITEMSLOT_H
#define CAVER_MENUITEMSLOT_H

#include <boost/shared_ptr.hpp>

#include "GUIView.h"
#include "GUITexturedRect.h"

namespace Caver {

class Item;
class Skill;
class MenuItemSlot;

class IMenuItemSlotDelegate {
public:
    virtual void MenuItemSlotSelected(MenuItemSlot* slot) = 0;
    virtual void MenuItemSlotDeselected(MenuItemSlot* slot) = 0;
    virtual ~IMenuItemSlotDelegate() {}
};

class MenuItemSlot : public GUIView {
public:
    MenuItemSlot();
    ~MenuItemSlot() override;

    void Init(float scale);
    void SetItem(const boost::shared_ptr<Item>& item);
    void SetSkill(const boost::shared_ptr<Skill>& skill, bool flag = false);
    void SetSelected(bool selected);

    void Update(float deltaTime) override;
    void DrawRect(RenderingContext* context, const Rectangle& rect,
                  const Matrix4& transform) override;
    void TouchBegan(const FWTouch& touch) override;

    IMenuItemSlotDelegate* delegate;          // +256
    float field264;                           // +264 (scale, default 1.0f)
    boost::shared_ptr<Item> m_item;           // +272
    boost::shared_ptr<Skill> m_skill;         // +288
    bool field304;                            // +304
    bool field305;                            // +305
    bool field306;                            // +306
    char _pad307[5];                          // +307
    GUITexturedRect m_rect1;                  // +312
    GUITexturedRect m_rect2;                  // +424
    GUITexturedRect m_rect3;                  // +536
    bool field648;                            // +648
    int field652;                             // +652
};

// Golden aarch64 NDK-libc++ layout anchor; MSVC STL member sizes differ,
// so this assert is Android/Linux-only (the Windows build is self-consistent
// under MSVC STL and does not claim golden byte-layout parity).
#ifndef _MSC_VER
static_assert(sizeof(MenuItemSlot) == 656, "MenuItemSlot sizeof must be 656");
#endif

} // namespace Caver

#endif // CAVER_MENUITEMSLOT_H
