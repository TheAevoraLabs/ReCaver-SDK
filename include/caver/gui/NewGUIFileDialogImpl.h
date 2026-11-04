// Recovered from libswordigo.so (1.4.13) evidence — IDA 0x53F638:
// NewGUIFileDialogImpl is a free factory function (not a class) whose
// whole body is `return 0;` — the Android build has no file-dialog
// implementation.
#ifndef CAVER_NEWGUIFILEDIALOGIMPL_H
#define CAVER_NEWGUIFILEDIALOGIMPL_H

namespace Caver {

class GUIFileDialogImpl;

GUIFileDialogImpl* NewGUIFileDialogImpl();

} // namespace Caver

#endif
