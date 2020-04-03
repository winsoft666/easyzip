#include "tools.h"
#include <algorithm>
#include <cstdio>
#include <iostream>
#include <iterator>
#include <assert.h>
#include "defs.h"
#include "filesystem.hpp"

namespace easyzip {
/* calculate the CRC32 of a file, because to encrypt a file, we need known the CRC32 of the file before */
void GetFileCrc(std::istream& input_stream, std::vector<char>& buff, unsigned long& result_crc) {
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

bool IsLargeFile(std::istream& input_stream) {
  ZPOS64_T pos = 0;
  input_stream.seekg(0, std::ios::end);
  pos = input_stream.tellg();
  input_stream.seekg(0);

  return pos >= 0xffffffff;
}

bool IsFileExists(const std::string& filename) {
  std::error_code ec;
  return filesystem::exists(filename, ec);
}

bool MakeDir(const std::string& newdir) {
  std::error_code ec;
  return filesystem::create_directories(newdir, ec);
}

void RemoveDir(const std::string& foldername) {
  std::error_code ec;
  filesystem::remove_all(foldername, ec);
}

bool IsDirectory(const std::string& path) {
  std::error_code ec;
  return filesystem::is_directory(path, ec);
}

std::string GetParentDir(const std::string& filepath) {
  filesystem::path p(filepath);
  return p.parent_path().string();
}

std::string GetCurrentPath() {
  return filesystem::current_path().string();
}

std::vector<std::string> GetFilesFromDir(const std::string& path) {
  std::vector<std::string> v;
  filesystem::recursive_directory_iterator itr(path);
  for (itr = filesystem::begin(itr); itr != filesystem::end(itr); itr++) {
    if (!filesystem::is_directory(itr->path())) {
      v.push_back(itr->path().string());
    }
  }

  return v;
}

std::string GetFileNameFromPath(const std::string& fullPath) {
  filesystem::path p(fullPath);
  return p.filename().string();
}

std::string GetLastDirNameFromPath(const std::string& path) {
  if (path.length() == 0)
    return "";
  if (path.length() == 1 && path == ".")
    return "";
  if (path.length() == 2 && path == "..")
    return "";

  assert(IsDirectory(path));

  std::string path2 = path;

#ifdef _WIN32
  if (path2[path2.length() - 1] == '\\') {
    path2 = path2.substr(0, path2.length() - 1);
  }
#else
  if (path2[path2.length() - 1] == '/') {
    path2 = path2.substr(0, path2.length() - 1);
  }
#endif

  filesystem::path p(path2);
  return p.filename().string();
}

}  // namespace easyzip
