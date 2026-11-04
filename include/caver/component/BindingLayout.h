// Caver::Binding is the real aggregate in core/Binding.h (proven
// 48 bytes by std::vector<Binding>::__push_back_slow_path @0x327508;
// m_property +0, m_name +8, m_field32 +32, m_valueType +36,
// m_field40 +40). The layout stub that used to live here defined a
// second, divergent Caver::Binding (identifier / name / field32 /
// field36 / found) and is retired: this header forwards to the real
// class so no translation unit can see both spellings at once.
#ifndef CAVER_BINDING_LAYOUT_STUB_H
#define CAVER_BINDING_LAYOUT_STUB_H

#include "../core/Binding.h"

#endif
