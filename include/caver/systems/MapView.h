// Recovered from libswordigo.so (1.4.13) evidence — IDA/Hex-Rays exports
// in ~/workspace/analysis/swordigo/functions/Caver/MapView/ (+ MapViewNode),
// the golden ctor (0x4C1A2C) / dtor (0x4C791C) instruction streams, and
// every method body's field accesses.
//
// Caver::MapView is the in-game world-map screen: a GUIView (256 bytes)
// that owns the map's view-node list, five embedded 88-byte Sprites
// (nodes / icons / paths / guide / selection), eighteen
// intrusive_ptr<Texture> map icons, the location-marker GUIFrameView and
// the marker-move GUIAnimation state. Object size 1072 bytes, proven by
// the highest ctor/dtor accesses (+1048 shared_ptr, +1064 bool).
//
// Layout (all offsets byte-proven; names marked UNRECOVERED where the
// binary gives no name):
//   +256 MapViewDelegate* m_delegate (raw; TouchEnded calls its vtable
//        slots 0/1/2 = DidSelectionDidChange / DidTapSelectedNode /
//        DidTapEmptySpace, matching MapMenuPage/PortalViewController)
//   +264 boost::shared_ptr<Map> m_map
//   +280 boost::shared_ptr<GameState> m_gameState
//   +296 std::vector<boost::shared_ptr<MapViewNode> > m_viewNodes
//   +320 Sprite m_nodesSprite     (Init with the parent texture of
//   +408 Sprite m_iconsSprite       the "ui_map_node" texture, etc.)
//   +496 Sprite m_pathsSprite
//   +584 Sprite m_guideSprite
//   +672 boost::shared_ptr<MapPath> m_targetPath (guide path)
//   +688 int m_targetKind (guide-target type, from GuideTarget+0;
//        1/2 -> quest icon, 3 -> spell icon, 4 -> key icon)
//   +696 boost::shared_ptr<GUIFrameView> m_locationMarker
//   +712 boost::shared_ptr<MapNode> m_animFromNode
//   +728 boost::shared_ptr<MapNode> m_animToNode
//   +744 boost::shared_ptr<GUIAnimation> m_markerAnimation
//   +760 boost::intrusive_ptr<Texture> m_selectedTexture
//   +768 Sprite m_selectionSprite
//   +856..+992 eighteen intrusive_ptr<Texture> icons, in ctor load
//        order: node, town, boss, locked, path, checkmark, quest,
//        portal, portal_big, treasure, guide_edge, guide_end,
//        guide_end_noarrow, guide_icon_key, guide_icon_quest,
//        guide_icon_spell, guide_node, guide_start
//   +1000 bool m_nodePressed
//   +1008 unsigned long m_pressedNodeIndex
//   +1016 unsigned long long m_touchIdentifier (FWTouch identifier
//        latched by TouchBegan, matched by TouchEnded/Cancelled)
//   +1024 Vector2 m_touchPosition (FWTouch position latched likewise)
//   +1032 bool m_hasSelection
//   +1040 unsigned long m_selectedNodeIndex
//   +1048 boost::shared_ptr<GUIView> m_field1048 (UNRECOVERED name;
//        ClearSelection calls slot 15 (RemoveFromSuperview) on it and
//        resets it; nothing in MapView ever assigns it)
//   +1064 bool m_field1064 (UNRECOVERED name; Init's second bool
//        argument — when set, nodes are drawn at 0.7 scale/tint unless
//        their MapViewNode+41 flag is set, and TouchEnded ignores
//        nodes without that flag)
//
// The classes below marked "stub" belong to other slices; only the
// surface MapView uses is declared, with layouts pinned by the MapView
// bodies themselves (binary wins over the owning slices' headers).
#ifndef CAVER_MAPVIEW_H
#define CAVER_MAPVIEW_H

#include <cstring>
#include <string>
#include <vector>

#include <boost/intrusive_ptr.hpp>
#include <boost/shared_ptr.hpp>

