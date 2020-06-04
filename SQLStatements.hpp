#ifndef SQLStatement_hpp
#define SQLStatement_hpp

#include <iostream>
#include <sstream>
#include <vector>

#include "Filters.hpp"
#include "Helpers.hpp"
#include "Schema.hpp"
#include "Statement.hpp"
#include "Tokenizer.hpp"
#include "keywords.hpp"

namespace ECE141 {
class UseTableStatement : public Statement {
   public:
    UseTableStatement(std::string aName);
    virtual ~UseTableStatement();

    void setDBName(std::string dbName);
    std::string getDBName() const;

   private:
    std::string DBName;
};

class ShowTables : public Statement {
   public:
    ShowTables(std::string dbName);
    ~ShowTables();

    std::string getDBName();

   private:
    std::string dbName;
    bool index;
};

class DropTableStatement : public Statement {
   public:
    DropTableStatement(std::string aName);
    ~DropTableStatement();

    std::string getTableName() const;

   private:
    std::string tableName;
};

class DescribeTableStatement : public Statement {
   public:
    DescribeTableStatement(std::string aTableName);
    ~DescribeTableStatement();

    std::string getTableName() const;

   private:
    std::string tableName;
};

class InsertStatement : public Statement {
   public:
    InsertStatement(std::string aName);
    ~InsertStatement();

    std::string getTableName() const;
    std::vector<std::string> getKeys() const;
    std::vector<std::vector<std::string>> getValues() const;
    void parseTokenizer(Tokenizer& aTokenizer);
    StatusResult getParseError() const { return this->parseError; }
    // parse and validate input

   private:
    std::string tableName;
    std::vector<std::vector<std::string>> values;
    std::vector<std::string> keys;
    Errors parseError;
};

class DeleteStatement : public Statement {
   public:
    DeleteStatement(std::string aTableName);
    ~DeleteStatement();

    std::string getTableName() const;

   private:
    std::string tableName;
};

class SelectStatement : public Statement {
   public:
    SelectStatement();
    ~SelectStatement();

    void addToken(Token& aToken);
    std::vector<Token> getTokens() const;

   private:
    std::string tableName;
    std::vector<Token> tokens;
};

class UpdateStatement : public Statement {
   public:
    UpdateStatement();
    ~UpdateStatement();

    void addToken(Token& aToken);
    std::string getName() const;
    std::string getKey() const { return this->key; }
    std::string getNewVal() const { return this->newVal; }
    unsigned int filterCount() const;
    std::vector<Token> getTokens() const;
    StatusResult parseTokens();
    StatusResult buildExpressions();

    std::string& getLhs(int aPos = 0) const;
    std::string& getRhs(int aPos = 0) const;

    bool getOr() const;
    bool getAnd() const;

   private:
    std::string tableName;
    std::string key;
    std::string newVal;
    std::vector<Token> tokens;
    Filters filter;
};

class CreateTableStatement : public Statement {
   public:
    CreateTableStatement(std::string aName, Schema* schema = nullptr);
    ~CreateTableStatement();

    std::string getSchemaName() const;
    Errors checkErrStatus() const;
    Schema& getNewSchema() const { return *newSchema; }

    void parseTokenizer(Tokenizer& aTokenizer);
    void parseAttributes(std::vector<Token>& attributes);
    bool checkNotNullable(std::vector<Token>& attributes);
    bool checkPrimaryKey(std::vector<Token>& attributes);
    bool checkAutoIncr(std::vector<Token>& attributes);
    bool checkDefault(std::vector<Token>& attributes, Attribute& attr);

   private:
    Schema* newSchema;
    std::string schemaName;
    Errors errStatus;
};

}  // namespace ECE141

#endif