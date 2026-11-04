// LAYOUT STUB for Caver::Proto::SoundLibrary (protobuf-generated class),
// recovered from the bodies of Proto::SoundLibrary::SharedCtor / Clear /
// MergeFrom and SoundLibrary::LoadFromProtobufMessage /
// SaveToProtobufMessage in libswordigo.so (1.4.13). Only the fields those
// functions touch are modelled, at their true offsets. The generated-proto
// work owns the real class; this stub exists so the systems slice can
// build and parity-grade Load/Save now.
//
// Layout (0x50 bytes, from the Proto::SoundLibrary ctor's operator
// new(0x50) in New()):
//   +0   MessageLite vptr
//   +8   int cached_size_ (SharedCtor zeroes it; SetCachedSize writes it)
//   +16  google::protobuf::RepeatedPtrField<Proto::SoundEffect> effects_:
//          +16  SoundEffect** elements_ (starts at &initial_space_)
//          +24  int current_size_
//          +28  int allocated_size_
//          +32  int total_size_ (starts at 4, kInitialSize)
//          +40  void* initial_space_[4]
//   +72  int has_bits_[1] (zeroed by SharedCtor, swapped by Swap)
#ifndef CAVER_PROTO_SOUNDLIBRARY_STUB_H
#define CAVER_PROTO_SOUNDLIBRARY_STUB_H

// The layout stub that used to live here (80 bytes: vptr + cached_size_,
// the effects RepeatedPtrField at +16, has_bits_ at +72) was a second,
// divergent definition of Caver::Proto::SoundLibrary alongside the
// generated one in proto/Common.pb.h. Both spellings measured sizeof 80
// against the golden operator new(0x50), but only one may exist: the
// methods-only variant of this stub in core/EngineSystems.h was sizeof 8
// and corrupted CaverShell::InitView's stack (see the comment there).
// This header now forwards to the generated class; the layout notes above
// remain the recovered golden offsets the generated fields occupy.
#include "../proto/Common.pb.h"

namespace Caver {
namespace Proto {

} // namespace Proto
} // namespace Caver

#endif
