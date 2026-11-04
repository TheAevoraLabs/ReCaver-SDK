// Recovered from libswordigo.so (1.4.13) evidence.
// A named group of scene objects (editor/runtime organisation: hiding,
// locking, script hooks). Layout from the ctor/dtor/AddObject bodies:
//   +0   ReferenceCountedObject base (vptr, refcount at +8)
//   +16  IBindable base (vptr)
//   +24  Scene* m_scene (plain pointer, set by SetScene)
//   +32  std::string m_identifier
//   +56  boost::shared_ptr<Program> m_program (run by FinishLoad with
//        the identifier as argument)
//   +72  FastVector<SceneObject*> m_objects (count +72, capacity +76,
//        data +80 — growth doubles from a floor of 10, hand-inlined
//        in AddObject)
//   +88  bool fields x4 (one at +90 initialises true; the binding
//        properties read +88/+89/+91 individually — names pending)
#ifndef CAVER_SCENEOBJECTGROUP_H
#define CAVER_SCENEOBJECTGROUP_H

#include <string>
#include <vector>
#include <boost/shared_ptr.hpp>

#include "../component/ReferenceCountedObject.h"
#include "../component/IBindable.h"
#include "FastVector.h"

namespace Caver {

class Binding;
class BindingValue;
class Program;
class Scene;
class SceneObject;

namespace Proto { class SceneObjectGroup; }

class SceneObjectGroup : public ReferenceCountedObject, public IBindable {
public:
    SceneObjectGroup();
    virtual ~SceneObjectGroup();

    void UpdateBindingDependencies();
    void GetBindings(std::vector<Binding>* bindings);
    void GetEnumValuesForBindedProperty(int property, std::vector<BindingValue>* values);
    BindingValue ValueForBindedProperty(int property);
    void SetValueForBindedProperty(int property, const BindingValue& value);
    void PerformBindedAction(int action);

    void LoadFromProtobufMessage(const Proto::SceneObjectGroup& message);
    void SaveToProtobufMessage(Proto::SceneObjectGroup* message) const;
    void FinishLoad();

    void SetScene(Scene* scene);
    void SetIdentifier(const std::string& identifier);
    void AddObject(SceneObject* object);
    void RemoveObject(SceneObject* object);
    void RemoveObjectAtIndex(int index);
    void RemoveAllObjects();

    bool IsHidden() const { return m_hidden; }
    void SetHidden(bool hidden) { m_hidden = hidden; }
    bool Bool90() const { return m_bool90; }
    void SetBool90(bool val) { m_bool90 = val; }

private:
    Scene* m_scene = 0;                         // +24
    std::string m_identifier;                   // +32
    boost::shared_ptr<Program> m_program;       // +56
    FastVector<SceneObject*> m_objects;         // +72
    bool m_hidden = false;                      // +88 — binding "hidden"
    bool m_locked = false;                      // +89 — binding "locked"
    bool m_bool90 = true;                       // +90 — semantics UNRECOVERED (not editor-bound; defaults true)
    bool m_updateAll = false;                   // +91 — binding "updateAll"
    int m_activationFrame = 0;                  // +92 — Scene::ActivateObject frame stamp (updateAll cascade guard)
};

} // namespace Caver

#endif
