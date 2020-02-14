#include "tools.h"
#include <algorithm>
#include <cstdio>
#include <iostream>
#include <iterator>
#include "defs.h"
#include "filesystem.hpp"

namespace easyzip {
/* calculate the CRC32 of a file,
because to encrypt a file, we need known the CRC32 of the file before */

void getFileCrc(std::istream& input_stream, std::vector<char>& buff, unsigned long& result_crc) {
  unsigned long calculate_crc = 0;
  unsigned long size_read = 0;
  unsigned long total_read = 0;

  do {
    input_stream.read(buff.data(), buff.size());
    size_read = (unsigned long)input_stream.gcount();

    if (size_read > 0)
      calculate_crc = crc32(calculate_crc, (const unsigned char*)buff.data(), size_read);

    total_read += size_read;

  } while (size_read > 0);

  input_stream.clear();
  input_stream.seekg(0, std::ios_base::beg);
  result_crc = calculate_crc;
}

bool isLargeFile(std::istream& input_stream) {
  ZPOS64_T pos = 0;
  input_stream.seekg(0, std::ios::end);
  pos = input_stream.tellg();
  input_stream.seekg(0);

  return pos >= 0xffffffff;
}

bool checkFileExists(const std::string& filename) {
  return filesystem::exists(filename);
}

bool makedir(const std::string& newdir) {
  return filesystem::create_directories(newdir);
}

void removeFolder(const std::string& foldername) {
  filesystem::remove_all(foldername);
}

bool isDirectory(const std::string& path) {
  return filesystem::is_directory(path);
}

std::string parentDirectory(const std::string& filepath) {
  filesystem::path p(filepath);
  return p.parent_path().string();
}

std::string currentPath() {
  return filesystem::current_path().string();
}

std::vector<std::string> filesFromDirectory(const std::string& path) {
  std::vector<std::string> v;
  filesystem::recursive_directory_iterator itr(path);
  for (itr = filesystem::begin(itr); itr != filesystem::end(itr); itr++) {
    if (!filesystem::is_directory(itr->path())) {
      v.push_back(itr->path().string());
    }
  }

  return v;
}

std::string fileNameFromPath(const std::string& fullPath) {
  filesystem::path p(fullPath);
  return p.filename().string();
}
}  // namespace easyzip
