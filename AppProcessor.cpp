//
//  CommandProcessor.cpp
//  ECEDatabase
//
//  Created by rick gessner on 3/30/18.
//  Copyright © 2018 rick gessner. All rights reserved.
//

#include "AppProcessor.hpp"

namespace ECE141 {

class VersionStatement : public Statement {
   public:
    VersionStatement() : Statement(Keywords::version_kw) {}
};

//.....................................

AppCmdProcessor::AppCmdProcessor(CommandProcessor *aNext)
    : CommandProcessor(aNext), statementPtr(nullptr) {
    buildMap();
    addHelpKWs();
    buildHelpStrMap();
}

AppCmdProcessor::~AppCmdProcessor() {}

// USE: -----------------------------------------------------
void AppCmdProcessor::buildMap() {
    this->commandMap.emplace(Keywords::help_kw, &AppCmdProcessor::helpCommand);
    this->commandMap.emplace(Keywords::version_kw,
                             &AppCmdProcessor::versionCommand);
    this->commandMap.emplace(Keywords::quit_kw, &AppCmdProcessor::quitCommand);
}

void AppCmdProcessor::addHelpKWs() {
    this->helpKWs.push_back(ECE141::Keywords::create_kw);
    this->helpKWs.push_back(ECE141::Keywords::describe_kw);
    this->helpKWs.push_back(ECE141::Keywords::drop_kw);
    this->helpKWs.push_back(ECE141::Keywords::help_kw);
    this->helpKWs.push_back(ECE141::Keywords::quit_kw);
    this->helpKWs.push_back(ECE141::Keywords::show_kw);
    this->helpKWs.push_back(ECE141::Keywords::use_kw);
    this->helpKWs.push_back(ECE141::Keywords::version_kw);
}

void AppCmdProcessor::buildHelpStrMap() {
    helpStrings.emplace(ECE141::Keywords::create_kw, "create");
    helpStrings.emplace(ECE141::Keywords::describe_kw, "describe");
    helpStrings.emplace(ECE141::Keywords::drop_kw, "drop");
    helpStrings.emplace(ECE141::Keywords::help_kw, "help");
    helpStrings.emplace(ECE141::Keywords::quit_kw, "quit");
    helpStrings.emplace(ECE141::Keywords::show_kw, "show");
    helpStrings.emplace(ECE141::Keywords::use_kw, "use");
    helpStrings.emplace(ECE141::Keywords::version_kw, "version");
}

void AppCmdProcessor::printHelpStatment(const Keywords &currKW,
                                        const std::string &kwStr) {
    std::cout << "  " << kwStr << " -- "
              << "insert rel info here\n";
}

StatusResult AppCmdProcessor::helpCommand(const AppStatement &aStatement) {
    if(aStatement.checkNextWord()) {
        auto it = helpStrings.find(aStatement.getNextWord());
        if(it != helpStrings.end()) {
            printHelpStatment(it->first, it->second);
        } else {
            return Errors::unknownCommand;  // return unrec command if invalid
        }
    } else {
        for(const auto &[key, value] : helpStrings) {
            printHelpStatment(key, value);
        }
    }

    return Errors::noError;
}

StatusResult AppCmdProcessor::quitCommand(const AppStatement &aStatement) {
    return Errors::userTerminated;
}

StatusResult AppCmdProcessor::versionCommand(const AppStatement &aStatement) {
    std::cout << "version ECE141b-7\n";
    return Errors::noError;
}

StatusResult AppCmdProcessor::interpret(const Statement &aStatement) {
    Keywords statementType = aStatement.getType();
    StatusResult retStatus = Errors::noError;
    auto it = commandMap.find(statementType);

    const AppStatement *appStmnt =
        dynamic_cast<const AppStatement *>(&aStatement);

    if(it != commandMap.end()) {
        retStatus = (this->*(it->second))(*appStmnt);
    } else {
        std::cerr << "app command not in map\n";
    }

    /*if(statementPtr != nullptr) {
        delete(statementPtr);
        statementPtr = nullptr;
    }*/

    return StatusResult(retStatus);
}

// USE: factory to create statement based on given tokens...
Statement *AppCmdProcessor::getStatement(Tokenizer &aTokenizer) {
    std::vector<ECE141::Keywords> keywords{
        Keywords::help_kw, Keywords::version_kw, Keywords::quit_kw};

    Token currToken = aTokenizer.current();
    auto it = std::find(keywords.begin(), keywords.end(), currToken.keyword);
    if(it != keywords.end()) {
        aTokenizer.next();

        AppStatement *returnStatement = new AppStatement(currToken.keyword);

        if(aTokenizer.more()) {
            currToken = aTokenizer.current();
            auto it =
                std::find(helpKWs.begin(), helpKWs.end(), currToken.keyword);
            if(it != keywords.end()) {
                returnStatement->setNextWord(currToken.keyword);
            }
            aTokenizer.next();
        }

        statementPtr = returnStatement;
        return returnStatement;
    }

    return nullptr;
}

}  // namespace ECE141
