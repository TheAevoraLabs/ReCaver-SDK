// Recovered from libswordigo.so (1.4.13) evidence — see docs/CORE_HEADERS.md.
// Shell preferences store: typed key/value pairs read by the shell every
// frame (view size, scale factors, behaviour switches).
//
// Evidence (0x50C644 ctor): the whole object is zero-initialised — six int
// slots at +0..+23 and four float slots at +24..+39 — so every default is 0.
// The accessors are plain array indexing (0x50C654/0x50C65C int,
// 0x50C664/0x50C670 float); Get is const (the mangled names carry K).
#ifndef CAVER_FWSHELLPREFERENCES_H
#define CAVER_FWSHELLPREFERENCES_H

namespace Caver {

// The enum type names are part of the mangled names of the accessors
// (_ZNK5Caver18FWShellPreferences3GetENS_19FWShellIntPrefsEnumE and
// ...ENS_21FWShellFloatPrefsEnumE) and are therefore fixed.
// The individual enumerator names are not evidenced by the binary.
enum FWShellIntPrefsEnum {
    FWShellIntPrefsViewWidth = 0,
    FWShellIntPrefsViewHeight = 1,
    FWShellIntPrefsWindowWidth = 2,
    FWShellIntPrefsWindowHeight = 3,
    FWShellIntPrefsUnknown4 = 4,
    FWShellIntPrefsUnknown5 = 5
};

enum FWShellFloatPrefsEnum {
    FWShellFloatPrefsUnknown0 = 0,
    FWShellFloatPrefsViewScale = 1,
    FWShellFloatPrefsUnknown2 = 2,
    FWShellFloatPrefsUnknown3 = 3
};

class FWShellPreferences {
public:
    FWShellPreferences();

    // Asymmetric on purpose: the int getter is const in the golden
    // (_ZNK...3GetENS_19FWShellIntPrefsEnumE) while the float getter is not
    // (_ZN...3GetENS_21FWShellFloatPrefsEnumE, no K qualifier).
    int Get(FWShellIntPrefsEnum key) const;
    float Get(FWShellFloatPrefsEnum key);
    void Set(FWShellIntPrefsEnum key, int value);
    void Set(FWShellFloatPrefsEnum key, float value);

private:
    int m_intValues[6];     // +0
    float m_floatValues[4]; // +24
};

} // namespace Caver

#endif
