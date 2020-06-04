//
//  Row.hpp
//  Assignment4
//
//  Created by rick gessner on 4/19/20.
//  Copyright © 2020 rick gessner. All rights reserved.
//

#ifndef Row_hpp
#define Row_hpp

#include <stdio.h>

#include <string>
#include <utility>
#include <variant>
#include <vector>

#include "Database.hpp"
#include "Storage.hpp"

namespace ECE141 {

class Row {
   public:
    Row();
    Row(const Row &aRow);
    ~Row();
    Row &operator=(const Row &aRow);
    bool operator==(Row &aCopy) const;

    void addToRow(std::string &key, std::string &value);
    KeyValues &getPairs() { return this->kvPairs; }
    const std::string &getValue(std::string &key) {
        return std::get<std::string>(kvPairs[key]);
    }

    void setValue(std::string &key, std::string &newVal);
    KeyValues &getKVs();

    // STUDENT: What other methods do you require?

   protected:
    KeyValues kvPairs;
};

}  // namespace ECE141

#endif /* Row_hpp */
