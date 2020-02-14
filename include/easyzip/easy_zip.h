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
  // clang-format off
		// Minizip options/params:
		//              -o                -a             -0            -1             -9             -j
		enum zipFlags { Overwrite = 0x01, Append = 0x02, Store = 0x04, Faster = 0x08, Better = 0x10, NoPaths = 0x20, SaveHierarchy = 0x40 };
  // clang-format on
  Zipper(std::iostream& buffer);
  Zipper(std::vector<unsigned char>& buffer);
  Zipper(const std::string& zipname);
  Zipper(const std::string& zipname, const std::string& password);

  ~Zipper();

  bool add(std::istream& source,
           const std::tm& timestamp,
           const std::string& nameInZip = std::string(),
           zipFlags flags = Better);
  bool add(std::istream& source,
           const std::string& nameInZip = std::string(),
           zipFlags flags = Better);
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
#endif // !EASY_ZIP_H_
