// Recovered from libswordigo.so (1.4.13) evidence — AddToContext
// @0x517758 (jni-small SF-1). The class carries no state the golden
// body reads (the context arrives as the argument); it is the
// factory that installs the four built-in programs (ids 0..3) into
// a RenderingContext.
#ifndef CAVER_BASICRENDERINGPROGRAMS_H
#define CAVER_BASICRENDERINGPROGRAMS_H

namespace Caver {

class RenderingContext;

class BasicRenderingPrograms {
public:
    static void AddToContext(RenderingContext* context);
};

} // namespace Caver

#endif
