#include "DBProcessor.hpp"

namespace ECE141 {

namespace fs = std::filesystem;

DBProcessor::DBProcessor(CommandProcessor *aNext) : CommandProcessor(aNext) {
    buildMap();
    statementPtr = nullptr;
}

DBProcessor::~DBProcessor() {}

StatusResult DBProcessor::interpret(const Statement &aStatement) {
    Keywords statementType = aStatement.getType();
    StatusResult retStatus = Errors::noError;
    auto it = commandMap.find(statementType);

    const DBStatement *appStmnt =
        dynamic_cast<const DBStatement *>(&aStatement);

    if(it != commandMap.end()) {
        retStatus = (this->*(it->second))(*appStmnt);
    } else {
        std::cerr << "app command not in map\n";
    }

    /*if(statementPtr != nullptr) {
        delete statementPtr;
        statementPtr = nullptr;
    }*/

    return StatusResult(retStatus);
}

// USE: factory to create statement based on given tokens...
Statement *DBProcessor::getStatement(Tokenizer &aTokenizer) {
    std::vector<ECE141::Keywords> keywords{
        Keywords::create_kw, Keywords::describe_kw, Keywords::drop_kw,
        Keywords::show_kw};

    Token firstKW = aTokenizer.current();
    auto it = std::find(keywords.begin(), keywords.end(), firstKW.keyword);
    if(it != keywords.end()) {
        DBStatement *returnStatement;

        if(firstKW.keyword == Keywords::show_kw) {
            returnStatement = new DBStatement(firstKW.keyword);
            Token currToken = aTokenizer.peek();

            if(currToken.keyword == Keywords::databases_kw) {
                aTokenizer.next();
                aTokenizer.next();
            } else {
                returnStatement = nullptr;
            }

            return returnStatement;
        } else {
            returnStatement = new DBStatement(firstKW.keyword);
            Token currToken = aTokenizer.peek();

            if(currToken.keyword == Keywords::database_kw &&
               aTokenizer.size() == 3) {
                std::string newDB = aTokenizer.peek(2).data;
                returnStatement->setDBName(newDB);
                aTokenizer.next();
                aTokenizer.next();
                aTokenizer.next();
            } else {
                // std::cout << "returning nullptr\n";
                returnStatement = nullptr;
            }
        }

        statementPtr = returnStatement;
        return returnStatement;
    }

    return nullptr;
}

void DBProcessor::buildMap() {
    commandMap.emplace(Keywords::create_kw, &DBProcessor::create);
    commandMap.emplace(Keywords::use_kw, &DBProcessor::use);
    commandMap.emplace(Keywords::show_kw, &DBProcessor::show);
    commandMap.emplace(Keywords::describe_kw, &DBProcessor::describe);
    commandMap.emplace(Keywords::drop_kw, &DBProcessor::drop);
}

StatusResult DBProcessor::create(const DBStatement &dbStatement) const {
    std::string newDBName = dbStatement.getDBName();

    CreateNewStorage createStorage;
    Database database(newDBName, createStorage);
    StatusResult retStatus = database.createDatabase(newDBName);

    return retStatus;
}

StatusResult DBProcessor::show(const DBStatement &dbStatement) const {
    std::string defPath(StorageInfo::getDefaultStoragePath());

    StatusResult retStatus = Errors::noError;
    std::string dbName = dbStatement.getDBName();

    for(const auto &it : fs::directory_iterator(defPath)) {
        fs::path path = it.path();
        std::cout << path.filename().string() << std::endl;
    }

    return retStatus;
}

StatusResult DBProcessor::use(const DBStatement &dbStatement) const {
    std::string defPath(StorageInfo::getDefaultStoragePath());

    StatusResult retStatus = Errors::noError;
    std::string dbName = dbStatement.getDBName();

    if(!fileExists(dbName)) {
        retStatus = Errors::unknownDatabase;
    } else {
        std::cout << "Using database: " << dbName << std::endl;
    }

    return retStatus;
}

StatusResult DBProcessor::describe(const DBStatement &dbStatement) const {
    std::string defPath(StorageInfo::getDefaultStoragePath());
    std::string dbName = dbStatement.getDBName();

    OpenExistingStorage openStorage;
    Database database(dbStatement.getDBName(), openStorage);

    StatusResult retStatus = database.describeDatabase(dbName);

    return retStatus;
}

StatusResult DBProcessor::drop(const DBStatement &dbStatement) const {
    std::string defPath(StorageInfo::getDefaultStoragePath());
    std::string dbName = dbStatement.getDBName();

    OpenExistingStorage createStorage;
    Database database(dbStatement.getDBName(), createStorage);

    StatusResult retStatus = database.dropDatabase(dbName);

    return retStatus;
}

bool DBProcessor::fileExists(std::string name) const {
    std::string defPath(StorageInfo::getDefaultStoragePath());
    std::string newstr = defPath + name;
    return fs::exists(defPath + name);
}

}  // namespace ECE141