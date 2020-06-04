//
//  Storage.cpp
//  Assignment2
//
//  Created by rick gessner on 4/5/20.
//  Copyright © 2020 rick gessner. All rights reserved.
//

#include "Storage.hpp"

#include <cstdlib>
#include <fstream>

namespace ECE141 {

// USE: Our main class for managing storage...
const char *StorageInfo::getDefaultStoragePath() {
    // STUDENT -- MAKE SURE TO SET AN ENVIRONMENT VAR for DB_PATH!
    //           This lets us change the storage location during autograder
    //           testing

    // WINDOWS USERS:  Use forward slash (/) not backslash (\) to separate
    // paths.
    //                (Windows convert forward slashes for you)

    const char *thePath = std::getenv("DB_PATH");
    return thePath;
}

//----------------------------------------------------------

// path to the folder where you want to store your DB's...
std::string getDatabasePath(const std::string &aDBName) {
    std::string defPath(StorageInfo::getDefaultStoragePath());

    std::string thePath = defPath + "/" + aDBName;

    return thePath;
}

// USE: ctor ---------------------------------------
Storage::Storage(const std::string aName, CreateNewStorage) : name(aName) {
    std::string thePath = getDatabasePath(name);
    // try to create a new db file in known storage location.
    // throw error if it fails...
}

// USE: ctor ---------------------------------------
Storage::Storage(const std::string aName, OpenExistingStorage) : name(aName) {
    std::string thePath = getDatabasePath(aName);
    // try to OPEN a db file a given storage location
    // if it fails, throw an error
}

// USE: dtor ---------------------------------------
Storage::~Storage() { stream.close(); }

// USE: validate we're open and ready ---------------------------------------
bool Storage::isReady() const { return stream.is_open() || true; }

// USE: count blocks in file ---------------------------------------
uint32_t Storage::getTotalBlockCount() {
    // how can we compute the total number of blocks in storage?
    uint32_t theCount = 0;
    return theCount;
}

// Call this to locate a free block in this storage file.
// If you can't find a free block, then append a new block and return its
// blocknumber
StatusResult Storage::findFreeBlockNum() {
    return StatusResult{noError};  // return blocknumber in the 'value' field...
}

// USE: for use with "storable API" [NOT REQUIRED -- but very useful]

StatusResult Storage::save(Storable &aStorable) {
    // High-level IO: save a storable object (like a table row)...
    return StatusResult{noError};
}

// USE: for use with "storable API" [NOT REQUIRED -- but very useful]

StatusResult Storage::load(Storable &aStorable) {
    // high-level IO: load a storable object (like a table row)
    return StatusResult{noError};
}

// USE: write data a given block (after seek)
StatusResult Storage::writeBlock(StorageBlock &aBlock, uint32_t aBlockNumber) {
    std::string dbPath = getDatabasePath(this->name);

    std::ofstream newStream(dbPath, std::ios::app);

    if(isReady()) {
        newStream << aBlock.header.type << aBlock.data << std::endl;
    }

    stream.close();

    return StatusResult{};
}

StatusResult Storage::removeEntries(std::string tableName, bool dropTable) {
    std::vector<char> charList = {'D'};

    if(dropTable) {
        charList.push_back('E');
    }

    std::string dbPath = getDatabasePath(this->name);
    // stream.open(dbPath, std::ios::out);

    std::vector<std::string> linesToKeep;

    std::fstream newStream;
    std::string line;

    newStream.open(dbPath);

    while(std::getline(newStream, line)) {
        auto it = std::find(charList.begin(), charList.end(), line[0]);
        if(it != charList.end() &&
           line.substr(1, line.find(" ") - 1) == tableName) {
            line.replace(line.begin(), line.end(), "");  // does nothing really
        } else {
            linesToKeep.push_back(line);
        }
    }

    newStream.close();

    std::ofstream keepStream;
    keepStream.open(dbPath, std::ios::out | std::ios::trunc);

    for(auto iter : linesToKeep) {
        keepStream << iter << std::endl;
    }

    return Errors::noError;
}

StatusResult Storage::readSchema(std::vector<Schema> &schemaPtr) {
    std::string dbPath = getDatabasePath(this->name);

    stream.open(dbPath, std::ios::in);
    bool attrs = true;

    if(isReady()) {
        while(stream.good()) {
            std::string line;
            std::string name;
            attrs = true;
            getline(stream, line);

            if(line[0] != 'E') {
                continue;
            }

            char *ptr = &line[1];

            while(*ptr != ' ') {
                std::string charHolder(1, *ptr);
                name.append(charHolder);
                ptr++;
            }

            Schema *newSchema = new Schema(name);

            // std::cerr << "name: " << name << std::endl;

            ptr++;

            while(attrs) {
                std::string attrName;
                while(*ptr != ' ') {
                    std::string charHolder(1, *ptr);
                    attrName.append(charHolder);
                    ptr++;
                }

                // std::cerr << "attr name: " << attrName << std::endl;
                ptr++;

                Attribute *newAttr = new Attribute(attrName, (DataType)*ptr);

                // std::cerr << "type char: " << *ptr << std::endl;

                ptr += 2;

                // std::cerr << "field length char: " << *ptr << std::endl;
                if(*ptr == '1') {
                    ptr += 2;
                    std::string fieldLength(1, *ptr);
                    ptr++;
                    while(*ptr != ' ') {
                        std::string charHolder(1, *ptr);
                        attrName.append(charHolder);
                        fieldLength.append(charHolder);
                        ptr++;
                    }
                    // std::cerr << "field length string: " << fieldLength
                    //<< std::endl;
                    newAttr->setFieldLength(std::stoi(fieldLength));
                    ptr++;
                } else {
                    ptr += 2;
                }

                // std::cerr << "not null char: " << *ptr << std::endl;
                if(*ptr == '1') {
                    newAttr->setNotNullable(true);
                }

                ptr += 2;

                // std::cerr << "prim key char: " << *ptr << std::endl;
                if(*ptr == '1') {
                    newAttr->setPrimKey(true);
                }

                ptr += 2;

                // std::cerr << "auto incr char: " << *ptr << std::endl;
                if(*ptr == '1') {
                    newAttr->setAutoIncr(true);
                }

                ptr += 2;

                // std::cerr << "def char: " << *ptr << std::endl;
                if(*ptr == '1') {
                    ptr += 2;
                    std::string defVal;
                    while(*ptr != ' ') {
                        std::string charHolder(1, *ptr);
                        attrName.append(charHolder);
                        defVal.append(charHolder);
                        ptr++;
                    }
                    // std::cerr << "def val: " << defVal << std::endl;
                    newAttr->setDefValStr(defVal);
                } else {
                    ptr++;
                }

                if(*ptr == ' ' && *(ptr + 1) == '/') {
                    attrs = false;
                } else {
                    ptr++;
                }

                newSchema->addAttribute(*newAttr);
            }

            schemaPtr.push_back(*newSchema);
        }
    }

    return Errors::noError;
}

// USE: read data from a given block (after seek)
StatusResult Storage::readBlock(StorageBlock &aBlock, uint32_t aBlockNumber) {
    // STUDENT: Implement this; this is your low-level block IO...
    return StatusResult{};
}

}  // namespace ECE141
