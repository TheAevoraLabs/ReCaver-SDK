// Recovered from libswordigo.so (1.4.13) evidence — IDA 0x5308E4 / 0x530968.
// Layout (20 bytes): a presence-flag word at +0 (bits 1/2/4/8, one per
// field, mirroring the protobuf has-bits) followed by four floats at
// +4/+8/+12/+16. LoadFromProtobufMessage copies each present field from
// the Proto::GUIMargins message (floats at +12/+16/+20/+24, has-bits at
// +28) and sets the matching flag; SaveToProtobufMessage is the mirror.
#ifndef CAVER_GUIMARGINS_H
#define CAVER_GUIMARGINS_H

namespace Caver {

namespace Proto {
class GUIMargins;
} // namespace Proto

struct GUIMargins {
    int flags;      // +0
    float field1;   // +4
    float field2;   // +8
    float field3;   // +12
    float field4;   // +16

    void LoadFromProtobufMessage(const Proto::GUIMargins& message);
    void SaveToProtobufMessage(Proto::GUIMargins* message) const;
};

} // namespace Caver

#endif
