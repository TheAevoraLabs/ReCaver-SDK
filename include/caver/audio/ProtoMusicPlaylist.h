// Caver::Proto::MusicPlaylist and Caver::Proto::MusicTrack are the
// generated classes in proto/Common.pb.h. The layout stub that used to
// live here is retired: the generated classes occupy the same fields
// at the same offsets (MusicTrack: resource_name_ +16, volume_ +24,
// has_bits_ +28, sizeof 0x20; MusicPlaylist: name_ +16, track_
// RepeatedPtrField +24, has_bits_ +80), and MusicPlaylist::Load / Save
// already use the generated accessors (name(), track_size(), track(i),
// add_track(), set_name(), set_resource_name(), set_volume()). This
// header forwards so existing includers keep working without a second,
// divergent definition of the classes. It also retires the stub's
// declared-not-defined google::protobuf::internal::RepeatedPtrFieldBase
// ::Reserve declaration: the real entry point lives in the protobuf
// runtime the generated code links against.
#ifndef CAVER_PROTO_MUSICPLAYLIST_STUB_H
#define CAVER_PROTO_MUSICPLAYLIST_STUB_H

#include "../proto/Common.pb.h"

#endif
