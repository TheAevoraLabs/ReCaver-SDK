// LAYOUT STUB for Caver::Proto::HealthComponent (protobuf-generated
// extension message), recovered from the bodies of
// HealthComponent::LoadFromProtobufMessage / SaveToProtobufMessage in
// libswordigo.so (1.4.13) and the generated class's own field-number
// constants in .rodata. The generated-proto work owns the real class;
// this stub exists so the components slice can build and parity-grade
// Load/Save now.
//
// Schema (proven from k<Field>FieldNumber constants):
//   field 1  max_health  (int32)                        -> +12
//   field 2  health_type (enum Type: ENEMY=0, FRIENDLY=1) -> +16
//   field 3  bar_offset  (Proto::Vector2 message)       -> +24 (ptr)
//   has_bits_ at +32 (bit0 max_health, bit1 health_type, bit2 bar_offset)
// The message is registered as an extension of Proto::Component with
// field number 123 (kExtensionFieldNumber); the extension identifier
// object lives in BSS as HealthComponent::extension: the field number
// sits at +0 and the default-instance pointer at +8 (both loads proven
// in Load's prologue at 0x36B3E0).
#ifndef CAVER_PROTO_HEALTHCOMPONENT_STUB_H
#define CAVER_PROTO_HEALTHCOMPONENT_STUB_H

namespace google {
namespace protobuf {

class MessageLite {};   // shape-only: lets the stub messages derive from it
class FieldDescriptor;

namespace internal {

// Only the two out-of-line entry points the components slice calls,
// declared with their exact golden signatures (PLT names verified:
// _ZNK6google8protobuf8internal12ExtensionSet10GetMessageEiRKNS0_11MessageLiteE
// _ZN6google8protobuf8internal12ExtensionSet14MutableMessageEihRKNS0_11MessageLiteEPKNS0_15FieldDescriptorE).
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
// Forward declaration for the friend declaration below: a qualified
// friend (friend class ::Caver::HealthComponent) requires the class to
// already be declared, and this header is included before
// HealthComponent.h in some TUs.
class HealthComponent;
namespace Proto {

// Proto::Vector2 message: only the fields Load/Save touch. The
// component's bar offset is a pair of floats (x_ +12, y_ +16) that the
// component code moves as one double-width value; has_bits_ at +20
// (bit0 x, bit1 y). sizeof 0x18 (Save's operator new(0x18)).
class Vector2 {
public:
    Vector2();

    char _messageLiteInternals[12];     // +0..+11
    float x_;                           // +12
    float y_;                           // +16
    int has_bits_;                      // +20
};

class HealthComponent : public ::google::protobuf::MessageLite {
public:
    char _messageLiteInternals[12];     // +0..+11 (vtable ptr, cached size)
    int max_health_;                    // +12 — field 1
    int health_type_;                   // +16 — field 2 (enum: 0 ENEMY, 1 FRIENDLY)
    char _pad20[4];                     // +20
    Vector2* bar_offset_;               // +24 — field 3
    int has_bits_;                      // +32

    // Extension identifier object shape (see header note): field
    // number 123 at +0, default instance pointer at +8.
    struct ExtensionIdentifier {
        int number;                                 // +0 — 123 (proven)
        const HealthComponent* default_instance;    // +8
    };
    static ExtensionIdentifier extension;
private:
    // MSVC mangles static data members with access and const-ness, so
    // this declaration must match the generated .pb class exactly
    // (private, non-const pointer); Itanium ignores both, which is why
    // the mismatch only surfaced on Windows. The component class is a
    // friend so its Load/Save methods keep accessing it.
    friend class ::Caver::HealthComponent;
    static HealthComponent* default_instance_;
public:
    // Reference-returning to match the generated .pb class (see the
    // static above for why MSVC cares and Itanium does not).
    static const HealthComponent& default_instance();
};

} // namespace Proto
} // namespace Caver

#endif
