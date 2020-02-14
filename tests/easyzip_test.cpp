#include <fstream>
#include <iostream>
#include <map>
#include <ostream>
#include <string>
#include <vector>
#include "gtest/gtest.h"
#include "../src/tools.h"
#include "easyzip/easy_unzip.h"
#include "easyzip/easy_zip.h"


TEST(StringZipTest, Test1) {
  using namespace easyzip;
  if (checkFileExists("ziptest.zip"))
    std::remove("ziptest.zip");

  Zipper zipper("ziptest.zip");

  std::stringstream strdata;
  strdata << "test string data compression";

  zipper.add(strdata, "strdata");
  zipper.close();

  Unzipper unzipper("ziptest.zip");

  {
    EXPECT_TRUE(unzipper.entries().size() == 1);
    EXPECT_TRUE(unzipper.entries().front().name == "strdata");

    {
      unzipper.extract();

      EXPECT_TRUE(checkFileExists("strdata"));

      std::ifstream testfile("strdata");
      EXPECT_TRUE(testfile.good());

      std::string test((std::istreambuf_iterator<char>(testfile)),
                       std::istreambuf_iterator<char>());
      testfile.close();
      EXPECT_TRUE(test == "test string data compression");

      {
        std::map<std::string, std::string> alt_names;
        alt_names["strdata"] = "alternative_strdata.dat";

        unzipper.extract("", alt_names);

        EXPECT_TRUE(checkFileExists("alternative_strdata.dat"));

        std::ifstream testfile2("alternative_strdata.dat");
        EXPECT_TRUE(testfile2.good());

        std::string test2((std::istreambuf_iterator<char>(testfile2)),
                          std::istreambuf_iterator<char>());
        testfile2.close();
        EXPECT_TRUE(test2 == "test string data compression");

        { EXPECT_TRUE(false == unzipper.extractEntry("fake.dat")); }
      }

      unzipper.close();
    }
  }

  std::remove("strdata");
  std::remove("alternative_strdata.dat");
  std::remove("ziptest.zip");
}

TEST(FileZipTest, Test1) {
  using namespace easyzip;
  if (checkFileExists("ziptest.zip"))
    std::remove("ziptest.zip");

  Zipper zipper("ziptest.zip");

  {
    std::ofstream test1("test1.txt");
    test1 << "test file compression";
    test1.flush();
    test1.close();

    std::ifstream test1stream("test1.txt");

    zipper.add(test1stream, "test1.txt");

    test1stream.close();
    zipper.close();

    std::remove("test1.txt");

    Unzipper unzipper("ziptest.zip");

    {
      std::vector<ZipEntry> entries = unzipper.entries();
      EXPECT_TRUE(entries.size() == 1);
      EXPECT_TRUE(entries.front().name == "test1.txt");

      {
        unzipper.extractEntry("test1.txt");
        // due to sections forking or creating different stacks we need to make
        // sure the local instance is closed to prevent mixing the closing when
        // both instances are freed at the end of the scope
        unzipper.close();

        EXPECT_TRUE(checkFileExists("test1.txt"));

        std::ifstream testfile("test1.txt");
        EXPECT_TRUE(testfile.good());

        std::string test((std::istreambuf_iterator<char>(testfile)),
                         std::istreambuf_iterator<char>());
        testfile.close();
        EXPECT_TRUE(test == "test file compression");

        {
          std::ofstream test2("test2.dat");
          test2 << "other data to compression test";
          test2.flush();
          test2.close();

          std::ifstream test2stream("test2.dat");

          zipper.open();
          zipper.add(test2stream, "TestFolder/test2.dat");
          zipper.close();

          test2stream.close();
          std::remove("test2.dat");

          Unzipper unzipper("ziptest.zip");

          {
            EXPECT_TRUE(unzipper.entries().size() == 2);
            EXPECT_TRUE(unzipper.entries().front().name == "test1.txt");
            EXPECT_TRUE(unzipper.entries()[1].name == "TestFolder/test2.dat");

            {
              unzipper.extract();
              unzipper.close();

              EXPECT_TRUE(checkFileExists("TestFolder/test2.dat"));

              std::ifstream testfile("TestFolder/test2.dat");
              EXPECT_TRUE(testfile.good());

              std::string test((std::istreambuf_iterator<char>(testfile)),
                               std::istreambuf_iterator<char>());
              testfile.close();
              EXPECT_TRUE(test == "other data to compression test");

              {
                makedir(currentPath() + "/TestFiles/subfolder");
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
                zipper.add("TestFiles");
                zipper.close();

                Unzipper unzipper("ziptest.zip");
                EXPECT_TRUE(unzipper.entries().size() == 5);

                {
                  makedir(currentPath() + "/NewDestination");

                  unzipper.extract(currentPath() + "/NewDestination");

                  std::vector<std::string> files = filesFromDirectory(
                      currentPath() + "/NewDestination");

                  EXPECT_TRUE(
                      checkFileExists("NewDestination/TestFiles/test1.txt"));
                  EXPECT_TRUE(
                      checkFileExists("NewDestination/TestFiles/test2.pdf"));
                  EXPECT_TRUE(checkFileExists(
                      "NewDestination/TestFiles/subfolder/test-sub.txt"));
                }

                unzipper.close();
              }
            }
          }

          removeFolder("TestFolder");
          removeFolder("TestFiles");
          removeFolder("NewDestination");
          std::remove("test1.txt");
        }
      }
    }

    std::remove("ziptest.zip");
  }
}