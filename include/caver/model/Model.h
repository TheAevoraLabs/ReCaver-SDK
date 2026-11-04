// Recovered from libswordigo.so (1.4.13) evidence.
//
// Layout (proven by ~Model and by ModelInstance::InitWithModel / Draw):
//   +0   std::string                          name
//   +24  std::vector<shared_ptr<Mesh>>        meshes
//   +48  boost::shared_ptr<const Skeleton>    skeleton
// sizeof = 64. ~Model's body is exactly the member teardown (string,
// vector of shared_ptrs released back-to-front, skeleton shared_ptr),
// so the destructor is defaulted here.
#ifndef CAVER_MODEL_H
#define CAVER_MODEL_H

#include <boost/shared_ptr.hpp>
#include <string>
#include <vector>

namespace Caver {

class Mesh;
class Skeleton;

class Model {
public:
    ~Model();

    std::string m_name;                                   // +0
    std::vector<boost::shared_ptr<Mesh> > m_meshes;       // +24
    boost::shared_ptr<const Skeleton> m_skeleton;         // +48
};

} // namespace Caver

#endif
