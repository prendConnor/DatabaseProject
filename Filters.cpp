//
//  Filters.hpp
//  Assignement6
//
//  Created by rick gessner on 5/4/20.
//  Copyright © 2020 rick gessner. All rights reserved.
//

#include "Filters.hpp"

#include <stdio.h>

#include "Row.hpp"
#include "Schema.hpp"

namespace ECE141 {

Filters::Filters() : andCond(false), orCond(false) {}

Filters::~Filters() {}

bool Filters::matches(KeyValues &aList) const {
    /*for(auto exps : expressions) {
        for(auto listIt : aList) {
            new
        }
    }*/

    return false;
}

std::string &Filters::getLhs(int aPos = 0) const {
    return this->expressions[aPos]->lhs;
}

std::string &Filters::getRhs(int aPos = 0) const {
    return this->expressions[aPos]->rhs;
}

void Filters::setOr(bool newVal) { this->orCond = newVal; }
void Filters::setAnd(bool newVal) { this->andCond = newVal; }
bool Filters::getOr() const { return this->orCond; }
bool Filters::getAnd() const { return this->andCond; }

Filters &Filters::add(Expression *anExpression) {
    this->expressions.push_back(anExpression);
    return *this;
}

}  // namespace ECE141
