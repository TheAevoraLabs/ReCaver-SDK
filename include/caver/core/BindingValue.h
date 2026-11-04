// Recovered from libswordigo.so (1.4.13) evidence — see docs/CORE_HEADERS.md.
// A typed value passed to / returned from IBindable property accessors
// (ValueForBindedProperty / SetValueForBindedProperty).
//
// There is NO inline storage union: the binary proves a type tag plus a
// type-erased heap value behind a boost::shared_ptr<void>. Every static
// factory below allocates a copy of the value with operator new, builds a
// boost sp_counted_impl_p<T*> control block for that exact T, and stores
// {tag, px, pn, 0, 0} into the 40-byte result object:
//   +0  int tag (evidence per factory, cited on the enum below)
//   +8  px — pointer to the heap copy
//   +16 pn — sp_counted_impl_p<T*> control block
//   +24/+32 second shared_ptr, always null in every factory
// sizeof(BindingValue) = 40 is proven by
// std::vector<BindingValue>::__push_back_slow_path @0x3276A8
// (operator new(40 * n), element stride 40, same field-by-field copy).
// ~BindingValue @0x326790 releases the +32 control block first, then the
// +16 one — i.e. the two shared_ptr members in reverse declaration order.
// Ghidra (g_64.c, Caver::BindingValue bodies) agrees on every offset.
#ifndef CAVER_BINDINGVALUE_H
#define CAVER_BINDINGVALUE_H

#include <string>

#include <boost/shared_ptr.hpp>

#include "Vector2.h"
#include "Rectangle.h"

namespace Caver {

class Program;
class Vector3;
class FloatColor;

// Tag values are proven IN ORDER by the store to offset +0 in each
// factory (and by the switches in ParseFromString @0x513024 and
// ConvertToString @0x5133BC). The original enumerator SPELLINGS are
// UNRECOVERED: no string or RTTI in the binary names them.
// The comment after each value is the heap copy the factory makes.
enum BindingValueType {
    BindingValueTypeNone       = 0,  // UNRECOVERED: spelling. Default/empty: ParseFromString @0x513024 writes tag 0 with both shared_ptrs null for type 0, types 7-10 and any out-of-range type.
    BindingValueTypeString     = 1,  // std::string, operator new(0x18) — ValueWithString @0x326608
    BindingValueTypeInt        = 2,  // int, operator new(4) — ValueWithInt @0x32C138
    BindingValueTypeUInt       = 3,  // unsigned int bits, operator new(4) — ValueWithUInt @0x513274 (parameter mangled 'i'/int; ParseFromString @0x513024 feeds it boost lexical_cast<unsigned int>, ConvertToString @0x5133BC case 3 reads it back as unsigned int)
    BindingValueTypeBool       = 4,  // bool, operator new(1) — ValueWithBool @0x32A71C
    BindingValueTypeFloat      = 5,  // float, operator new(4) — ValueWithFloat @0x32A864
    BindingValueTypeProgram    = 6,  // Program, operator new(0x38) — ValueWithProgram @0x512874
    BindingValueTypeFloatColor = 7,  // FloatColor (16 bytes, 4 floats), operator new(0x10) — ValueWithFloatColor @0x512A9C
    BindingValueTypeVector2    = 8,  // Vector2 (8 bytes), operator new(8) — ValueWithVector2 @0x512BF8
    BindingValueTypeVector3    = 9,  // Vector3 (12 bytes), operator new(0xC) — ValueWithVector3 @0x512D5C
    BindingValueTypeRectangle  = 10, // Rectangle (16 bytes), operator new(0x10) — ValueWithRectangle @0x512EC8
};

class BindingValue {
public:
    // No out-of-line constructor or copy constructor exists in the arm64
    // binary: default/copy construction is inlined at every call site
    // (see e.g. the field-by-field copy in __push_back_slow_path
    // @0x3276A8). Only the destructor is out-of-line.
    BindingValue() : m_type(BindingValueTypeNone) {}
    BindingValue(BindingValueType type, const boost::shared_ptr<void>& value)
        : m_type(type), m_value(value) {}
    ~BindingValue();                                // @0x326790

    // Static factories (mangled names carry no 'this'; call sites pass
    // the value in the first argument register and the result in x8).
    static BindingValue ValueWithString(const std::string& value);   // @0x326608
    static BindingValue ValueWithInt(int value);                     // @0x32C138
    static BindingValue ValueWithUInt(int value);                    // @0x513274 — see the UInt enum note on the stored bits
    static BindingValue ValueWithBool(bool value);                   // @0x32A71C
    static BindingValue ValueWithFloat(float value);                 // @0x32A864
    static BindingValue ValueWithProgram(const Program& value);      // @0x512874
    static BindingValue ValueWithFloatColor(const FloatColor& value);// @0x512A9C
    static BindingValue ValueWithVector2(const Vector2& value);      // @0x512BF8
    static BindingValue ValueWithVector3(const Vector3& value);      // @0x512D5C
    static BindingValue ValueWithRectangle(const Rectangle& value);  // @0x512EC8
    static BindingValue ParseFromString(const std::string& string, BindingValueType type); // @0x513024

    // Out-of-line accessors. There are NO out-of-line Int/UInt/Bool/
    // Float/String accessors: call sites read the value directly through
    // the +8 pointer (e.g. CollectableItemComponent::
    // SetValueForBindedProperty @0x360CAC does **((_DWORD**)value + 1)).
    Program ProgramValue() const;                   // @0x512A24 — copies the 56-byte Program out of the +8 pointer
    FloatColor FloatColorValue() const;             // @0x512BE8 — copies 16 bytes out of the +8 pointer
    Vector2 Vector2Value() const;                   // @0x512D4C — copies 8 bytes out of the +8 pointer
    Vector3 Vector3Value() const;                   // @0x512EB0 — copies 12 bytes out of the +8 pointer
    Rectangle RectangleValue() const;               // @0x513014 — copies 16 bytes out of the +8 pointer
    std::string ConvertToString() const;            // @0x5133BC — only tags 1..6 convert; 0 and 7..10 yield an empty string

    BindingValueType m_type;                        // +0 — evidence: tag store in every factory above
    char _pad4[4];                                  // +4 — never written or read in any factory/accessor/dtor body
    boost::shared_ptr<void> m_value;                // +8 (px +8, pn +16) — evidence: factories @0x326608..@0x513274 store px/pn here; ~BindingValue @0x326790 releases pn at +16 last. Declared element type is the type-erasure reading of the per-T control blocks; the exact original declared type is UNRECOVERED: it may have been shared_ptr<void> exactly as written here.
    boost::shared_ptr<std::string> m_stringValue;   // +24 (px +24, pn +32) — evidence: null in every factory; Component::ValueForBindedOutlet @0x349710 and Texture::ValueForBindedProperty @0x54EA64 lazily create it via boost::shared_ptr<std::string>::reset<std::string> at this+24 and assign it a string (the source component's label, resp. the enum string for an Int value, e.g. imageTypeStrings[] / pixelFormatStrings[]). Original member NAME is UNRECOVERED: m_stringValue is descriptive only.
};

} // namespace Caver

#endif
