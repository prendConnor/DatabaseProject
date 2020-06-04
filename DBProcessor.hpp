

#ifndef DBProcessor_hpp
#define DBProcessor_hpp

#include <stdio.h>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <vector>

#include "CommandProcessor.hpp"
#include "DBStatement.hpp"
#include "Database.hpp"
#include "Helpers.hpp"
#include "Storage.hpp"
#include "Tokenizer.hpp"

namespace ECE141 {

class DBProcessor : public CommandProcessor {
   public:
    typedef StatusResult (DBProcessor::*commandPointer)(
        const DBStatement&) const;
    DBProcessor(CommandProcessor* aNext = nullptr);
    virtual ~DBProcessor();

    virtual Statement* getStatement(Tokenizer& aTokenizer);
    virtual StatusResult interpret(const Statement& aStatement);

    void buildMap();  // build command map
    StatusResult create(const DBStatement& dbStatement) const;
    StatusResult show(const DBStatement& dbStatement) const;
    StatusResult use(const DBStatement& dbStatement) const;
    StatusResult describe(const DBStatement& dbStatement) const;
    StatusResult drop(const DBStatement& dbStatement) const;

    bool fileExists(std::string name) const;

   private:
    std::map<ECE141::Keywords, commandPointer> commandMap;
    Statement* statementPtr;
};

}  // namespace ECE141

#endif  // DBProcessor_hpp