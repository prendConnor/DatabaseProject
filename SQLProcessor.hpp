//
//  SQLProcessor.hpp
//  Assignment4
//
//  Created by rick gessner on 4/18/20.
//  Copyright © 2020 rick gessner. All rights reserved.
//

#ifndef SQLProcessor_hpp
#define SQLProcessor_hpp

#include <stdio.h>

#include <vector>

#include "CommandProcessor.hpp"
#include "Database.hpp"
#include "RowCollection.hpp"
#include "SQLStatements.hpp"
#include "Schema.hpp"
#include "Timer.hpp"
#include "Tokenizer.hpp"
#include "View.hpp"

class Statement;
class Database;  // define this later...

namespace ECE141 {

class SQLProcessor : public CommandProcessor {
   public:
    SQLProcessor(CommandProcessor *aNext = nullptr);
    virtual ~SQLProcessor();

    virtual Statement *getStatement(Tokenizer &aTokenizer);
    virtual StatusResult interpret(const Statement &aStatement);

    StatusResult createTable(const Schema &aSchema);
    StatusResult dropTable(const std::string &aName);
    StatusResult describeTable(const std::string &aName) const;
    StatusResult showTables() const;
    StatusResult useTable(const UseTableStatement &statement);

    void printTableNames(std::vector<std::string> &names);
    bool tableExists(const std::string &tableName) const;
    void schemaToStorageBlock(const Schema &aSchema, std::string &dataString);
    StatusResult dataSetToStorageBlock(const std::vector<std::string> &vals,
                                       std::vector<std::string> &keys,
                                       std::string &tableName,
                                       std::string &strToStore) const;

    StatusResult insert(const InsertStatement &aStatement) const;
    // StatusResult update();
    StatusResult deleteFromTable(const DeleteStatement &aStatement) const;
    StatusResult selectFromTable(const SelectStatement &aStatement) const;

    bool checkDBKW(Tokenizer &aTokenizer, Keywords &kw);
    bool getTableByName(std::string &aName, Schema &ref) const;
    StatusResult attributeCheck(std::vector<std::string> attrs,
                                Schema &schemaToCheck) const;
    StatusResult attributeCheck(std::string attrs, Schema &schemaToCheck) const;
    void addToCollection(Row &aRow, std::string &schemaName) const;
    StatusResult update(const UpdateStatement &aStatement);
    void printIndeces(std::vector<std::string> &names);

   protected:
    Database *openDB;
    std::vector<Schema> openSchema;
    std::vector<RowCollection *> dataSets;
    Statement *statementPtr;  // used to free statement
    Errors errStat;
    bool index;
};

}  // namespace ECE141
#endif /* SQLProcessor_hpp */
