#include "DBStatement.hpp"

namespace ECE141 {
// AppStement methods

DBStatement::DBStatement(Keywords aStatementType) : Statement(aStatementType) {}

DBStatement::~DBStatement() {}

void DBStatement::setDBName(std::string dbName) { this->DBName = dbName; }
std::string DBStatement::getDBName() const { return this->DBName; }

}  // namespace ECE141