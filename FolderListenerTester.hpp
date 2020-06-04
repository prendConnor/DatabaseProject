#include <iostream>
#include <fstream>

#include "FolderReader.hpp"
#include "Storage.hpp"

#ifndef FolderListenerTester_hpp
#define FolderListenerTester_hpp

namespace ECE141 {

class FolderListenerTester : public FolderListener {
   public:
    FolderListenerTester();
    bool operator()(const std::string &aName);

    int getNameNum();
    std::string getIndex(int index);

    bool sanityTest();

    bool testNumOfFiles(int num);
    void setUpTestDir(std::string path, int num = 0);
    void closeTestDir(std::string path);

   private:
    std::vector<std::string> nameVec;
};

}  // namespace ECE141

#endif