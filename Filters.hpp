//
//  Filters.hpp
//  RGAssignment6
//
//  Created by rick gessner on 5/4/20.
//  Copyright © 2020 rick gessner. All rights reserved.
//

#ifndef Filters_h
#define Filters_h

#include <stdio.h>

#include <string>
#include <vector>

#include "Errors.hpp"
#include "Tokenizer.hpp"
#include "Value.hpp"

namespace ECE141 {

class Row;
class Schema;

/*struct xOperand {
    Operand() {}
    Operand(std::string &aName, TokenType aType, ValueType &aValue,
            uint32_t anId = 0)
        : type(aType), name(aName), value(aValue), entityId(anId) {}

    TokenType type;  // so we know if it's a field, a const (number, string)...
    std::string name;  // for named attr. in schema
    ValueType value;
    uint32_t entityId;
};*/

//---------------------------------------------------

struct Expression {
    std::string lhs;
    std::string rhs;
    std::string op;

    Expression(std::string &aLHSOperand, std::string anOp,
               std::string &aRHSOperand)
        : lhs(aLHSOperand), rhs(aRHSOperand), op(anOp) {}

    bool operator()(KeyValues &aList);
};

//---------------------------------------------------

using Expressions = std::vector<Expression *>;

//---------------------------------------------------

class Filters {
   public:
    Filters();
    Filters(const Filters &aFilters);
    ~Filters();

    size_t getCount() const { return expressions.size(); }
    bool matches(KeyValues &aList) const;
    Filters &add(Expression *anExpression);

    std::string &getLhs(int aPos) const;
    std::string &getRhs(int aPos) const;

    void setOr(bool newVal);
    void setAnd(bool newVal);

    bool getOr() const;
    bool getAnd() const;

    friend class Tokenizer;

   protected:
    Expressions expressions;
    bool andCond;
    bool orCond;
};

}  // namespace ECE141

#endif /* Filters_h */
