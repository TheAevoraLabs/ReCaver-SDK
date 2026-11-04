// Recovered from libswordigo.so (1.4.13) evidence — see
// docs/CORE_HEADERS.md and the components slice board.
// Completed by the components slice (run 3): the three classes'
// bodies are plain std::map / std::vector operations in the golden
// (the tree walks in RegisterComponentInterface / IsInterfacesManaged
// / ManageInterface are libc++ __tree internals), with these layouts:
//   ComponentCollection: a std::vector<intrusive_ptr<Component>>
//     alone at +0 (24 bytes) — AddComponent is its push_back.
//   ComponentManager: a std::map<long, ComponentCollection*> —
//     node value (the collection POINTER) sits at node+40; entries
//     inserted by ManageInterface start null and are materialised by
//     ComponentCollectionForInterface. (This supersedes the earlier
//     {begin,end} stub struct, which was the vector's head misread
//     as the whole class.)
//   ComponentFactory: the creators map (tree header at +0) plus a
//     std::vector<std::string> of registered names at +24.
#ifndef CAVER_COMPONENTMANAGER_H
#define CAVER_COMPONENTMANAGER_H

#include <map>
#include <set>
#include <string>
#include <vector>
#include <boost/function.hpp>
#include <boost/intrusive_ptr.hpp>

namespace Caver {

class Component;

class ComponentCollection {
public:
    template<typename T>
    struct Collection {
        void* m_f0;
        void* m_f8;
        T* m_begin;
        void* m_f24;
        T* m_end;
    };

    void AddComponent(Component* component);
    void Purge();
    void PurgeInactive();

    std::vector<boost::intrusive_ptr<Component>> m_components;   // +0
};

class ComponentManager {
public:
    ComponentManager();
    ~ComponentManager();

    void Purge();
    void PurgeInactive();
    // Called by Component::RegisterInterface (a plain tail call in
    // the golden binary: manager, interfaceId, component). Adds the
    // component only when the interface is already managed.
    void RegisterComponentInterface(long interfaceId, Component* component);
    bool IsInterfacesManaged(long interfaceId);
    ComponentCollection* ComponentCollectionForInterface(long interfaceId);
    void ManageInterface(long interfaceId);
    template<typename T>
    ComponentCollection::Collection<T*> ComponentsWithInterface();

    std::set<long> m_managedInterfaces;                 // +0 (24 bytes)
    std::map<long, ComponentCollection*> m_collections; // +24 (24 bytes)
};

class ComponentFactory {
public:
    static ComponentFactory* sharedManager();
    ~ComponentFactory();

    void RegisterComponentClass(const std::string& className,
                                boost::function<Component*()> creator);
    Component* NewComponentWithClassName(const std::string& className);

    std::map<std::string, boost::function<Component*()>> m_creators;  // +0
    std::vector<std::string> m_classNames;                            // +24 in the golden
};

class DefaultComponents {
public:
    static void RegisterAll();
};

} // namespace Caver

#endif
