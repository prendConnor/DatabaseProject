//
//  Database.cpp
//  Database1
//
//  Created by rick gessner on 4/12/19.
//  Copyright © 2019 rick gessner. All rights reserved.
//

#include "Database.hpp"

#include <sstream>

#include "Storage.hpp"
#include "View.hpp"

// this class represents the database object.
// This class should do actual database related work,
// we called upon by your db processor or commands

namespace ECE141 {

Database::Database(const std::string aName, CreateNewStorage)
    : name(aName), storage(aName, CreateNewStorage{}) {}

Database::Database(const std::string aName, OpenExistingStorage)
    : name(aName), storage(aName, OpenExistingStorage{}) {}

Database::~Database() {}

StatusResult Database::createDatabase(std::string &aName) {
    StatusResult retStatus = Errors::noError;
    std::string defPath(StorageInfo::getDefaultStoragePath());

    std::string dbToCreate = defPath + "/" + aName;

    if(dbCreated()) {
        retStatus = Errors::databaseExists;
    } else {
        StorageBlock headerBlock(BlockType::meta_block);

        std::ofstream newDBFile;
        newDBFile.open(dbToCreate);
        newDBFile.close();
        this->storage.writeBlock(headerBlock, 0);
    }

    return retStatus;
}

StatusResult Database::describeDatabase(std::string &aName) {
    StatusResult retStatus = Errors::noError;
    std::string defPath(StorageInfo::getDefaultStoragePath());

    std::string dbToDescribe = defPath + "/" + aName;

    if(!dbCreated()) {
        retStatus = Errors::unknownDatabase;
    } else {
        std::ifstream inFile;
        inFile.open(dbToDescribe);
        std::string currLine;
        int lineCount = 0;

        std::cout << "\nBlk#  Type       Other\n";
        std::cout << "-------------------------------\n";

        while(std::getline(inFile, currLine)) {
            std::cout << lineCount << "     ";

            if(currLine.size() == 0) {
                continue;
            }

            switch(currLine[0]) {
                case('T'):
                    std::cout << "Meta       ";
                    break;
                case('D'):
                    std::cout << "Data       ";
                    break;
                case('E'):
                    std::cout << "schema     ";
                    break;
                case('F'):
                    std::cout << "Free       ";
                    break;
                case('I'):
                    std::cout << "Index      ";
                    break;
                default:
                    std::cout << "Unknown    ";
                    break;
            }

            if(currLine.size() > 2) {
                std::cout << currLine.substr(1, currLine.find(" "));
            }

            lineCount++;

            std::cout << std::endl;
        }

        std::cout << std::endl << lineCount << " rows in set\n";
    }

    return retStatus;
}

StatusResult Database::dropDatabase(std::string &aName) {
    StatusResult retStatus = Errors::noError;
    std::string defPath(StorageInfo::getDefaultStoragePath());

    std::string dbToDrop = defPath + "/" + aName;

    if(!dbCreated()) {
        retStatus = Errors::unknownDatabase;
    } else {
        fs::remove(dbToDrop);
    }

    return retStatus;
}

bool Database::dbCreated() {
    std::string defPath(StorageInfo::getDefaultStoragePath());
    std::string dbPath = defPath + "/" + this->name;
    return fs::exists(dbPath);
}

}  // namespace ECE141
