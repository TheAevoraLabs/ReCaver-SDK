// Caver::Proto::ObjectLibrary, ObjectTemplate and SceneObject are the
// generated classes in proto/Scene.pb.h, and Proto::Program is the
// generated class in proto/Common.pb.h; ObjectLibrary.cpp already uses
// the generated classes. The layout stub that used to live here is
// retired: it modelled only the fields ObjectLibrary::Load / Save
// touch, at their true offsets (ObjectLibrary 256 bytes, four
// RepeatedPtrField fields at +24/+80/+136/+192, has_bits_ +248;
// ObjectTemplate 32 bytes with scene_object_ +16; SceneObject name_
// +24; Program 48 bytes), and kept a second, divergent definition of
// those classes in the tree. This header forwards to the generated
// Scene classes so existing includers keep working without that
// duplicate; includers that also need Proto::Program include
// proto/Common.pb.h directly, as ObjectLibrary.cpp does.
#ifndef CAVER_PROTO_OBJECTLIBRARY_STUB_H
#define CAVER_PROTO_OBJECTLIBRARY_STUB_H

#include "../proto/Scene.pb.h"

#endif
