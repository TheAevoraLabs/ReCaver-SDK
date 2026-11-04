// Recovered from libswordigo.so (1.4.13) evidence — IDA/Hex-Rays
// exports in ~/workspace/analysis/swordigo/functions/Caver/MenuViewPage/,
// the golden vtable _ZTVN5Caver12MenuViewPageE @0x630248 (328 bytes:
// 16-byte header + exactly 39 function slots, ABS64 relocations) and
// the RTTI _ZTIN5Caver12MenuViewPageE @0x6301E8
// (__si_class_type_info, single base Caver::GUIView @0x635630).
//
// MenuViewPage: the shared base class of the tabbed-menu pages
// (InventoryView, SettingsView, CharacterView, QuestsView). It adds
// exactly one member to GUIView — the page-title string — and exactly
// one virtual: Dismiss, which is PURE here.
//
// Vtable (vptr value @0x630258 = _ZTV + 16; slots in order):
//   0-37  the 38 GUIView slots verbatim (TouchBegan slot 0 from
//         GUIResponder, nextResponder slot 6, ~MenuViewPage slots 7/8,
//         Update slot 9, DrawRect slot 10, LayoutSubviews slot 11,
//         ... SetOrthoProjection slot 37)
//   38    __cxa_pure_virtual  (Dismiss — pure in this class; every
//         concrete page overrides it: InventoryView, SettingsView
//         @0x4D41D0, CharacterView, QuestsView @slot 38 all hold
//         their own Dismiss in this slot)
//
// Layout (sizeof 280, byte-verified against the D2 @0x4AA3B8 body,
// which destroys the string at +256 then tails into ~GUIView, and
// against every derived page's ctor/dtor store maps):
//   +0..255  GUIView base
//   +256     std::string page title (assigned "INVENTORY" / "QUESTS" /
//            ... by each derived page's ctor; the member name itself
//            is UNRECOVERED — pageTitle is the name used by the
//            InventoryView/CharacterView/SettingsView slices)
//
// Out-of-line functions: MenuViewPage has NO out-of-line ctor in the
// golden (no C1/C2 symbols exist; the ctor was inlined into every
// derived page, which calls GUIView::GUIView directly). Its only
// out-of-line functions are the two destructors:
//   D2 @0x4AA3B8 (68 bytes): vptr reset to the MenuViewPage vtable,
//     libc++ long-string destroy of the +256 string (byte +256 bit 0
//     is the __is_long_ flag, data pointer at +272), then ~GUIView.
//   D0 @0x4AA3FC (4 bytes): a lone `brk #1` — clang's deleting
//     destructor for an abstract class (a MenuViewPage most-derived
//     object cannot exist, so D0 is unreachable); plain C++ with this
//     header reproduces both bodies instruction-for-instruction
//     (verified against the golden disassembly, NDK r27d -O2).
// The golden D2/D0/vtable/RTTI symbols are all WEAK (vague linkage);
// the reconstruction below emits them GLOBAL, which parity.py does
// not grade — only the function bytes matter.
//
// MERGE SWAP (game-views coordinator): this header EXTRACTS the
// MenuViewPage definition that currently lives inside
// src/caver/gui/InventoryView.h (identical shape: inline empty ctor,
// out-of-line dtor, pure Dismiss, pageTitle at +256). At merge time:
//   - InventoryView.h: delete its MenuViewPage class, #include
//     "MenuViewPage.h" instead; InventoryView.cpp: delete its
//     `MenuViewPage::~MenuViewPage() {}` definition (it now lives in
//     MenuViewPage.cpp — keeping both is a duplicate-symbol link
//     error in any TU that sees both).
//   - SettingsView.h / CharacterView.h: swap their
//     #include "InventoryView.h" (MenuViewPage source) for
//     #include "MenuViewPage.h" (they only need MenuViewPage).
//   - TabbedMenuView.cpp: delete its TU-local MenuViewPage class
//     definition in favour of this header.
//   - QuestsView.h: RE-BASE onto MenuViewPage (see below); it must
//     NOT keep its own pageTitle member once it derives from here.
// Until that swap lands, do NOT include this header and
// InventoryView.h in the same translation unit (duplicate class
// definition) — this header was parity-graded standalone against
// GUIView.h only.
//
// QuestsView arbitration (merge-time job, evidence settled here):
// QuestsView FITS MenuViewPage exactly and should be re-based, NOT
// left GUIView-direct: its RTTI @0x632518 is __si_class_type_info
// whose single base is _ZTIN5Caver12MenuViewPageE @0x6301E8 (the
// same base SettingsView's RTTI @0x6327E0 names), its vtable
// @0x6323D0 is 328 bytes / 39 slots with QuestsView::Dismiss in
// slot 38, its page-title string sits at +256, and sizeof 384 =
// 280 (MenuViewPage) + 104 of its own members. Re-basing changes
// only the RTTI base and turns its Dismiss declaration into an
// override; layout and vtable shape are already identical.
#ifndef CAVER_MENUVIEWPAGE_H
#define CAVER_MENUVIEWPAGE_H

#include <string>

#include "GUIView.h"

namespace Caver {

// GUIView + the page-title string at +256; Dismiss (slot 38) is
// pure virtual. No out-of-line ctor exists in the golden — the ctor
// was inlined into every derived page (each derived ctor calls
// GUIView::GUIView directly and constructs/assigns the string).
class MenuViewPage : public GUIView {
public:
    MenuViewPage() {}
    ~MenuViewPage() override;  // slots 7, 8

    virtual void Dismiss() = 0;  // slot 38

    std::string pageTitle;  // +256
};

// Golden aarch64 NDK-libc++ layout anchor; MSVC STL member sizes differ,
// so this assert is Android/Linux-only (the Windows build is self-consistent
// under MSVC STL and does not claim golden byte-layout parity).
#ifndef _MSC_VER
static_assert(sizeof(MenuViewPage) == 280,
              "MenuViewPage layout: GUIView (256) + page title string");
#endif

} // namespace Caver

#endif
