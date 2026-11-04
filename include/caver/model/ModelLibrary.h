// Recovered from libswordigo.so (1.4.13) evidence.
// Singleton library of loaded models/animations: three std::map members
// back to back (24 bytes each, object size 72 — see sharedLibrary's
// operator new(0x48)). Map types pinned by ModelForName /
// AnimationForModel:
//   +0   map<string, shared_ptr<const Model>>            model cache
//   +24  map<const Model*, shared_ptr<PODLoader>>        the loader that
//        produced each model (kept alive with the model)
//   +48  map<string, shared_ptr<const KeyframeAnimation>> animation
//        cache, keyed by "<model name>|<animation name>"
#ifndef CAVER_MODELLIBRARY_H
#define CAVER_MODELLIBRARY_H

#include <boost/shared_ptr.hpp>
#include <map>
#include <string>

namespace Caver {

class KeyframeAnimation;
class Model;
class PODLoader;

class ModelLibrary {
public:
    static ModelLibrary* sharedLibrary();
    static void SetSharedLibrary(ModelLibrary* library);

    void Clear();

    boost::shared_ptr<const Model> ModelForName(const std::string& name);
    boost::shared_ptr<const KeyframeAnimation> AnimationForModel(
        const Model* model, const std::string& name);

private:
    std::map<std::string, boost::shared_ptr<const Model> > m_map0;   // +0
    std::map<const Model*, boost::shared_ptr<PODLoader> > m_map24;   // +24
    std::map<std::string, boost::shared_ptr<const KeyframeAnimation> >
        m_map48;                                                     // +48

    static ModelLibrary* s_sharedLibrary;
};

} // namespace Caver

#endif
