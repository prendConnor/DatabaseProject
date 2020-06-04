#ifndef AppStatement_hpp
#define AppStatement_hpp

#include <iostream>

#include "Statement.hpp"
#include "keywords.hpp"

namespace ECE141 {
class AppStatement : public Statement {
   public:
    AppStatement(Keywords aStatementType);
    virtual ~AppStatement();
    AppStatement(const AppStatement& toCopy);
    void operator=(const AppStatement& lhs);
    bool checkNextWord() const;
    void setNextWord(Keywords newKw);
    Keywords getNextWord() const;

   private:
    Keywords nextWord;
    bool hasNextWord;
};

}  // namespace ECE141

#endif  // AppStatement