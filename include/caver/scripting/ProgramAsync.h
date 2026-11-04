// ProgramAsync.h - Native coroutine and timer scheduler (replaces async.scl).
// Provides native non-blocking timers, coroutines, and wait primitives.
// Credit: Quantum Creeper / Aevora Labs.

#ifndef CAVER_PROGRAM_ASYNC_H
#define CAVER_PROGRAM_ASYNC_H

namespace Caver {

class ProgramState;

class ProgramAsync {
public:
    static void RegisterLibrary(ProgramState* state);
    static void Update(float deltaTime);
    static void Reset();
};

} // namespace Caver

#endif // CAVER_PROGRAM_ASYNC_H
