#include "FolderListenerTester.hpp"

namespace ECE141 {
FolderListenerTester::FolderListenerTester() {}

bool FolderListenerTester::operator()(const std::string &aName) {
    nameVec.push_back(aName);
    return true;
}

int FolderListenerTester::getNameNum() { return nameVec.size(); }
std::string FolderListenerTester::getIndex(int index) { return nameVec[index]; }

bool FolderListenerTester::sanityTest() {
    std::string defaultPath = StorageInfo::getDefaultStoragePath();
    std::string testDirStr = "testDir";

    std::string testStr(defaultPath + testDirStr);
    setUpTestDir(testStr);


    FolderListenerTester listener;
    FolderReader reader(testStr.c_str());

    reader.each(listener, "ext");
    closeTestDir(testStr);

    return (listener.getNameNum() == 0);
}

void FolderListenerTester::setUpTestDir(std::string path, int num) {
    fs::create_directory(path);
    if(num == 0) {
        return;
    } else {
        for(int i = 0; i < num; i++) {
            std::string newFile =
                (std::string("/FileName") + std::to_string(i));
            std::string fileToCreate(path + newFile);
            std::ofstream newDB;
            newDB.open(fileToCreate);
            newDB.close();
        }
    }
}

void FolderListenerTester::closeTestDir(std::string path) {
    fs::remove_all(path);
}

bool FolderListenerTester::testNumOfFiles(int num) {
    std::string defaultPath = StorageInfo::getDefaultStoragePath();
    std::string testDirStr = "testDir";

    std::string testStr(defaultPath + testDirStr);
    setUpTestDir(testStr, num);

    FolderListenerTester listener;
    FolderReader reader(testStr.c_str());

    reader.each(listener, "ext");
    closeTestDir(testStr);

    return (listener.getNameNum() == num);
}

}  // namespace ECE141