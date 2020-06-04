//
//  Storage.hpp
//  Assignment2
//
//  Created by rick gessner on 4/5/20.
//  Copyright © 2020 rick gessner. All rights reserved.
//

#ifndef Storage_hpp
#define Storage_hpp

#include <stdio.h>

#include <algorithm>
#include <fstream>
#include <map>
#include <string>
#include <variant>

#include "Errors.hpp"
#include "Schema.hpp"
#include "StorageBlock.hpp"
#include "Value.hpp"

namespace ECE141 {

// first, some utility classes...

class StorageInfo {
   public:
    static const char *getDefaultStoragePath();
};

struct CreateNewStorage {};
struct OpenExistingStorage {};

/*struct Storable {
    virtual StatusResult encode(std::ostream &aWriter) = 0;
    virtual StatusResult decode(std::istream &aReader) = 0;
    virtual BlockType getType() const = 0;  // what kind of block is this?
};*/

// USE: Our storage manager class...
class Storage {
   public:
    Storage(const std::string aName, CreateNewStorage);
    Storage(const std::string aName, OpenExistingStorage);
    ~Storage();
    uint32_t getTotalBlockCount();

    // high-level IO (you're not required to use this, but it may help)...
    StatusResult save(Storable &aStorable);  // using a stream api
    StatusResult load(Storable &aStorable);  // using a stream api

    // low-level IO...
    StatusResult readBlock(StorageBlock &aBlock, uint32_t aBlockNumber);
    StatusResult writeBlock(StorageBlock &aBlock, uint32_t aBlockNumber);

    StatusResult readSchema(std::vector<Schema> &schemaPtr);
    StatusResult removeEntries(std::string tableName, bool dropTable = false);

   protected:
    bool isReady() const;

    std::string name;
    StatusResult findFreeBlockNum();
    std::fstream stream;
};

}  // namespace ECE141

#endif /* Storage_hpp */
