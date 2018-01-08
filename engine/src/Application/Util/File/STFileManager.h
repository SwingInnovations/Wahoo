#ifndef SWINGTECH1_STFILEMANAGER_H
#define SWINGTECH1_STFILEMANAGER_H

#include <iostream>
#include <fstream>
#include <string>
#include <sys/stat.h>

/**
 * Wrapper class for file operations.
 */
class STFileManager {
public:
    template<typename T> static bool Write(const std::string& path, T* param);
    template<typename T> static T* Read(const std::string& path);
    static bool DirExists(const std::string& str);
    static int CreateDir(const std::string& filePath);
};

template<typename T>
bool STFileManager::Write(const std::string &path, T *param) {
    std::fstream file(path, std::ios::binary | std::ios::out | std::ios::trunc);
    if(!file.is_open()){
        std::cerr << "Failed to open: " << path << std::endl;
        return false;
    }
    file.write((char*)param, sizeof(T));
    file.flush();
    file.close();
    return true;
}

template<typename T>
T *STFileManager::Read(const std::string &path) {
    T* ret = nullptr;
    std::fstream file(path, std::ios::binary | std::ios::in | std::ios::trunc);
    if(!file.is_open()){
        std::cerr << "Failed to open: " << path << std::endl;
        return nullptr;
    }
    file.seekg(0);
    file.read((char*)ret, sizeof(T));
    file.close();
    return ret;
}



#endif //SWINGTECH1_STFILEMANAGER_H
