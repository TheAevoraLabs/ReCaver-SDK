// Recovered from libswordigo.so (1.4.13) evidence.
// Declaration only — the primitive builders' definitions live with
// the model lane's MeshPrimitives.cpp (not yet recovered).
#ifndef CAVER_MESHPRIMITIVES_H
#define CAVER_MESHPRIMITIVES_H

#include <boost/shared_ptr.hpp>

namespace Caver {

class MeshInstance;

class MeshPrimitives {
public:
    static boost::shared_ptr<MeshInstance> Cube(bool flag);
};

} // namespace Caver

#endif
