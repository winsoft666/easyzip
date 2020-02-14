#ifndef EASY_ZIP_H_
#define EASY_ZIP_H_
#pragma once

#include <ctime>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

namespace easyzip {
class Zipper {
 public:
  enum zipFlags {
    Overwrite = 0x01,  // -o
    Append = 0x02,     // -a
    Store = 0x04,      // -0
    Faster = 0x08,     // -1
    Better = 0x10,     // -9
    NoPaths = 0x20,    // -j
    SaveHierarchy = 0x40
  };
  Zipper(std::iostream& buffer);
  Zipper(std::vector<unsigned char>& buffer);
  Zipper(const std::string& zipname);
  Zipper(const std::string& zipname, const std::string& password);

  ~Zipper();

  bool add(std::istream& source,
           const std::tm& timestamp,
           const std::string& nameInZip,
           zipFlags flags = Better);
  bool add(std::istream& source, const std::string& nameInZip, zipFlags flags = Better);
  bool add(const std::string& fileOrFolderPath, zipFlags flags = Better);

  void open();
  void close();

 private:
  void release();

  std::string m_password;
  std::string m_zipname;
  std::iostream& m_obuffer;
  std::vector<unsigned char>& m_vecbuffer;
  bool m_usingMemoryVector;
  bool m_usingStream;
  bool m_open;

  struct Impl;
  Impl* m_impl;
};
}  // namespace easyzip
#endif  // !EASY_ZIP_H_
