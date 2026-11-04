// Recovered from libswordigo.so (1.4.13) evidence.
//
// Caver::GuideTarget is the engine-side guide target (the thing the
// guide arrow points the player at), converted to/from
// Caver::Proto::GuideTarget by GameData::Load/SaveToProtobufMessage.
//
// Layout proven from the constructor inlined into
// GameData::LoadFromProtobufMessage (0x4709EC), which allocates
// operator new(0x88) and value-initialises exactly this shape:
//   +0   Proto::GuideTarget::Type type (int, zeroed)
//   +8   std::string name
//   +32  std::string levelName
//   +56  std::string objectIdentifier
//   +80  std::string carryObjectIdentifier
//   +104 bool showOnlyAfterSceneLoad (zeroed)
//   +112 std::map<std::string, std::string> portalHints
//        (tree __begin_node at +112, __end_node at +120, size at +128)
// sizeof = 0x88 (136), matching operator new(0x88) at the alloc site.
//
// The class is NON-polymorphic: the golden exports no vtable, no
// typeinfo and no out-of-line ctor/dtor for Caver::GuideTarget
// (dynsym + RTTI absence verified 2026-10-23). Only three own
// out-of-line member functions exist:
//   PortalHintForLevel      0x4339AC (WEAK in the golden — it was
//                           defined inline in this header)
//   LoadFromProtobufMessage 0x47711C
//   SaveToProtobufMessage   0x47724C (const)
//
// Field order in Load/Save (proto field numbers from the golden's
// k<Field>FieldNumber constants): type=1, name=2, level_name=3,
// object_identifier=4, carry_object_identifier=5,
// show_only_after_scene_load=6, portal_hint=7 (repeated LevelObject:
// level_name=1, object_identifier=2).
#ifndef CAVER_GUIDETARGET_H
#define CAVER_GUIDETARGET_H

#include <map>
#include <string>

// ---------------------------------------------------------------------
// Proto::GuideTarget / Proto::GuideTarget_LevelObject come from the
// GENERATED header (proto/GameData.pb.h), which is the single
// definition in the tree. The hand-written layout stubs that used to
// live here are DELETED: they collided with the generated classes
// (same namespace, same names, different members), so no TU could
// include both this header and GameData.pb.h -- which is exactly why
// core/GameData.cpp carries its own TU-local GuideTarget instead of
// including this header. With the stubs gone, GameData.cpp can adopt
// this header once its local class is deleted (core slice job); the
// generated accessors (type/name/level_name/object_identifier/
// carry_object_identifier/show_only_after_scene_load/portal_hint/
// add_portal_hint) are the ones GuideTarget.cpp's Load/Save use, and
// the generated inline bodies match the stub's behaviour (set_
// allocates from the default string on first use; Add() reuses a
// cleared element or Reserves, as the golden Save inlines).
#include "../proto/GameData.pb.h"


namespace Caver {

class GuideTarget {
public:
    GuideTarget()
        : m_type(Proto::GuideTarget::UNKNOWN),
          m_showOnlyAfterSceneLoad(false) {}

    void LoadFromProtobufMessage(const Proto::GuideTarget& message);
    void SaveToProtobufMessage(Proto::GuideTarget* message) const;

    std::string PortalHintForLevel(const std::string& levelName);

    Proto::GuideTarget::Type m_type;                // +0
    std::string m_name;                             // +8
    std::string m_levelName;                        // +32
    std::string m_objectIdentifier;                 // +56
    std::string m_carryObjectIdentifier;            // +80
    bool m_showOnlyAfterSceneLoad;                  // +104
    std::map<std::string, std::string> m_portalHints;   // +112
};

// WEAK in the golden (0x4339AC, 108 bytes): defined inline in the
// header. Returns the mapped object identifier for the level, or an
// empty string when the level has no portal hint.
inline std::string GuideTarget::PortalHintForLevel(const std::string& levelName)
{
    std::map<std::string, std::string>::iterator hint = m_portalHints.find(levelName);
    if (hint == m_portalHints.end())
        return std::string("");
    return hint->second;
}

} // namespace Caver

#endif
