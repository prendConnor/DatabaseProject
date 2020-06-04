//
//  Attribute.hpp
//  Assignment4
//
//  Created by rick gessner on 4/18/20.
//  Copyright © 2020 rick gessner. All rights reserved.
//

#ifndef Attribute_hpp
#define Attribute_hpp

#include <stdio.h>

#include <iostream>
#include <string>

#include "Value.hpp"

namespace ECE141 {

/*enum class DataType {
    no_type = 'N',
    bool_type = 'B',
    datetime_type = 'D',
    float_type = 'F',
    int_type = 'I',
    varchar_type = 'V',
};*/

class Attribute {
   protected:
    std::string name;
    DataType type;
    int fieldLength;
    bool autoIncr;
    bool primKey;
    bool notNullable;
    bool defValSwitch;
    std::string defValStr;

   public:
    Attribute(DataType aType = DataType::no_type);
    Attribute(const std::string& aName, DataType aType);
    Attribute(const Attribute& aCopy);
    ~Attribute();

    Attribute& setName(std::string& aName);
    Attribute& setType(DataType aType);
    void setFieldLength(int aLength);
    void setAutoIncr(bool aBool);
    void setPrimKey(bool aBool);
    void setNotNullable(bool aBool);

    int getFieldLength() const { return fieldLength; }
    bool getNotNullable() const { return notNullable; }
    bool getPrimKey() const { return primKey; }
    bool getAutoIncr() const { return autoIncr; }

    void setDefValStr(std::string& aVal);
    bool getDefValSwitch() const;
    std::string getDefValStr() const;

    bool isValid();  // is this schema valid? Are all the attributes value?

    const std::string& getName() const { return name; }
    DataType getType() const { return type; }

    // STUDENT: are there other getter/setters to manage other attribute
    // properties?
};

}  // namespace ECE141

#endif /* Attribute_hpp */
