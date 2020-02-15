#include <fstream>
#include <iostream>
#include <map>
#include <ostream>
#include <string>
#include <vector>
#include "gtest/gtest.h"
#include "tools.h"
#include "easyzip/easyzip.h"
using namespace easyzip;

TEST(StringZipTest, Test1) {
  if (IsFileExists("ziptest.zip"))
    std::remove("ziptest.zip");

  Zipper zipper("ziptest.zip");

  std::stringstream strdata;
  strdata << "test string data compression";

  zipper.add(strdata, "strdata");
  zipper.close();

  Unzipper unzipper("ziptest.zip");

  EXPECT_TRUE(unzipper.entries().size() == 1);
  EXPECT_TRUE(unzipper.entries().front().name == "strdata");

  unzipper.extract();

  EXPECT_TRUE(IsFileExists("strdata"));

  std::ifstream testfile("strdata");
  EXPECT_TRUE(testfile.good());

  std::string test((std::istreambuf_iterator<char>(testfile)), std::istreambuf_iterator<char>());
  testfile.close();
  EXPECT_TRUE(test == "test string data compression");

  std::map<std::string, std::string> alt_names;
  alt_names["strdata"] = "alternative_strdata.dat";

  unzipper.extract("", alt_names);

  EXPECT_TRUE(IsFileExists("alternative_strdata.dat"));

  std::ifstream testfile2("alternative_strdata.dat");
  EXPECT_TRUE(testfile2.good());

  std::string test2((std::istreambuf_iterator<char>(testfile2)), std::istreambuf_iterator<char>());
  testfile2.close();
  EXPECT_TRUE(test2 == "test string data compression");

  EXPECT_TRUE(false == unzipper.extractEntry("fake.dat"));

  unzipper.close();

  std::remove("strdata");
  std::remove("alternative_strdata.dat");
  std::remove("ziptest.zip");
}

TEST(FileZipTest, Test1) {
  if (IsFileExists("ziptest.zip"))
    std::remove("ziptest.zip");

  Zipper zipper("ziptest.zip");

  std::ofstream test1("test1.txt");
  test1 << "test file compression";
  test1.flush();
  test1.close();

  std::ifstream test1stream("test1.txt");

  // add test1.txt
  zipper.add(test1stream, "test1.txt");

  test1stream.close();
  zipper.close();

  std::remove("test1.txt");

  Unzipper unzipper("ziptest.zip");

  std::vector<ZipEntry> entries = unzipper.entries();
  EXPECT_TRUE(entries.size() == 1);
  EXPECT_TRUE(entries.front().name == "test1.txt");

  unzipper.extractEntry("test1.txt");
  // due to sections forking or creating different stacks we need to make
  // sure the local instance is closed to prevent mixing the closing when
  // both instances are freed at the end of the scope
  unzipper.close();

  EXPECT_TRUE(IsFileExists("test1.txt"));

  std::ifstream testfile("test1.txt");
  EXPECT_TRUE(testfile.good());

  std::string test((std::istreambuf_iterator<char>(testfile)), std::istreambuf_iterator<char>());
  testfile.close();
  EXPECT_TRUE(test == "test file compression");

  std::ofstream test2("test2.dat");
  test2 << "other data to compression test";
  test2.flush();
  test2.close();

  std::ifstream test2stream("test2.dat");

  zipper.open();
  // add TestFolder/test2.dat
  zipper.add(test2stream, "TestFolder/test2.dat");
  zipper.close();

  test2stream.close();
  std::remove("test2.dat");

  do {
    Unzipper unzipper("ziptest.zip");

    EXPECT_TRUE(unzipper.entries().size() == 2);
    EXPECT_TRUE(unzipper.entries().front().name == "test1.txt");
    EXPECT_TRUE(unzipper.entries()[1].name == "TestFolder/test2.dat");

    unzipper.extract();
    unzipper.close();

    EXPECT_TRUE(IsFileExists("TestFolder/test2.dat"));

    std::ifstream testfile("TestFolder/test2.dat");
    EXPECT_TRUE(testfile.good());

    std::string file_content((std::istreambuf_iterator<char>(testfile)),
                             std::istreambuf_iterator<char>());
    testfile.close();
    EXPECT_TRUE(file_content == "other data to compression test");

    MakeDir(GetCurrentPath() + "/TestFiles/subfolder");
    std::ofstream test("TestFiles/test1.txt");
    test << "test file compression";
    test.flush();
    test.close();

    std::ofstream test1("TestFiles/test2.pdf");
    test1 << "test file compression";
    test1.flush();
    test1.close();

    std::ofstream test2("TestFiles/subfolder/test-sub.txt");
    test2 << "test file compression";
    test2.flush();
    test2.close();

    zipper.open();
    // add TestFiles folder
    zipper.add("TestFiles");
    zipper.close();

    do {
      Unzipper unzipper("ziptest.zip");
      EXPECT_TRUE(unzipper.entries().size() == 5);

      MakeDir(GetCurrentPath() + "/NewDestination");

      unzipper.extract(GetCurrentPath() + "/NewDestination");

      std::vector<std::string> files = GetFilesFromDir(GetCurrentPath() + "/NewDestination");

      EXPECT_TRUE(IsFileExists("NewDestination/TestFiles/test1.txt"));
      EXPECT_TRUE(IsFileExists("NewDestination/TestFiles/test2.pdf"));
      EXPECT_TRUE(IsFileExists("NewDestination/TestFiles/subfolder/test-sub.txt"));

      unzipper.close();
    } while (false);

    RemoveDir("TestFolder");
    RemoveDir("TestFiles");
    RemoveDir("NewDestination");
    std::remove("test1.txt");
  } while (false);

  std::remove("ziptest.zip");
}

TEST(MemoryZipTest, Test1) {
  std::stringstream output;
  Zipper zipper(output);

  std::stringstream strdata;
  strdata << "test string data compression";

  EXPECT_TRUE(zipper.add(strdata, "str"));
  zipper.close();

  std::stringstream strdata2;
  Unzipper unzipper(output);
  EXPECT_TRUE(unzipper.extractEntryToStream("str", strdata2));
  unzipper.close();

  EXPECT_TRUE(strdata.str() == strdata2.str());
}