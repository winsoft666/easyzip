#pragma once

#include <istream>
#include <string>
#include <vector>

namespace easyzip {
void GetFileCrc(std::istream& input_stream, std::vector<char>& buff, unsigned long& result_crc);
bool IsLargeFile(std::istream& input_stream);
bool IsFileExists(const std::string& filename);
bool MakeDir(const std::string& newdir);
void RemoveDir(const std::string& foldername);
std::string GetParentDir(const std::string& filepath);
std::string GetCurrentPath();
bool IsDirectory(const std::string& path);
std::vector<std::string> GetFilesFromDir(const std::string& path);
std::string GetFileNameFromPath(const std::string& path);
std::string GetLastDirNameFromPath(const std::string& path); // the path must a dir
}  // namespace easyzip
