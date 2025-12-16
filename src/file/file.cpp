#include "file.h"
#include <iostream>
#include <vector> 

// Constructor
FileManager::FileManager() {}

// Helper function definition
File* FileManager::getFile(const std::string &name)
{
    for(auto &f : files)
    {
        if(f.name == name)
            return &f;
    }
    return nullptr;
}

void FileManager::createFile(const std::string &name, const std::string &permissions)
{
    if(getFile(name))
    {
        std::cout << "File already exists: " << name << "\n";
        return;
    }
    files.push_back({name, "", permissions});
    std::cout << "File created: " << name << " with permissions: " << permissions << "\n";
}

void FileManager::writeFile(const std::string &name, const std::string &content)
{
    File* f = getFile(name);
    if(f)
    {
        if(f->permissions.find("w") != std::string::npos)
        {
            f->content = content;
            std::cout << "Content written to file: " << name << "\n";
        }
        else
            std::cout << "Write permission denied for file: " << name << "\n";
    }
    else
        std::cout << "File not found: " << name << "\n";
}

std::string FileManager::readFile(const std::string &name)
{
    File* f = getFile(name);
    if(f)
    {
        if(f->permissions.find("r") != std::string::npos)
        {
            std::cout << "File read successfully: " << name << "\n";
            return f->content;
        }
        else
            std::cout << "Read permission denied for file: " << name << "\n";
    }
    else
        std::cout << "File not found: " << name << "\n";
    return "";
}

void FileManager::deleteFile(const std::string &name)
{
    for(size_t i=0; i<files.size(); i++)
    {
        if(files[i].name == name)
        {
            files.erase(files.begin() + i);
            std::cout << "File deleted: " << name << "\n";
            return;
        }
    }
    std::cout << "File not found: " << name << "\n";
}

std::vector<File> FileManager::listFiles() const
{
    return files;
}