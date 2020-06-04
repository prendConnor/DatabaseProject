#ifndef DBStatement_hpp
#define DBStatement_hpp

#include <iostream>

#include "Statement.hpp"
#include "keywords.hpp"

namespace ECE141 {
class DBStatement : public Statement {
   public:
    DBStatement(Keywords aStatementType);
    ~DBStatement();

    void setDBName(std::string dbName);
    std::string getDBName() const;

   private:
    std::string DBName;
};

}  // namespace ECE141

#endif  // AppStatement