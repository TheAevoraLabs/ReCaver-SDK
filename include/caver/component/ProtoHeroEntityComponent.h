// LAYOUT STUB for Caver::Proto::HeroEntityComponent (protobuf-generated
// extension message), recovered from the bodies of
// HeroEntityComponent::LoadFromProtobufMessage / SaveToProtobufMessage
// in libswordigo.so (1.4.13) and the generated class's own field-number
// constant in .rodata. The generated-proto work owns the real class
// (src/caver/proto/Scene.pb.h); this stub exists so the hero slice can
// build and parity-grade Load/Save now, mirroring ProtoHealthComponent.h.
//
// Schema (proven): extension of Proto::Component, field number 165
// (kExtensionFieldNumber at golden 0x2684F8; HealthComponent's 123 at
// 0x268284 cross-checks the method). One field:
//   field 1  on_item_get  (Proto::Program message)  -> +16 (ptr)
//   has_bits_ at +24 (bit0 on_item_get), proven by Save's
//   `*(ext + 24) |= 1` and its operator new(0x30) for Proto::Program.
#ifndef CAVER_PROTO_HEROENTITYCOMPONENT_STUB_H
#define CAVER_PROTO_HEROENTITYCOMPONENT_STUB_H

namespace google {
namespace protobuf {

class MessageLite {};   // shape-only: lets the stub messages derive from it
class FieldDescriptor;

namespace internal {

// Only the two out-of-line entry points the hero slice calls, declared
// with their exact golden signatures (same as ProtoHealthComponent.h).
class ExtensionSet {
public:
    const MessageLite& GetMessage(int number, const MessageLite& default_instance) const;
    MessageLite* MutableMessage(int number, unsigned char type,
                                const MessageLite& default_instance,
                                const FieldDescriptor* descriptor);
};

} // namespace internal
} // namespace protobuf
} // namespace google

namespace Caver {
// Forward declaration for the friend declaration below (see
// ProtoHealthComponent.h for why it is needed).
class HeroEntityComponent;
namespace Proto {

// Proto::Program message: only what Save touches — the constructor
// (out-of-line in the generated TU) and the proven 0x30 size
// (Save's operator new(0x30)); has_bits_ sits at +24 per Save's
// `*(message + 24) |= 1` on the extension message below, and the
// on_item_get pointer at +16 is read by both Load and Save.
class Program {
public:
    Program();
    char _messageLiteInternals[48];     // +0..+47 (sizeof 0x30 proven)
};

class HeroEntityComponent : public ::google::protobuf::MessageLite {
public:
    char _messageLiteInternals[12];     // +0..+11 (vtable ptr, cached size)
    char _pad12[4];                     // +12
    Program* on_item_get_;              // +16 — field 1
    int has_bits_;                      // +24

    // Extension identifier object shape (see ProtoHealthComponent.h):
    // field number 165 at +0, default instance pointer at +8.
    struct ExtensionIdentifier {
        int number;                                 // +0 — 165 (proven)
        const HeroEntityComponent* default_instance; // +8
    };
    static ExtensionIdentifier extension;
private:
    // Private to match the generated .pb class exactly (MSVC mangles
    // static access; Itanium does not, which is why this only surfaced
    // on Windows). The component class is a friend so its Load/Save
    // methods keep accessing it.
    friend class ::Caver::HeroEntityComponent;
    static HeroEntityComponent* default_instance_;
public:
    static const HeroEntityComponent& default_instance();
};

} // namespace Proto
} // namespace Caver

#endif
