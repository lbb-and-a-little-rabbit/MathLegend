#include "physfs_assistant.h"

std::vector<char> LoadFile(const std::string& path){
    PHYSFS_File* file = PHYSFS_openRead(path.c_str());
    if (!file) {
        throw std::runtime_error("File not found in pak: " + path);
    }

    PHYSFS_sint64 fileSize = PHYSFS_fileLength(file);

    std::vector<char> buffer(fileSize);
    PHYSFS_readBytes(file, buffer.data(), fileSize);

    PHYSFS_close(file);

    return buffer;
}