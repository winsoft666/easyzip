#pragma once

#include <istream>
#include <string>
#include <vector>

#ifdef WIN32
const std::string Separator = "\\";
#else
const std::string Separator = "/";
#endif

namespace easyzip {
void getFileCrc(std::istream& input_stream, std::vector<char>& buff, unsigned long& result_crc);
bool isLargeFile(std::istream& input_stream);
bool checkFileExists(const std::string& filename);
bool makedir(const std::string& newdir);
void removeFolder(const std::string& foldername);
std::string parentDirectory(const std::string& filepath);
std::string currentPath();
bool isDirectory(const std::string& path);
std::vector<std::string> filesFromDirectory(const std::string& path);
std::string fileNameFromPath(const std::string& path);
}  // namespace easyzip
