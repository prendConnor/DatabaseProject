#include "AppStatement.hpp"

namespace ECE141 {
// AppStement methods

AppStatement::AppStatement(Keywords aStatementType)
    : Statement(aStatementType), nextWord(), hasNextWord(false) {}

AppStatement::~AppStatement() {}

AppStatement::AppStatement(const AppStatement& toCopy) {
    this->stmtType = toCopy.stmtType;
    this->hasNextWord = toCopy.hasNextWord;
    this->nextWord = toCopy.nextWord;
}

void AppStatement::operator=(const AppStatement& lhs) {
    this->stmtType = lhs.stmtType;
    this->nextWord = lhs.nextWord;
    this->hasNextWord = lhs.hasNextWord;
}

bool AppStatement::checkNextWord() const { return this->hasNextWord; }

void AppStatement::setNextWord(Keywords newKw) {
    this->nextWord = newKw;
    this->hasNextWord = true;
}

Keywords AppStatement::getNextWord() const { return this->nextWord; }

}  // namespace ECE141