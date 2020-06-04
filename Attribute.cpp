//
//  Attribute.cpp
//  Assignment4
//
//  Created by rick gessner on 4/18/20.
//  Copyright © 2020 rick gessner. All rights reserved.
//

#include "Attribute.hpp"

namespace ECE141 {

Attribute::Attribute(DataType aType) : type(aType) {}

Attribute::Attribute(const std::string& aName, DataType aType)
    : name(aName), type(aType) {
    fieldLength = -1;
    autoIncr = false;
    primKey = false;
    notNullable = false;
    defValSwitch = false;
    defValStr = "";
}

Attribute::Attribute(const Attribute& aCopy) {
    this->name = aCopy.getName();
    this->type = aCopy.getType();
    this->fieldLength = aCopy.getFieldLength();
    this->notNullable = aCopy.getNotNullable();
    this->primKey = aCopy.getPrimKey();
    this->autoIncr = aCopy.getAutoIncr();
    this->defValSwitch = aCopy.getDefValSwitch();
    this->defValStr = aCopy.getDefValStr();
}

Attribute::~Attribute() {}

Attribute& Attribute::setName(std::string& aName) {
    this->name = aName;
    return *this;
}

Attribute& Attribute::setType(DataType aType) {
    this->type = aType;
    return *this;
}

void Attribute::setDefValStr(std::string& aVal) {
    this->defValStr = aVal;
    this->defValSwitch = true;
}

bool Attribute::getDefValSwitch() const {
    return this->defValSwitch;
}

std::string Attribute::getDefValStr() const {
    return this->defValStr;
}

void Attribute::setFieldLength(int aLength) { this->fieldLength = aLength; }
void Attribute::setAutoIncr(bool aBool) { this->autoIncr = aBool; }
void Attribute::setPrimKey(bool aBool) { this->primKey = aBool; }
void Attribute::setNotNullable(bool aBool) { this->notNullable = aBool; }

}  // namespace ECE141
