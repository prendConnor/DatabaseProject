//
//  FolderReader.hpp
//  Database5
//
//  Created by rick gessner on 4/4/20.
//  Copyright © 2020 rick gessner. All rights reserved.
//

#ifndef FolderReader_h
#define FolderReader_h

#include <filesystem>
#include <string>
#include <vector>

namespace ECE141 {

namespace fs = std::filesystem;

class FolderListener {
   public:
    virtual bool operator()(const std::string &aName) = 0;
};

class FolderReader {
   public:
    FolderReader(const char *aPath) : path(aPath) {}
    virtual ~FolderReader() {}

    virtual bool exists(const std::string &aPath) { return fs::exists(aPath); }

    virtual void each(FolderListener &aListener,
                      const std::string &anExtension) const {
        for(const auto &it : fs::directory_iterator(path)) {
            fs::path currPath = it.path();
            aListener(currPath.filename().string());
        }
    };

    std::string path;
};

}  // namespace ECE141

#endif /* FolderReader_h */
