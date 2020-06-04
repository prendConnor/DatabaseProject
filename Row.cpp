///
//  Row.cpp
//  Assignment4
//
//  Created by rick gessner on 4/19/20.
//  Copyright © 2020 rick gessner. All rights reserved.
//

#include "Row.hpp"

namespace ECE141 {

// STUDENT: You need to fully implement these methods...

Row::Row() { this->kvPairs = {}; }

Row::Row(const Row &aRow) { this->kvPairs = aRow.kvPairs; }

Row &Row::operator=(const Row &aRow) {
    this->kvPairs = aRow.kvPairs;
    return *this;
}

bool Row::operator==(Row &aCopy) const {
    return this->kvPairs == aCopy.kvPairs;
}

Row::~Row() {}

void Row::addToRow(std::string &key, std::string &value) {
    this->kvPairs.insert(std::pair<std::string, std::string>(key, value));
}

void Row::setValue(std::string &key, std::string &newVal) {
    kvPairs[key] = newVal;
}

KeyValues &Row::getKVs() { return this->kvPairs; }

}  // namespace ECE141