#include "../core/Color.h"
#include "../core/FWTouch.h"
#include "../core/GameData.h"
#include "../core/GameState.h"
#include "../core/LevelState.h"
#include "../core/Matrix4.h"
#include "../core/Quest.h"
#include "../core/Rectangle.h"
#include "../core/Vector2.h"
#include "../audio/AudioSystem.h"
#include "../audio/SoundEffect.h"
#include "../gui/GUIAnimation.h"
#include "../gui/GuideTarget.h"
#include "../gui/GUIFrameView.h"
#include "../render/Sprite.h"
#include "../gui/GUIView.h"
#include "../render/RenderingContext.h"
#include "../render/Texture.h"
#include "../render/TextureLibrary.h"

#include "MapNode.h"
#include "Map.h"

namespace Caver {

class MapView;
class QuestState;

// ---- declare-only stubs for other slices' types ----------------------
// StateProperties, LevelState and Quest are RETIRED stubs: they now
// come from their real headers (core/LevelState.h, core/Quest.h,
// included above). Layout proof for the retirement:
//   StateProperties: 24 bytes, std::set<std::string> of flags;
//     HasFlag is the only member MapView uses (LevelState+32).
//   LevelState: 64 bytes, identical to the retired stub (+0 name,
//     +24 visited, +32 properties, +56 numTreasures, +60
//     visited-node count added into MapViewNode+36 by MapView::Init).
//   Quest: 104 bytes, identical to the retired layout-only stub
//     (int +0 = -1 in the golden ctor @0x47E36C; strings at
//     +8/+32/+56/+80 per Load @0x47E394, Save @0x47E3E0).
//     MapView::Init's quest pass reads only the +80 string (the
//     quest's target level name, fed to Map::NodeForName), which
//     the real header calls m_mapLocation.
// GameData and GameState are RETIRED stubs: they now come from
// their real headers (core/GameData.h, core/GameState.h, included
// above), following the wave 7 StateProperties/LevelState/Quest
// pattern. Layout proof for the retirement:
//   GameData: real header carries the +48 holder as private
//     shared_ptr<void> m_unknown48; its pointee is the world Map
//     (GameState::Load calls NodeForName on it), exposed to
//     includers through the typed GameData::map() accessor.
//   GameState: real header carries +0 shared_ptr<GameData>
//     m_gameData, +320 as shared_ptr<void> m_shared320 (pointee
//     the current MapNode, whose +16 name SaveToProtobufMessage
//     reads), and +488 as bool m_guideEnabled; includers reach
//     them through GameState::currentNode() and guideEnabled().
//   The old stub member names (m_map, m_currentNode, m_field488)
//     are gone: every includer use site was migrated to the
//     accessors in the same commit, with identical values and
//     shared_ptr ownership semantics. No field, type or layout
//     change in either real header (accessors only).
// GuideTarget is a RETIRED stub: it now comes from its real header
// (gui/GuideTarget.h, included above), following the wave 7/8
// GameData/GameState pattern. Layout proof for the retirement:
//   First 56 bytes identical to the retired stub: +0 type
//     (Proto::GuideTarget::Type enum, values UNKNOWN=0, QUESTGET=1,
//     QUEST=2, SPELL=3, KEY=4, matching MapView's 1/2 -> quest icon,
//     3 -> spell icon, 4 -> key icon branches), +8 std::string
//     m_name, +32 std::string m_levelName (FindPath's target).
//   The real header adds members past +56 that the stub lacked
//     (+56 objectIdentifier, +80 carryObjectIdentifier,
//     +104 showOnlyAfterSceneLoad, +112 portalHints map),
//     for sizeof 136 (0x88, the golden operator new size);
//     MapView never allocates a GuideTarget by value and never
//     touches past +32, so the growth changes no MapView behaviour.
//   The only MapView use-site migration is in MapView.cpp:
//     m_string32 -> m_levelName; m_type is used unchanged
//     (the enum converts implicitly to MapView's int m_targetKind).
// No field, type or layout change in the real header.

// AudioSystem / SoundEffectSource: the audio slice's real headers are
// included above; TouchEnded plays the "click" sound and releases the
// returned intrusive_ptr<SoundEffectSource> at scope exit.

// ---- MapViewDelegate --------------------------------------------------
// Vtable slot order proven by TouchEnded's indirect calls through
// MapView+256 (slot 0 after a selection change, slot 1 on re-tap of
// the selected node, slot 2 on empty space) and by the implementors
// (MapMenuPage, PortalViewController).
class MapViewDelegate {
public:
    virtual void MapViewDidSelectionDidChange(MapView* mapView) = 0;
    virtual void MapViewDidTapSelectedNode(MapView* mapView) = 0;
    virtual void MapViewDidTapEmptySpace(MapView* mapView) = 0;
};

// ---- MapViewNode -------------------------------------------------------
// 48 bytes, layout from its ctor (0x4C880C) and InitWithMapNode
// (0x4C8834) plus every MapView body that touches it.
class MapViewNode {
public:
    MapViewNode();
    void InitWithMapNode(const boost::shared_ptr<MapNode>& node);

