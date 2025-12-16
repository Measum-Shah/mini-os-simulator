#ifndef FILE_H
#define FILE_H

#include <string>
#include <vector> // CRITICAL: Defines std::vector

// Struct definition (must be visible to FileManager)
struct File {
    std::string name;
    std::string content;
    std::string permissions; // e.g., "rw", "r", "w"
};

// Class declaration (must be complete)
class FileManager {
private:
    std::vector<File> files; // 'File' and 'std::vector' must be defined here
    File* getFile(const std::string &name); 
public:
    FileManager();
    void createFile(const std::string &name, const std::string &permissions);
    void writeFile(const std::string &name, const std::string &content);
    std::string readFile(const std::string &name);
    void deleteFile(const std::string &name);
    std::vector<File> listFiles() const;
    const std::vector<File>& getFiles() const { return files; } 
};

#endif // FILE_H