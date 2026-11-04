// LAYOUT STUB for Caver::Proto::ItemDropComponent (protobuf-generated
// extension message), recovered from the bodies of
// ItemDropComponent::LoadFromProtobufMessage / SaveToProtobufMessage in
// libswordigo.so (1.4.13), the generated class in src/caver/proto/
// Scene.pb.h, and the field-number constants in .rodata. The real
// generated header (caver/proto/Scene.pb.h) supersedes this stub at
// merge time; this stub exists so the world-components lane can build
// and parity-grade Load/Save now (same pattern as ProtoHealthComponent.h).
//
// Schema (Scene.proto, proven):
//   message ItemDropComponent (extension of Proto::Component, field 505):
//     field 1  item_name               (string)  -> msg +16 (ptr)
//     field 2  item_identifier         (string)  -> msg +24 (ptr)
//     field 3  drop_entry              (repeated ItemDropEntry) -> +32
//     field 4  can_drop_multiple_items (bool)    -> msg +88
//     field 5  can_drop_default_items  (bool)    -> msg +89
//     has_bits_ at +92 (bits 1/2/4/8/0x10 in field order)
//   message ItemDropComponent_ItemDropEntry (sizeof 0x30, Save news it):
//     field 1  template_name   (string) -> msg +16 (ptr)
//     field 2  item_identifier (string) -> msg +24 (ptr)
//     field 3  drop_chance     (float)  -> msg +32
//     field 4  min_count       (int32)  -> msg +36
//     field 5  max_count       (int32)  -> msg +40
//     has_bits_ at +44
// The drop_entry_ RepeatedPtrFieldBase sits at msg +32: elements ptr
// +32, current_size_ +40, allocated_size_ +44, total_size_ +48.
#ifndef CAVER_PROTO_ITEMDROPCOMPONENT_STUB_H
#define CAVER_PROTO_ITEMDROPCOMPONENT_STUB_H

#include <string>

namespace google {
namespace protobuf {

class MessageLite {};
class FieldDescriptor;

namespace internal {

class ExtensionSet {
public:
    const MessageLite& GetMessage(int number, const MessageLite& default_instance) const;
    MessageLite* MutableMessage(int number, unsigned char type,
                                const MessageLite& default_instance,
                                const FieldDescriptor* descriptor);
};

class RepeatedPtrFieldBase {
public:
    void Reserve(int new_size);
};

} // namespace internal
} // namespace protobuf
} // namespace google

namespace Caver {
namespace Proto {

class ItemDropComponent_ItemDropEntry : public ::google::protobuf::MessageLite {
public:
    ItemDropComponent_ItemDropEntry();

    char _messageLiteInternals[12];     // +0..+15 (vptr, cached size, pad)
    std::string* template_name_;        // +16 — field 1
    static const std::string _default_template_name_;
    std::string* item_identifier_;      // +24 — field 2
    static const std::string _default_item_identifier_;
    float drop_chance_;                 // +32 — field 3
    int min_count_;                     // +36 — field 4
    int max_count_;                     // +40 — field 5
    int has_bits_;                      // +44
};

class ItemDropComponent : public ::google::protobuf::MessageLite {
public:
    char _messageLiteInternals[12];     // +0..+15
    std::string* item_name_;            // +16 — field 1
    static const std::string _default_item_name_;
    std::string* item_identifier_;      // +24 — field 2
    static const std::string _default_item_identifier_;
    // RepeatedPtrField<ItemDropComponent_ItemDropEntry> drop_entry_; // +32
    void** drop_entry_elements_;        // +32
    int drop_entry_current_size_;       // +40
    int drop_entry_allocated_size_;     // +44
    int drop_entry_total_size_;         // +48
    char _pad52[36];                    // +52..+87
    bool can_drop_multiple_items_;      // +88 — field 4
    bool can_drop_default_items_;       // +89 — field 5
    char _pad90[2];                     // +90
    int has_bits_;                      // +92

    struct ExtensionIdentifier {
        int number;                                 // +0 — 505 (proven)
        const ItemDropComponent* default_instance;  // +8
    };
    static ExtensionIdentifier extension;
    static const ItemDropComponent* default_instance_;
    static const ItemDropComponent* default_instance();
};

} // namespace Proto
} // namespace Caver

#endif