    boost::shared_ptr<MapNode> m_mapNode;  // +0
    Vector2 m_field16;                     // +16 (zeroed by the ctor;
                                           //  no MapView body writes
                                           //  it — name UNRECOVERED)
    bool m_visible;                        // +24 (ctor: 1)
    bool m_field25;                        // +25 (ctor: 0; the
                                           //  "known" gate for icons,
                                           //  recursion and touches)
    bool m_field26;                        // +26 (ctor: 1; Init sets it
                                           //  to !m_field25)
    bool m_field27;                        // +27 (ctor: 0; set for the
                                           //  GameState current node)
    bool m_field28;                        // +28 (ctor: 1; Init clears
                                           //  it; checkmark icon gate)
    bool m_field29;                        // +29 (ctor: 0; quest icon
                                           //  gate, set by Init's
                                           //  quest pass)
    char _pad30[2];                        // +30
    int m_field32;                         // +32 (icon count base: the
                                           //  node's +156 int is added
                                           //  in Init; also the count
                                           //  of treasure icons drawn)
    int m_field36;                         // +36 (collected count:
                                           //  LevelState+60 added in
                                           //  Init when applicable)
    bool m_field40;                        // +40 (portal flag)
    bool m_field41;                        // +41 (portal-found flag)
    char _pad42[2];                        // +42
    float m_field44;                       // +44 (node radius: 16.0
                                           //  after InitWithMapNode
                                           //  (MapView.cpp:136, IDA
                                           //  InitWithMapNode
                                           //  @0x4C8834 stores
                                           //  1098907648 = 16.0f),
                                           //  11.2 in the dimmed
                                           //  UpdateNodesSprite path)
};

// ---- MapView -----------------------------------------------------------
class MapView : public GUIView {
public:
    MapView();
    virtual ~MapView();

    void Init(const boost::shared_ptr<Map>& map,
              const boost::shared_ptr<GameState>& gameState, bool flagA,
              bool flagB);

    void UpdateTargetPath();
    void UpdateDefaultNodeVisibility();
    Rectangle LocationMarkerFrameForMapNode(MapNode* node);
    void RecursivelySetVisibleNodes(MapNode* node, const Vector2& center,
                                    const Vector2& range);
    void RecursivelySetVisibleNodes(MapNode* node, int depth);
    void UpdateNodes();
    void HideFarNodes(const boost::shared_ptr<MapNode>& node, float dx,
                      float dy);
    void UpdateNodesSprite();
    void UpdatePathsSprite();
    // Returns the animation by value in the golden (sret in x8 —
    // the mangled name does not encode it; proven by the golden
    // body's writes to the x8 slot on every path).
    boost::shared_ptr<GUIAnimation> AnimateLocationMarkerFromNodeToNode(
        const boost::shared_ptr<MapNode>& from,
        const boost::shared_ptr<MapNode>& to);
    void UpdateSelectionSprite();
    void AddPathToSprite(Sprite* sprite,
                         const boost::shared_ptr<MapPath>& path,
                         Color color,
                         const boost::intrusive_ptr<Texture>& texture);
    void AddPathSegmentToSprite(Sprite* sprite, const Vector2& from,
                                const Vector2& to, float width,
                                const boost::intrusive_ptr<Texture>& texture,
                                Color color);
    void AddPathSprite(MapViewNode* from, MapViewNode* to);
    void SetNodePressed(unsigned long index, bool pressed);
    void SelectNode(const boost::shared_ptr<MapNode>& node);
    void SelectNodeAtIndex(unsigned long index);
    void ClearSelection();
    MapViewNode* NearestNodeInDirection(MapViewNode* node,
                                        const Vector2& direction);

