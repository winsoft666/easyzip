#pragma once

#include <istream>
#include <string>
#include <vector>

bool IsFileExists(const std::string& filename);
bool MakeDir(const std::string& newdir);
void RemoveDir(const std::string& foldername);
std::string GetParentDir(const std::string& filepath);
std::string GetCurrentPath();
bool IsDirectory(const std::string& path);
std::vector<std::string> GetFilesFromDir(const std::string& path);
std::string GetFileNameFromPath(const std::string& path);