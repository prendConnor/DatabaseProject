//
//  Database.hpp
//  ECEDatabase
//
//  Created by rick gessner on 3/30/18.
//  Copyright © 2018 rick gessner. All rights reserved.
//

#ifndef Database_hpp
#define Database_hpp

#include <stdio.h>

#include <filesystem>
#include <iostream>
#include <string>

#include "Storage.hpp"

namespace ECE141 {

namespace fs = std::filesystem;

class Database {
   public:
    Database(const std::string aPath, CreateNewStorage);
    Database(const std::string aPath, OpenExistingStorage);
    ~Database();

    StatusResult createDatabase(std::string& aName);
    StatusResult describeDatabase(std::string& aName);
    StatusResult dropDatabase(std::string& aName);

    bool dbCreated();

    Storage& getStorage() { return storage; }
    std::string& getName() { return name; }

   protected:
    std::string name;
    Storage storage;
};

}  // namespace ECE141

#endif /* Database_hpp */