    virtual void LayoutSubviews();  // slot 11 (empty in the golden)
    virtual void Update(float deltaTime);  // slot 9
    virtual void DrawRect(RenderingContext* context, const Rectangle& rect,
                          const Matrix4& transform);  // slot 10
    virtual void TouchBegan(const FWTouch& touch);    // slot 0
    virtual void TouchMoved(const FWTouch& touch);    // slot 1
    virtual void TouchEnded(const FWTouch& touch);    // slot 2
    virtual void TouchCancelled(const FWTouch& touch);  // slot 3

    MapViewDelegate* m_delegate;                        // +256
    boost::shared_ptr<Map> m_map;                       // +264
    boost::shared_ptr<GameState> m_gameState;           // +280
    std::vector<boost::shared_ptr<MapViewNode> > m_viewNodes;  // +296
    Sprite m_nodesSprite;                               // +320
    Sprite m_iconsSprite;                               // +408
    Sprite m_pathsSprite;                               // +496
    Sprite m_guideSprite;                               // +584
    boost::shared_ptr<MapPath> m_targetPath;            // +672
    int m_targetKind;                                   // +688
    boost::shared_ptr<GUIFrameView> m_locationMarker;   // +696
    boost::shared_ptr<MapNode> m_animFromNode;          // +712
    boost::shared_ptr<MapNode> m_animToNode;            // +728
    boost::shared_ptr<GUIAnimation> m_markerAnimation;  // +744
    boost::intrusive_ptr<Texture> m_selectedTexture;    // +760
    Sprite m_selectionSprite;                           // +768
    boost::intrusive_ptr<Texture> m_nodeTexture;        // +856
    boost::intrusive_ptr<Texture> m_townTexture;        // +864
    boost::intrusive_ptr<Texture> m_bossTexture;        // +872
    boost::intrusive_ptr<Texture> m_lockedTexture;      // +880
    boost::intrusive_ptr<Texture> m_pathTexture;        // +888
    boost::intrusive_ptr<Texture> m_checkmarkTexture;   // +896
    boost::intrusive_ptr<Texture> m_questTexture;      // +904
    boost::intrusive_ptr<Texture> m_portalTexture;      // +912
    boost::intrusive_ptr<Texture> m_portalBigTexture;   // +920
    boost::intrusive_ptr<Texture> m_treasureTexture;    // +928
    boost::intrusive_ptr<Texture> m_guideEdgeTexture;   // +936
    boost::intrusive_ptr<Texture> m_guideEndTexture;    // +944
    boost::intrusive_ptr<Texture> m_guideEndNoArrowTexture;  // +952
    boost::intrusive_ptr<Texture> m_guideKeyTexture;    // +960
    boost::intrusive_ptr<Texture> m_guideQuestTexture;  // +968
    boost::intrusive_ptr<Texture> m_guideSpellTexture;  // +976
    boost::intrusive_ptr<Texture> m_guideNodeTexture;   // +984
    boost::intrusive_ptr<Texture> m_guideStartTexture;  // +992
    bool m_nodePressed;                                 // +1000
    unsigned long m_pressedNodeIndex;                   // +1008
    unsigned long long m_touchIdentifier;               // +1016
    Vector2 m_touchPosition;                            // +1024
    bool m_hasSelection;                                // +1032
    unsigned long m_selectedNodeIndex;                  // +1040
    boost::shared_ptr<GUIView> m_field1048;             // +1048
    bool m_field1064;                                   // +1064

private:
    void AddNodeIconsToSprite(MapViewNode* node, const Rectangle& rect,
                              float scale);
};

} // namespace Caver

#endif
