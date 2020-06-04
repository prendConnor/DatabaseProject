//
//  Value.hpp
//  RGAssignment5
//
//  Created by rick gessner on 4/26/20.
//  Copyright © 2020 rick gessner. All rights reserved.
//

#ifndef Value_h
#define Value_h

#include <cstdint>
#include <map>
#include <optional>
#include <string>
#include <variant>

#include "Errors.hpp"

namespace ECE141 {

struct Storable {
    virtual StatusResult encode(std::ostream &aWriter) = 0;
    virtual StatusResult decode(std::istream &aReader) = 0;
};

enum class DataType {
    no_type = 'N',
    bool_type = 'B',
    datetime_type = 'D',
    float_type = 'F',
    int_type = 'I',
    varchar_type = 'V',
};

using ValueType = std::variant<uint32_t, float, bool, std::string>;

using KeyValues = std::map<const std::string, ValueType>;

}  // namespace ECE141

#endif /* Value_h */
