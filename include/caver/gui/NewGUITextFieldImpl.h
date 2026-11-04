// Recovered from libswordigo.so (1.4.13) evidence — IDA 0x53F640:
// NewGUITextFieldImpl is a free factory function (not a class):
// operator new(0x1B8) followed by the GUITextFieldImpl constructor.
//
// This header must NOT define the class itself. An earlier version
// declared a stub `class GUITextFieldImpl { GUITextFieldImpl(); };`
// here, so the factory's `new GUITextFieldImpl` allocated a single
// byte while the real constructor (from GUITextFieldImpl.cpp, same
// mangled name) built the 440-byte object into it — a heap
// buffer overflow in the first base constructor. Every
// GUITextField::New() caller crashed: the rename prompt and the
// delete prompt, i.e. both save-slot crashes. The class lives only
// in GUITextFieldImpl.h; this header forward-declares it.
#ifndef CAVER_NEWGUITEXTFIELDIMPL_H
#define CAVER_NEWGUITEXTFIELDIMPL_H

namespace Caver {

class GUITextFieldImpl;

GUITextFieldImpl* NewGUITextFieldImpl();

} // namespace Caver

#endif
