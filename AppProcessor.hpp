//
//  AppProcessor.hpp
//  Database5
//
//  Created by rick gessner on 4/4/20.
//  Copyright © 2020 rick gessner. All rights reserved.
//

#ifndef AppProcessor_hpp
#define AppProcessor_hpp

#include <stdio.h>

#include <iostream>
#include <map>
#include <memory>
#include <vector>

#include "AppStatement.hpp"
#include "CommandProcessor.hpp"
#include "Helpers.hpp"
#include "Tokenizer.hpp"

namespace ECE141 {

class AppCmdProcessor : public CommandProcessor {
   public:
    typedef StatusResult (AppCmdProcessor::*commandPointer)(
        const AppStatement &);
    AppCmdProcessor(CommandProcessor *aNext = nullptr);
    virtual ~AppCmdProcessor();

    virtual Statement *getStatement(Tokenizer &aTokenizer);
    virtual StatusResult interpret(const Statement &aStatement);

    void buildMap();         // build command map
    void addHelpKWs();       // add valid kws to kw vector for help
    void buildHelpStrMap();  // build kw to str map
    void printHelpStatment(const Keywords &currKW, const std::string &kwStr);
    StatusResult helpCommand(const AppStatement &aStatement);
    StatusResult quitCommand(const AppStatement &aStatement);
    StatusResult versionCommand(const AppStatement &aStatement);

   private:
    std::map<ECE141::Keywords, commandPointer> commandMap;
    std::map<ECE141::Keywords, std::string> helpStrings;
    std::vector<ECE141::Keywords> helpKWs;
    AppStatement* statementPtr;
};

}  // namespace ECE141

#endif /* AppProcessor_hpp */
