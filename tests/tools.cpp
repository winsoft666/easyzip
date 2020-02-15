#include "tools.h"
#include <algorithm>
#include <cstdio>
#include <iostream>
#include <iterator>
#include "../src/filesystem.hpp"

using namespace easyzip;
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
