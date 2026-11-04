// LAYOUT STUB for Caver::Proto::EntityControllerComponent (protobuf-
// generated extension message), recovered from the bodies of
// EntityControllerComponent::LoadFromProtobufMessage /
// SaveToProtobufMessage in libswordigo.so (1.4.13). Modelled on
// ProtoHealthComponent.h (same slice pattern). The generated-proto
// work owns the real class (src/caver/proto/Scene.pb.h); this stub
// exists so this slice can build and parity-grade Load/Save now.
//
// Schema (src/caver/proto/Scene.proto, message EntityControllerComponent):
//   field 1  entity_id                (int32) -> +12
//   field 2  animation_controller_id  (int32) -> +16
//   field 3  default_move_animation_id(int32) -> +20
//   field 4  roam_area_id             (int32) -> +24
//   field 5  default_move_speed       (float) -> +28
//   field 6  default_acceleration     (float) -> +32
//   field 7  targeting_distance       (float) -> +36
//   field 8  movement_behavior        (enum)  -> +40
//   has_bits_ at +44 (bits 0..7, one per field — Save sets 0xFF)
// The extension identifier object is reached through the GOT in the
// golden (runtime-registered); its field number is UNRECOVERED here
// (HealthComponent's is 123; this one is registered by the proto
// slice's own registration code, which is not in the tree yet).
#ifndef CAVER_PROTO_ENTITYCONTROLLERCOMPONENT_STUB_H
#define CAVER_PROTO_ENTITYCONTROLLERCOMPONENT_STUB_H

namespace google {
namespace protobuf {

class MessageLite {};   // shape-only: lets the stub messages derive from it
class FieldDescriptor;

namespace internal {

// Only the two out-of-line entry points this slice calls, declared
// with their exact golden signatures (PLT names verified:
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
namespace Proto {

class EntityControllerComponent : public ::google::protobuf::MessageLite {
public:
    char _messageLiteInternals[12];     // +0..+11 (vtable ptr, cached size)
    int entity_id_;                     // +12 — field 1
    int animation_controller_id_;       // +16 — field 2
    int default_move_animation_id_;     // +20 — field 3
    int roam_area_id_;                  // +24 — field 4
    float default_move_speed_;          // +28 — field 5
    float default_acceleration_;        // +32 — field 6
    float targeting_distance_;          // +36 — field 7
    int movement_behavior_;             // +40 — field 8
    int has_bits_;                      // +44

    // Extension identifier object shape (see header note): field
    // number at +0 (UNRECOVERED), default instance pointer at +8.
    struct ExtensionIdentifier {
        int number;                                 // +0 — UNRECOVERED
        const EntityControllerComponent* default_instance;  // +8
    };
    static ExtensionIdentifier extension;
    static const EntityControllerComponent* default_instance_;
    static const EntityControllerComponent& default_instance();
};

} // namespace Proto
} // namespace Caver

#endif
