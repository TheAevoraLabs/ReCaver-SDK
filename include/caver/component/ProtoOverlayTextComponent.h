// LAYOUT STUB for Caver::Proto::OverlayTextComponent (protobuf-generated
// extension message), recovered from the bodies of
// OverlayTextComponent::LoadFromProtobufMessage / SaveToProtobufMessage
// in libswordigo.so (1.4.13) and the generated class's own field-number
// constants in .rodata (kTextFieldNumber=1, kTextOffsetFieldNumber=2,
// kSpriteNameFieldNumber=3, kSpriteOffsetFieldNumber=4,
// kExtensionFieldNumber=506 — read from the golden .rodata at
// 0x2687D8..0x2687E8). The generated-proto work owns the real class
// (src/caver/proto/Scene.pb.h); this stub exists so the world-components
// slice can build and parity-grade Load/Save now, following the
// ProtoHealthComponent.h precedent (whose ExtensionSet / Vector2 /
// MessageLite shapes are reused via the include below).
//
// Message layout (proven from Load/Save offset arithmetic):
//   +0  vtable ptr / MessageLite internals (12 bytes with cached size)
//   +16 text_          (std::string*, field 1)
//   +24 text_offset_   (Proto::Vector2*, field 2; floats at msg +12/+16)
//   +32 sprite_name_   (std::string*, field 3)
//   +40 sprite_offset_ (Proto::Vector2*, field 4)
//   +48 has_bits_      (bit0 text, bit1 text_offset, bit2 sprite_name,
//                       bit3 sprite_offset — set in that order by Save)
// The message is registered as an extension of Proto::Component with
// field number 506; the extension identifier object lives in BSS as
// OverlayTextComponent::extension (16 bytes: field number at +0,
// default-instance pointer at +8 — Load passes *(extension+8) to
// ExtensionSet::GetMessage).
#ifndef CAVER_PROTO_OVERLAYTEXTCOMPONENT_STUB_H
#define CAVER_PROTO_OVERLAYTEXTCOMPONENT_STUB_H

#include "ProtoHealthComponent.h"

namespace Caver {
// Forward declaration for the friend declaration below (see
// ProtoHealthComponent.h for why it is needed).
class OverlayTextComponent;
namespace Proto {

class OverlayTextComponent : public ::google::protobuf::MessageLite {
public:
    char _messageLiteInternals[12];     // +0..+11 (vtable ptr, cached size)
    std::string* text_;                 // +16 — field 1
    Vector2* text_offset_;              // +24 — field 2
    std::string* sprite_name_;          // +32 — field 3
    Vector2* sprite_offset_;            // +40 — field 4
    int has_bits_;                      // +48

private:
    // Private and const-correct to match the generated .pb class
    // exactly (MSVC mangles static access and const-ness; Itanium
    // ignores both, which is why this only surfaced on Windows). The
    // component class is a friend so its Load/Save methods keep
    // accessing these.
    friend class ::Caver::OverlayTextComponent;
    static const std::string _default_text_;
    static const std::string _default_sprite_name_;
    static OverlayTextComponent* default_instance_;
public:

    // Extension identifier object shape: field number 506 at +0,
    // default instance pointer at +8 (runtime-registered).
    struct ExtensionIdentifier {
        int number;                                     // +0 — 506 (proven)
        const OverlayTextComponent* default_instance;   // +8
    };
    static ExtensionIdentifier extension;
    static const OverlayTextComponent& default_instance();
};

} // namespace Proto
} // namespace Caver

#endif
