//
//  SQLProcessor.cpp
//  Assignment4
//
//  Created by rick gessner on 4/18/20.
//  Copyright © 2020 rick gessner. All rights reserved.
//

#include "SQLProcessor.hpp"

namespace ECE141 {

SQLProcessor::SQLProcessor(CommandProcessor* aNext) : CommandProcessor(aNext) {
    openDB = nullptr;
    openSchema = {};
    errStat = Errors::noError;
    this->dataSets = {};
    statementPtr = nullptr;
}

SQLProcessor::~SQLProcessor() {
    /*if(openDB != nullptr) {
        delete openDB;
    }

    while

    openSchema.clear();

    for(auto iter : dataSets) {
        delete &iter;
    }
    dataSets.clear();*/
}

Statement* SQLProcessor::getStatement(Tokenizer& aTokenizer) {
    // aTokenizer.dump();
    // std::cerr << "\nbreak\n";
    std::vector<ECE141::Keywords> keywords{
        Keywords::create_kw, Keywords::use_kw,    Keywords::describe_kw,
        Keywords::drop_kw,   Keywords::show_kw,   Keywords::insert_kw,
        Keywords::delete_kw, Keywords::select_kw, Keywords::update_kw};

    Token firstKW = aTokenizer.current();
    auto it = std::find(keywords.begin(), keywords.end(), firstKW.keyword);

    // kw not found
    if(it == keywords.end()) {
        return nullptr;
    }

    // if format of command is not correct, not valid
    if(!checkDBKW(aTokenizer, *it)) {
        return nullptr;
    }

    Statement* retStatement = nullptr;

    if(firstKW.keyword == Keywords::use_kw) {
        retStatement = new UseTableStatement(aTokenizer.peek().data);
        // statementPtr = new UseTableStatement(aTokenizer.peek().data);
        // retStatement = statementPtr;
        aTokenizer.next();
        aTokenizer.next();
    } else if(firstKW.keyword == Keywords::create_kw) {
        aTokenizer.next();
        std::string name = aTokenizer.current().data;
        aTokenizer.next();
        retStatement = new CreateTableStatement(name);
        CreateTableStatement* createStatement =
            dynamic_cast<CreateTableStatement*>(retStatement);
        createStatement->parseTokenizer(aTokenizer);
    } else if(firstKW.keyword == Keywords::drop_kw) {
        aTokenizer.next();
        retStatement = new DropTableStatement(aTokenizer.current().data);
        aTokenizer.next();
    } else if(firstKW.keyword == Keywords::describe_kw) {
        retStatement = new DescribeTableStatement(aTokenizer.current().data);
        aTokenizer.next();
    } else if(firstKW.keyword == Keywords::show_kw) {
        if(openDB != nullptr) {
            retStatement = new ShowTables(this->openDB->getName());
            if(aTokenizer.current().data == "index") {
                this->index = true;
            }
        } else {
            retStatement = new ShowTables("");
        }
        aTokenizer.next();
        aTokenizer.next();
    } else if(firstKW.keyword == Keywords::insert_kw) {
        Token currToken = aTokenizer.current();
        retStatement = new InsertStatement(currToken.data);
        InsertStatement* insertStmnt =
            dynamic_cast<InsertStatement*>(retStatement);
        insertStmnt->parseTokenizer(aTokenizer);
    } else if(firstKW.keyword == Keywords::delete_kw) {
        Token currToken = aTokenizer.current();
        retStatement = new DeleteStatement(currToken.data);
        aTokenizer.next();
    } else if(firstKW.keyword == Keywords::select_kw) {
        retStatement = new SelectStatement();
        SelectStatement* selectStmnt =
            dynamic_cast<SelectStatement*>(retStatement);
        while(aTokenizer.more()) {
            selectStmnt->addToken(aTokenizer.current());
            aTokenizer.next();
        }
    } else if(firstKW.keyword == Keywords::update_kw) {
        retStatement = new UpdateStatement();
        UpdateStatement* updateStmnt =
            dynamic_cast<UpdateStatement*>(retStatement);
        while(aTokenizer.more()) {
            updateStmnt->addToken(aTokenizer.current());
            aTokenizer.next();
        }
        updateStmnt->parseTokens();
    }

    statementPtr = retStatement;
    return retStatement;
}

StatusResult SQLProcessor::interpret(const Statement& aStatement) {
    Keywords statementType = aStatement.getType();
    StatusResult retStatus = Errors::noError;

    if(errStat != Errors::noError) {
        return errStat;
    }

    if(statementType == Keywords::use_kw) {
        // dynamically cast statment into correct type and call correct method
        const UseTableStatement* useTableStmnt =
            dynamic_cast<const UseTableStatement*>(&aStatement);
        retStatus = useTable(*useTableStmnt);

    } else if(statementType == Keywords::create_kw) {
        if(this->openDB == nullptr) {
            return Errors::noDatabaseSpecified;
        }

        const CreateTableStatement* createTableStmnt =
            dynamic_cast<const CreateTableStatement*>(&aStatement);

        retStatus = createTable(createTableStmnt->getNewSchema());

    } else if(statementType == Keywords::drop_kw) {
        const DropTableStatement* dropTableStmnt =
            dynamic_cast<const DropTableStatement*>(&aStatement);
        retStatus = dropTable(dropTableStmnt->getTableName());

    } else if(statementType == Keywords::describe_kw) {
        const DescribeTableStatement* describeTableStmnt =
            dynamic_cast<const DescribeTableStatement*>(&aStatement);
        retStatus = describeTable(describeTableStmnt->getTableName());

    } else if(statementType == Keywords::show_kw) {
        if(this->openDB == nullptr) {
            return Errors::noDatabaseSpecified;
        }

        std::vector<std::string> tables;
        std::vector<std::string> fields;

        for(auto iter : this->openSchema) {
            tables.push_back(iter.getName());
        }

        if(index)
            printIndeces(tables);
        else
            printTableNames(tables);
    } else if(statementType == Keywords::insert_kw) {
        const InsertStatement* insertStmnt =
            dynamic_cast<const InsertStatement*>(&aStatement);
        retStatus = insert(*insertStmnt);
    } else if(statementType == Keywords::delete_kw) {
        const DeleteStatement* deleteStmnt =
            dynamic_cast<const DeleteStatement*>(&aStatement);
        retStatus = deleteFromTable(*deleteStmnt);
    } else if(statementType == Keywords::select_kw) {
        const SelectStatement* selectStmnt =
            dynamic_cast<const SelectStatement*>(&aStatement);
        retStatus = selectFromTable(*selectStmnt);
    } else if(statementType == Keywords::update_kw) {
        const UpdateStatement* updateStmnt =
            dynamic_cast<const UpdateStatement*>(&aStatement);
        retStatus = update(*updateStmnt);
    }

    /*if(statementPtr != nullptr) {
        std::cout << "statement pointer: " << statementPtr << std::endl;
        delete statementPtr;
        statementPtr = nullptr;
    }*/

    return retStatus;
}  // namespace ECE141

StatusResult SQLProcessor::update(const UpdateStatement& aStatement) {
    StatusResult retStatus = Errors::noError;

    if(this->openDB == nullptr) return Errors::noDatabaseSpecified;  // no db

    std::string tableName(aStatement.getName());
    if(!tableExists(tableName)) return Errors::unknownTable;  // no such table

    if(!retStatus) {
        return retStatus;
    }

    Schema selectedTable(tableName);
    getTableByName(tableName, selectedTable);

    std::string key(aStatement.getKey());
    std::vector<std::string> attribute{key};

    if(!attributeCheck(attribute, selectedTable))
        return Errors::unknownAttribute;

    RowCollection* collectionToUpdate;
    for(auto iter : dataSets) {
        if(tableName == iter->getParentName()) {
            collectionToUpdate = &*iter;
        }
    }

    // change based on filters
    std::string newVal(aStatement.getNewVal());
    if(aStatement.filterCount() == 0) {
        for(auto& iter : collectionToUpdate->getRows()) {
            iter.setValue(key, newVal);
        }
    } else if(aStatement.filterCount() == 1) {
        // change where expressions are true
        std::string lhs(aStatement.getLhs());
        std::string rhs(aStatement.getRhs());

        for(auto& iter : collectionToUpdate->getRows()) {
            if(iter.getValue(lhs) == rhs) {
                iter.setValue(key, newVal);
            }
        }

    } else if(aStatement.filterCount() == 2) {
        std::string lhs(aStatement.getLhs());
        std::string rhs(aStatement.getRhs());
        std::string lhsTwo(aStatement.getLhs(1));
        std::string rhsTwo(aStatement.getRhs(1));

        if(aStatement.getAnd()) {
            for(auto& iter : collectionToUpdate->getRows()) {
                if(iter.getValue(lhs) == rhs && iter.getValue(lhsTwo) == rhsTwo)
                    iter.setValue(key, newVal);
            }
        } else if(aStatement.getOr()) {
            for(auto& iter : collectionToUpdate->getRows()) {
                if(iter.getValue(lhs) == rhs || iter.getValue(lhsTwo) == rhsTwo)
                    iter.setValue(key, newVal);
            }
        }
    }

    return retStatus;
}

bool SQLProcessor::checkDBKW(Tokenizer& aTokenizer, Keywords& kw) {
    aTokenizer.next();

    Token currToken = aTokenizer.current();

    if(kw == Keywords::show_kw) {
        return ((currToken.keyword == Keywords::tables_kw ||
                 currToken.data == "indexes") &&
                aTokenizer.size() == 2);
    } else if(kw == Keywords::create_kw && aTokenizer.size() > 3) {
        return (currToken.keyword == Keywords::table_kw);
    } else if(kw == Keywords::use_kw && aTokenizer.size() == 3) {
        return (aTokenizer.current().keyword == Keywords::database_kw);
    } else if(kw == Keywords::drop_kw && aTokenizer.size() == 3) {
        return (currToken.keyword == Keywords::table_kw);
    } else if(kw == Keywords::describe_kw && aTokenizer.size() == 2) {
        return true;
    } else if(kw == Keywords::insert_kw &&
              currToken.keyword == Keywords::into_kw) {
        aTokenizer.next();
        return true;
    } else if(kw == Keywords::delete_kw && aTokenizer.size() == 3) {
        aTokenizer.next();
        return true;
    } else if(kw == Keywords::select_kw && aTokenizer.size() > 3) {
        return true;
    } else if(kw == Keywords::update_kw) {
        return true;
    }

    return false;
}

StatusResult SQLProcessor::selectFromTable(
    const SelectStatement& aStatement) const {
    // get tokens and initialize return status
    std::vector<Token> tokens(aStatement.getTokens());
    StatusResult retStatus = Errors::noError;
    std::string whereLhs, whereRhs, whereOp, orderBy;
    unsigned int limit = 0;
    bool limitOn = false;
    bool orderByOpt = false;

    // make sure db is open, error if not
    if(this->openDB == nullptr) return Errors::noDatabaseSpecified;

    std::vector<std::string> selectVars;
    auto selectIt = tokens.begin();

    Timer timer;

    while(selectIt->keyword != Keywords::from_kw) {
        selectVars.push_back(selectIt->data);
        selectIt++;

        if(selectIt == tokens.end()) {
            retStatus = Errors::keywordExpected;
            return retStatus;
        }
    }

    if(selectVars.size() == 0) {
        std::cout << "\n0 Rows in set\n";
        return Errors::noError;
    }

    selectIt++;  // burn 'from' keyword

    std::string tableName(selectIt->data);
    selectIt++;

    // check that name belongs to a valid schema
    Schema selectedTable(tableName);
    if(!getTableByName(tableName, selectedTable)) return Errors::unknownTable;

    auto whereIt = tokens.begin();
    while(whereIt != tokens.end()) {
        if(whereIt->keyword == Keywords::where_kw) {
            whereLhs = (++whereIt)->data;  // save lhs

            // check if attr is valid
            retStatus = selectedTable.validAttribute(whereLhs);
            if(retStatus.code != Errors::noError) return retStatus;

            // get operator and rhs
            whereOp = (++whereIt)->data;
            whereRhs = (++whereIt)->data;
        }

        whereIt++;
    }

    // look for order by
    auto orderByIter = tokens.begin();
    while(orderByIter != tokens.end() - 1) {
        if(orderByIter->keyword == Keywords::order_kw &&
           (orderByIter + 1)->keyword == Keywords::by_kw) {
            orderBy = (orderByIter + 2)->data;  // get order by str
            orderByOpt = true;                  // set switch
        }
        orderByIter++;
    }

    if(orderByOpt) retStatus = selectedTable.validAttribute(orderBy);
    if(!retStatus) return retStatus;

    // look for limit
    while(selectIt != tokens.end()) {
        std::string tokenStr(selectIt->data);

        if(selectIt->data == "LIMIT") {
            limit = std::stoi((selectIt + 1)->data);
            limitOn = true;
            break;
        }

        selectIt++;
    }

    // validate select attrs (after we get table and Table name)
    if(!attributeCheck(selectVars, selectedTable))
        return Errors::unknownAttribute;

    RowCollection* collectionToSearch;
    for(auto iter : dataSets) {
        if(tableName == iter->getParentName()) {
            collectionToSearch = &*iter;
        }
    }

    timer.start();
    std::vector<Row> rowsToSearch(collectionToSearch->getRows());
    std::vector<Row> results;

    if(!whereRhs.empty()) {
        if(whereLhs == "id" && whereOp == ">") {
            unsigned int min = std::stoi(whereRhs);
            for(unsigned int i = 1; i <= min; i++) {
                rowsToSearch.erase(rowsToSearch.begin());
            }
        } else {
            std::string key(whereLhs);
            for(auto iter = rowsToSearch.begin(); iter != rowsToSearch.end();) {
                if(iter->getValue(key) != whereRhs) {
                    iter = rowsToSearch.erase(iter);
                } else {
                    iter++;
                }
            }
        }
    }

    if(limitOn && limit < rowsToSearch.size()) {
        for(unsigned int i = 0; i < limit; i++) {
            std::cerr << "i: " << i << std::endl;
            results.push_back(rowsToSearch.at(i));
        }
    } else {
        results = rowsToSearch;
    }

    timer.stop();

    TableView newView;
    newView.printSelectResults(results, timer.elapsed());

    return retStatus;
}

StatusResult SQLProcessor::insert(const InsertStatement& aStatement) const {
    StatusResult retStatus = Errors::noError;
    std::string tableName(aStatement.getTableName());

    if(this->openDB == nullptr) {
        return Errors::noDatabaseSpecified;
    }

    if(!tableExists(tableName)) {
        return Errors::unknownTable;
    }

    std::vector<std::string> keys = aStatement.getKeys();
    std::vector<std::vector<std::string>> values = aStatement.getValues();

    for(auto iter : values) {
        std::string toStore;
        retStatus = dataSetToStorageBlock(iter, keys, tableName, toStore);
        StorageBlock schemaHeader(BlockType::data_block);
        schemaHeader.setData(toStore);
        this->openDB->getStorage().writeBlock(schemaHeader, openSchema.size());
    }

    return retStatus;
}

StatusResult SQLProcessor::deleteFromTable(
    const DeleteStatement& aStatement) const {
    std::string tableName(aStatement.getTableName());

    if(openDB == nullptr) {
        return Errors::noDatabaseSpecified;
    }

    if(!tableExists(tableName)) {
        return Errors::unknownTable;
    }

    if(this->openDB != nullptr) {
        this->openDB->getStorage().removeEntries(tableName);
    }

    return Errors::noError;
}

StatusResult SQLProcessor::dataSetToStorageBlock(
    const std::vector<std::string>& vals, std::vector<std::string>& keys,
    std::string& tableName, std::string& strToStore) const {
    if(keys.size() != vals.size()) {
        return Errors::unknownAttribute;
    }

    strToStore.append(tableName + " ");
    Row* newData = new Row();

    auto keyIter = keys.begin();
    for(auto iter : vals) {
        newData->addToRow(*keyIter, iter);
        strToStore.append(*keyIter + "-" + iter + " ");
        keyIter++;
    }

    addToCollection(*newData, tableName);

    strToStore.append(" /");
    return Errors::noError;
}

StatusResult SQLProcessor::useTable(const UseTableStatement& statement) {
    if(this->openDB != nullptr) {
        openSchema.clear();
    }

    OpenExistingStorage openExisting;
    this->openDB = new Database(statement.getDBName(), openExisting);

    if(!this->openDB->dbCreated()) {
        return Errors::unknownDatabase;
    }

    // load schema into vector of schema
    this->openDB->getStorage().readSchema(this->openSchema);

    for(auto iter : openSchema) {
        RowCollection* newRow = new RowCollection(iter.getName());
        dataSets.push_back(newRow);
    }

    return Errors::noError;
}

StatusResult SQLProcessor::createTable(const Schema& aSchema) {
    if(tableExists(aSchema.getName())) {
        return Errors::tableExists;
    }

    this->openSchema.push_back(aSchema);

    if(this->openDB != nullptr) {
        StorageBlock schemaHeader(BlockType::entity_block);
        std::string dataString;
        schemaToStorageBlock(aSchema, dataString);
        schemaHeader.setData(dataString);
        this->openDB->getStorage().writeBlock(schemaHeader, openSchema.size());
        RowCollection* newRow = new RowCollection(aSchema.getName());
        dataSets.push_back(newRow);
    }

    return Errors::noError;
}

void SQLProcessor::schemaToStorageBlock(const Schema& aSchema,
                                        std::string& dataString) {
    dataString.append(aSchema.getName() + " ");

    for(auto iter : aSchema.getAttributes()) {
        std::string title = iter.getName();
        char type = (char)iter.getType();
        bool notNullable = iter.getNotNullable();
        bool pKey = iter.getPrimKey();
        bool autoIncr = iter.getAutoIncr();
        bool defaultVal = iter.getDefValSwitch();
        int fieldLength = iter.getFieldLength();

        std::string charStr(1, type);
        charStr.append(" ");

        dataString.append(title + " ");
        dataString.append(charStr);

        if(fieldLength != -1) {
            dataString.append("1 " + std::to_string(iter.getFieldLength()) +
                              " ");
        } else {
            dataString.append("0 ");
        }

        dataString.append((notNullable ? "1 " : "0 "));
        dataString.append((pKey ? "1 " : "0 "));
        dataString.append((autoIncr ? "1 " : "0 "));

        if(defaultVal) {
            // dataString.append("1 " + iter.getDefValStr() + " /");
            dataString.append("1 ");
        } else {
            // dataString.append("0 /");
            dataString.append("0 ");
        }
    }
    dataString.append("/");
}

StatusResult SQLProcessor::dropTable(const std::string& aName) {
    if(!tableExists(aName)) {
        return Errors::unknownTable;
    }

    int count = 0;
    int posToErase = -1;

    for(auto it : openSchema) {
        if(aName == it.getName()) {
            posToErase = count;
        }
        count++;
    }

    if(posToErase != -1) {
        openSchema.erase(openSchema.begin() + posToErase);
        this->openDB->getStorage().removeEntries(aName, true);
    }

    return Errors::noError;
}

StatusResult SQLProcessor::describeTable(const std::string& aName) const {
    if(!tableExists(aName)) {
        return Errors::unknownTable;
    }

    for(auto iter : openSchema) {
        if(aName == iter.getName()) {
            TableView view;
            view.printTable(&iter);
            break;
        }
    }

    return Errors::noError;
}

bool SQLProcessor::tableExists(const std::string& tableName) const {
    for(auto iter : openSchema) {
        if(iter.getName() == tableName) return true;
    }

    return false;
}

void SQLProcessor::printTableNames(std::vector<std::string>& names) {
    std::cout << " \nTables in " << this->openDB->getName() << std::endl;

    for(auto iter : names) {
        std::cout << " - " << iter << std::endl;
    }

    std::cout << names.size() << " rows in set\n";
}

void SQLProcessor::printIndeces(std::vector<std::string>& names) {
    std::cout << " \nTable      Field\n ";

    for(auto iter : names) {
        std::cout << iter << "     id\n";
    }

    std::cout << names.size() << " rows in set\n";
}

bool SQLProcessor::getTableByName(std::string& aName, Schema& ref) const {
    for(auto iter : this->openSchema) {
        if(aName == iter.getName()) {
            ref = iter;
            return true;
        }
    }

    return false;
}

StatusResult SQLProcessor::attributeCheck(std::vector<std::string> attrs,
                                          Schema& schemaToCheck) const {
    StatusResult retStatus = Errors::noError;

    for(auto it : attrs) {
        if(it == "*" && attrs.size() == 1) {
            return Errors::noError;
        } else {
            retStatus = schemaToCheck.validAttribute(it);
            if(!retStatus) return retStatus;
        }
    }

    return retStatus;
}

StatusResult SQLProcessor::attributeCheck(std::string attrs,
                                          Schema& schemaToCheck) const {
    return Errors::noError;
}

void SQLProcessor::addToCollection(Row& aRow, std::string& schemaName) const {
    for(auto iter : dataSets) {
        if(iter->getParentName() == schemaName) {
            iter->addRow(aRow);
        }
    }
}

}  // namespace ECE141
