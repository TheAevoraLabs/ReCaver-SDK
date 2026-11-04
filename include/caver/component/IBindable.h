// Recovered from libswordigo.so (1.4.13) evidence — see docs/CORE_HEADERS.md.
// Property-binding interface. Component's RTTI names it as the offset-16
// base; its vtable (recovered from the secondary vtable inside
// Component's _ZTV, in slot order) has exactly the six methods below and
// no destructor entries, so the interface declares no virtual dtor.
#ifndef CAVER_IBINDABLE_H
#define CAVER_IBINDABLE_H

#include <vector>

namespace Caver {

struct Binding;
class BindingValue;

class IBindable {
public:
    virtual void UpdateBindingDependencies() = 0;
    virtual void GetBindings(std::vector<Binding>* bindings) = 0;
    virtual void GetEnumValuesForBindedProperty(int property, std::vector<BindingValue>* values) = 0;
    virtual BindingValue ValueForBindedProperty(int property) = 0;
    virtual void SetValueForBindedProperty(int property, const BindingValue& value) = 0;
    virtual void PerformBindedAction(int action) = 0;
};

} // namespace Caver

#endif
