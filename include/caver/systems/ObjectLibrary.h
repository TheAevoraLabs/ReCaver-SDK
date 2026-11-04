// Recovered from libswordigo.so (1.4.13) evidence.
// A named library of object templates, textures and script programs,
// with named imports of other libraries. Layout established from the
// inlined constructor in LibraryWithName (0x4EF128, operator
// new(0x80)), the destructor (0x4F04CC) and the field accesses across
// the class's own functions:
//
//   +0    std::string m_name
//   +24   std::map<std::string, ObjectTemplateInfo> m_templates
//   +48   std::vector<boost::shared_ptr<ObjectLibrary>> m_importedLibraries
//   +72   std::vector<boost::intrusive_ptr<Texture>> m_textures
//   +96   std::vector<boost::shared_ptr<Program>> m_programs
//   +120  bool m_processingGuard — set while a recursive walk
//         (TemplateForName / ContainsTexture / LoadAllPrograms) is
//         visiting imported libraries; the entry check on the same
//         byte is what breaks import cycles.
//   +121  bool m_modified — read and cleared by SaveModifiedLibraries.
//
// Object size is 128 bytes. The destructor's member order (programs,
// textures, imported libraries, template map, name) pins the
// declaration order below. The constructor was inlined at every
// construction site in the golden binary (no out-of-line symbol), so
// it is defined in-class here.
//
// The class is not polymorphic: there is no vtable for it in the
// golden binary, and the destructor exists as D1/D2 only.
//
// Two TU-local statics live in the .cpp (adjacent in the golden
// binary at 0x6516B8 / 0x6516D0): the library search path string and
// the map of loaded libraries (name -> weak_ptr).
#ifndef CAVER_OBJECTLIBRARY_H
#define CAVER_OBJECTLIBRARY_H

#include <map>
#include <string>
#include <vector>

#include <boost/intrusive_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

namespace Caver {

namespace Proto {
class ObjectLibrary;
class ObjectTemplate;
}

class ObjectTemplate;
class Texture;
class Program;
class ProgramState;
class SceneProcessingContext;

class ObjectLibrary {
public:
    // One template slot in m_templates. The intrusive member is the
    // lazily materialised ObjectTemplate (created by TemplateForName
    // from the proto member); the proto member is the serialised form
    // kept for saving. The out-of-line destructor in the golden
    // binary (0x4F1060, WEAK) is the implicit one: it releases the
    // intrusive member first, then the shared member.
    struct ObjectTemplateInfo {
        boost::shared_ptr<Proto::ObjectTemplate> protoTemplate;  // +0
        boost::intrusive_ptr<ObjectTemplate> objectTemplate;     // +16
    };

    ObjectLibrary() : m_processingGuard(false), m_modified(false) {}
    ~ObjectLibrary();

    static std::string LibrarySearchPath();
    static void SetLibrarySearchPath(const std::string& path);
    static std::string FindLibrariesDirectoryFromParentDirectories(
        const std::string& startDirectory);
    static boost::shared_ptr<ObjectLibrary> LibraryWithName(
        const std::string& name);
    static void SaveModifiedLibraries();
    static void ClearLoadedLibraries();

    void LoadFromFile(const std::string& filePath);
    void SaveToFile(const std::string& filePath);

    void ImportLibrary(const boost::shared_ptr<ObjectLibrary>& library);
    void RemoveImportedLibrary(ObjectLibrary* library);

    void LoadFromProtobufMessage(const Proto::ObjectLibrary& message);
    void SaveToProtobufMessage(Proto::ObjectLibrary* message) const;

    // Empty body in the golden binary (a bare 4-byte ret at
    // 0x4F1108): whatever it once did, this build does nothing.
    // Whether it was static is UNRECOVERED (no callers, empty body);
    // it is declared as an instance method.
    void CreateAtlasesFromTextures(
        const std::string& name, const std::string& type,
        const boost::intrusive_ptr<Texture>* textures, int textureCount);

    void Process(SceneProcessingContext* context);

    boost::intrusive_ptr<ObjectTemplate> TemplateForName(
        const std::string& name, bool searchImported);
    void AddTemplate(const boost::intrusive_ptr<ObjectTemplate>& objectTemplate);
    void RemoveTemplate(ObjectTemplate* objectTemplate);
    void GetAllTemplates(
        std::vector<boost::intrusive_ptr<ObjectTemplate>>* templates,
        bool includeImported);
    void GetAllImportedLibraries(
        std::vector<boost::shared_ptr<ObjectLibrary>>* libraries,
        bool recursive);
    bool IsLibraryImported(const std::string& name, bool recursive);

    void AddTexture(const boost::intrusive_ptr<Texture>& texture);
    void RemoveTexture(Texture* texture);
    bool ContainsTexture(Texture* texture, bool recursive);

    void AddProgram(const boost::shared_ptr<Program>& program);
    void RemoveProgram(Program* program);
    void LoadAllPrograms(ProgramState* state);

    std::string m_name;                                             // +0
    std::map<std::string, ObjectTemplateInfo> m_templates;          // +24
    std::vector<boost::shared_ptr<ObjectLibrary>> m_importedLibraries; // +48
    std::vector<boost::intrusive_ptr<Texture>> m_textures;          // +72
    std::vector<boost::shared_ptr<Program>> m_programs;             // +96
    bool m_processingGuard;                                         // +120
    bool m_modified;                                                // +121
};

} // namespace Caver

#endif
