// Recovered from libswordigo.so (1.4.13) evidence.
//
// GUIFileDialog is a pure interface in this build: the golden binary
// contains NO vtable, RTTI or type-name for Caver::GUIFileDialog itself
// (verified by readelf over the full symbol table — only the
// boost::shared_ptr<GUIFileDialog> / sp_counted_impl_p<GUIFileDialog>
// instantiations exist). No method of the class is defined in the
// binary, which is why no vtable was ever emitted for it.
//
// The only factory in the binary, Caver::GUIFileDialog::New()
// @0x51FDCC (4 bytes, a tail branch), forwards to the free function
// Caver::NewGUIFileDialogImpl() @0x53F638 (8 bytes: return 0). The
// concrete implementation therefore lives in a platform layer that is
// not linked into this Android build, and New() always returns null
// here.
//
// The interface shape is pinned by the only call sites, in
// CaverShell::InitView @0x321DB4, which calls the returned object
// through its vtable:
//   +0  (slot 0) called with no arguments         -> Open()
//   +16 (slot 2) called with two std::strings,
//        "Scene files" and "*.scene"              -> SetFilter(...)
//   +24 (slot 3) returns a std::string whose
//        emptiness is tested, then assigned from  -> SelectedFilePath()
// Slot 1 (+8) is never called anywhere in the binary: UNRECOVERED.
// The destructor's slot position is likewise UNRECOVERED (the stub in
// GUIViewController.h guessed +32/+40; this header keeps a virtual
// destructor because shared_ptr<GUIFileDialog> disposal in the binary
// deletes through the concrete type, but its slot cannot be verified
// from this binary).
//
// Layout: only the vptr is observable in this build; the object is
// only ever handled through a pointer here, so sizeof is UNRECOVERED.
//
// NOTE: GUIViewController.h still carries an older stub declaration of
// this class; the gui coordinator should drop that stub in favour of
// this header (both cannot be included in one TU).
#ifndef CAVER_GUIFILEDIALOG_H
#define CAVER_GUIFILEDIALOG_H

#include <string>

namespace Caver {

class GUIFileDialog {
public:
    static GUIFileDialog* New();

    virtual void Open() = 0;                 // slot 0 (+0)
    virtual void UnrecoveredSlot1() = 0;     // slot 1 (+8) UNRECOVERED
    virtual void SetFilter(const std::string& description,
                           const std::string& pattern) = 0;  // slot 2 (+16)
    virtual const std::string& SelectedFilePath() = 0;       // slot 3 (+24)
    virtual ~GUIFileDialog();  // slot position UNRECOVERED
};

// Platform factory: returns nullptr in this build (see above).
GUIFileDialog* NewGUIFileDialogImpl();

} // namespace Caver

#endif
