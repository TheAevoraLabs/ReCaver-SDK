// Recovered from libswordigo.so (1.4.13) evidence — IDA/Hex-Rays
// exports in ~/workspace/analysis/swordigo/functions/Caver/QuestsView/,
// the golden vtable _ZTVN5Caver10QuestsViewE @0x6323D0 (ABS64
// relocations) and the ctor / dtor bodies.
//
// QuestsView: the quests page of the tabbed menu — a scroll view of
// quest title labels (one per completed / in-progress quest) plus, for
// every active QuestText, a framed "text bubble" (border + peg frame
// views) holding the quest's quoted text lines grouped by quest name.
//
// Derivation (binary-proven): the golden RTTI _ZTIN5Caver10QuestsViewE
// @0x632518 is a __si_class_type_info on Caver::MenuViewPage, and the
// vtable has 39 slots — the 38 GUIView slots (LayoutSubviews, slot 11,
// overridden) plus slot 38 = Dismiss, the virtual MenuViewPage adds.
// MenuViewPage itself is GUIView + the std::string at +256 (its D2
// @0x4AA3B8 is byte-for-byte the tail of QuestsView's D2: destroy the
// +256 string, then ~GUIView), and QuestsView's ctor calls
// GUIView::GUIView directly with the MenuViewPage part inlined.
// HAND-OFF: no MenuViewPage.h exists in the tree yet; until the
// game-views coordinator lands it, this header derives directly from
// GUIView and declares Dismiss itself (identical vtable shape and
// member layout; only the RTTI base differs).
//
// Layout (sizeof 384, byte-verified against the ctor @0x4CD690 store
// map, the dtor @0x4CF72C teardown order — reverse declaration order —
// and every method body's accesses):
//   +0..255  GUIView base
//   +256     std::string page title (MenuViewPage member in the
//            golden; the ctor assigns "QUESTS")
//   +280     boost::shared_ptr<GameState> gameState
//   +296     boost::shared_ptr<GUIScrollView> scrollView
//   +312     std::vector<QuestState*> quests (filled by
//            GameState::GetAllCompletedQuests then
//            GetAllQuestsInProgress, both appending)
//   +336     std::map<std::string, std::vector<LabelGroup> >
//            labelGroups (keyed by quest name)
//   +360     std::map<std::string, boost::shared_ptr<GUILabel> >
//            labels (keyed by quest name)
// Member NAMES are behaviour-derived and UNRECOVERED.
//
// LabelGroup (sizeof 56, proven by the vector stride in
// InitWithGameState / LayoutSubviews and the dtor @0x4CEC78 teardown:
// labels vector, then the +40 shared_ptr, then the +24 shared_ptr):
//   +0   std::vector<boost::shared_ptr<GUILabel> > labels
//   +24  boost::shared_ptr<GUIFrameView> border (the
//        "textbubble_border" frame; the scroll view's subview, and the
//        labels' superview)
//   +40  boost::shared_ptr<GUIFrameView> peg (the "textbubble_peg"
//        frame; a subview of border, its `centered` flag flipped by
//        LayoutSubviews on alternating groups)
#ifndef CAVER_QUESTSVIEW_H
#define CAVER_QUESTSVIEW_H

#include <boost/shared_ptr.hpp>
#include <map>
#include <string>
#include <vector>

#include "MenuViewPage.h"

namespace Caver {

class GameState;
class GUIFrameView;
class GUILabel;
class GUIScrollView;
class QuestState;

class QuestsView : public MenuViewPage {
public:
    struct LabelGroup {
        std::vector<boost::shared_ptr<GUILabel> > labels;  // +0
        boost::shared_ptr<GUIFrameView> border;            // +24
        boost::shared_ptr<GUIFrameView> peg;               // +40

        // Header-defined in the golden (the D2 @0x4CEC78 is a weak
        // symbol and InitWithGameState calls it out-of-line).
        ~LabelGroup() {}
    };

    QuestsView();
    ~QuestsView() override;  // slots 7, 8

    void InitWithGameState(const boost::shared_ptr<GameState>& gameState);

    void LayoutSubviews() override;  // slot 11
    void Dismiss() override;         // slot 38

    boost::shared_ptr<GameState> gameState;                       // +280
    boost::shared_ptr<GUIScrollView> scrollView;                  // +296
    std::vector<QuestState*> quests;                              // +312
    std::map<std::string, std::vector<LabelGroup> > labelGroups;  // +336
    std::map<std::string, boost::shared_ptr<GUILabel> > labels;   // +360
};

// Golden aarch64 NDK-libc++ layout anchor; MSVC STL member sizes differ,
// so this assert is Android/Linux-only (the Windows build is self-consistent
// under MSVC STL and does not claim golden byte-layout parity).
#ifndef _MSC_VER
static_assert(sizeof(QuestsView) == 384,
              "QuestsView layout: ctor/dtor store maps, golden");
#endif

} // namespace Caver

#endif
