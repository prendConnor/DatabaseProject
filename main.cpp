//
//
//  main.cpp
//  Database2
//
//  Created by rick gessner on 3/17/19.
//  Copyright © 2019 rick gessner. All rights reserved.
//

#include <fstream>
#include <iostream>
#include <sstream>
#include <string.h>

#include "AppProcessor.hpp"
#include "DBProcessor.hpp"
#include "Errors.hpp"
#include "Storage.hpp"
#include "Tokenizer.hpp"
#include "FolderReader.hpp"
#include "FolderListenerTester.hpp"
#include "SQLProcessor.hpp"

#define TEST_FLAG "--test"
#define TEST_FLAG_LENGTH 6

// USE: ---------------------------------------------

static std::map<int, std::string> theErrorMessages = {
    {ECE141::illegalIdentifier, "Illegal identifier"},
    {ECE141::unknownIdentifier, "Unknown identifier"},
    {ECE141::databaseExists, "Database exists"},
    {ECE141::tableExists, "Table Exists"},
    {ECE141::syntaxError, "Syntax Error"},
    {ECE141::unknownCommand, "Unknown command"},
    {ECE141::unknownDatabase, "Unknown database"},
    {ECE141::unknownTable, "Unknown table"},
    {ECE141::unknownError, "Unknown error"}};

void showError(ECE141::StatusResult &aResult) {
    std::string theMessage = "Unknown Error";
    if(theErrorMessages.count(aResult.code)) {
        theMessage = theErrorMessages[aResult.code];
    }
    std::cout << "Error (" << aResult.code << ") " << theMessage << "\n";
}

void testMethod(bool result, std::string testName) {
    std::cout << "Test Named: " << testName;
    if(result) {
        std::cout << " passed.";
    } else {
        std::cout << " did not pass.";
    }

    std::cout << std::endl;
}

// build a tokenizer, tokenize input, ask processors to handle...
ECE141::StatusResult handleInput(std::istream &aStream,
                                 ECE141::CommandProcessor &aProcessor) {
    ECE141::Tokenizer theTokenizer(aStream);

    // tokenize the input from aStream...
    ECE141::StatusResult theResult = theTokenizer.tokenize();
    while(theResult && theTokenizer.more()) {
        if(";" == theTokenizer.current().data) {
            theTokenizer.next();  // skip the ";"...
        } else
            theResult = aProcessor.processInput(theTokenizer);
    }
    return theResult;
}

//----------------------------------------------

int main(int argc, const char *argv[]) {
    // const char* path = ECE141::StorageInfo::getDefaultStoragePath();

    if(argc == 2 && strncmp(argv[1], TEST_FLAG, TEST_FLAG_LENGTH) == 0) {

        ECE141::FolderListenerTester tester;
        testMethod(tester.sanityTest(), "SANITY TEST (0 Files)");

        ECE141::FolderListenerTester testerTwo;
        testMethod(testerTwo.testNumOfFiles(7), "TEST 7 FILES");

        return 0;
    }

    ECE141::SQLProcessor SqlProcessor;
    ECE141::DBProcessor DBProcessor(&SqlProcessor);
    ECE141::AppCmdProcessor theProcessor(&DBProcessor);
    ECE141::StatusResult theResult{};

    if(argc > 1) {
        std::ifstream theStream(argv[1]);
        return handleInput(theStream, theProcessor);
    } else {
        std::string theUserInput;
        bool running = true;
        do {
            std::cout << "\n> ";
            if(std::getline(std::cin, theUserInput)) {
                if(theUserInput.length()) {
                    std::stringstream theStream(theUserInput);
                    theResult = handleInput(theStream, theProcessor);
                    if(!theResult) showError(theResult);
                }
                if(ECE141::userTerminated == theResult.code) running = false;
            }
        } while(running);
    }

    return 0;
}
