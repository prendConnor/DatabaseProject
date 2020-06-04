//
//  Schema.cpp
//  Assignment4
//
//  Created by rick gessner on 4/18/20.
//  Copyright © 2020 rick gessner. All rights reserved.
//

#include "Schema.hpp"

namespace ECE141 {

Schema::Schema(const std::string& aName) : name(aName) {}
Schema::~Schema() {}

Schema::Schema(const Schema& aCopy) {
    this->attributes = aCopy.attributes;
    this->name = aCopy.name;
    this->blockNum = aCopy.blockNum;
    this->changed = aCopy.changed;
}

void Schema::operator=(const Schema& rhs) {
    this->attributes = rhs.attributes;
    this->name = rhs.name;
    this->blockNum = rhs.blockNum;
    this->changed = rhs.changed;
}

Schema& Schema::addAttribute(const Attribute& anAttribute) {
    this->attributes.push_back(anAttribute);
    return *this;
}

const std::string& Schema::getName() const { return name; }
const AttributeList& Schema::getAttributes() const { return attributes; }
uint32_t Schema::getBlockNum() const { return blockNum; }
bool Schema::isChanged() { return changed; }
unsigned int Schema::getAttrNum() const { return this->attributes.size(); }

StatusResult Schema::validAttribute(std::string& attrName) const {
    for(auto iter : this->attributes) {
        if(iter.getName() == attrName) {
            return Errors::noError;
        }
    }

    return Errors::unknownAttribute;
}

}  // namespace ECE141
