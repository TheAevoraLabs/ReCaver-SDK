// Recovered from libswordigo.so (1.4.13) evidence — see docs/CORE_HEADERS.md.
// Resource path and string helper free functions used across the engine.
#ifndef CAVER_RESOURCEFETCH_H
#define CAVER_RESOURCEFETCH_H

#include <string>
#include <vector>

namespace Caver {

bool FileExistsAtPath(const std::string& path);
void* NewByteBufferFromAndroidAsset(const std::string& path, unsigned int* outSize);
std::string DocumentsDirectoryPath();
// Golden 0x53AF28 / 0x53BB6C / 0x53BC44. CachesDirectoryPath
// appends "Caches" to the same files-dir base DocumentsDirectoryPath
// uses (both golden bodies read the same global); the PathFor*
// helpers append a file name and create the directory on request.
std::string CachesDirectoryPath();
std::string PathForDocumentsDirectory(const std::string& file, bool createDirectory);
std::string PathForCachesDirectory(const std::string& file, bool createDirectory);
std::string PathForResourceOfType(const std::string& name, const std::string& type);
std::string StringByAppendingPathComponent(const std::string& path, const std::string& component);
std::string StringByRemovingPathExtension(const std::string& path);
std::string LastPathComponent(const std::string& path);
std::string FindFileFromParentDirectories(const std::string& file, std::string startDirectory);
void SetResourcesPath(const std::string& path);
std::string ResourcesPath();
std::string UniqueIdentifier();
void GetFilesWithExtension(const std::string& extension,
                           const std::string& directory,
                           std::vector<std::string>* files);

} // namespace Caver

#endif
