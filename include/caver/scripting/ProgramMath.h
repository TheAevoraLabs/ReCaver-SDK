// Recovered from libswordigo.so (1.4.13) evidence.
// Registration + stack helpers for the engine's math types in Lua.
// All six golden entry points are declared; the library bodies live
// in ProgramMath.cpp (recovered 2026-10-24 from the IDA exports and
// the registration tables in the golden's relocations).
#ifndef CAVER_PROGRAMMATH_H
#define CAVER_PROGRAMMATH_H

namespace Caver {

class ProgramState;
class Rectangle;
class Vector3;

class ProgramMath {
public:
    static void RegisterLibraries(ProgramState* state);
    static void RegisterVector3Library(ProgramState* state);
    static void RegisterRectangleLibrary(ProgramState* state);
    static void RegisterMathLibrary(ProgramState* state);
    static void PushVector3(ProgramState* state, const Vector3& value);
    static void PushRectangle(ProgramState* state, const Rectangle& value);
};

} // namespace Caver

#endif
