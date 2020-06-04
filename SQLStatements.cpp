#include "SQLStatements.hpp"

namespace ECE141 {
//--------Use Table Statment--------
UseTableStatement::UseTableStatement(std::string aName)
    : Statement(Keywords::use_kw), DBName(aName) {}
UseTableStatement::~UseTableStatement() {}

void UseTableStatement::setDBName(std::string dbName) { this->DBName = dbName; }

std::string UseTableStatement::getDBName() const { return this->DBName; }

// ShowTable
ShowTables::ShowTables(std::string dbName)
    : Statement(Keywords::show_kw), dbName(dbName), index(false) {}
ShowTables::~ShowTables() {}

std::string ShowTables::getDBName() { return this->dbName; }

// Drop Table Statement
DropTableStatement::DropTableStatement(std::string aName)
    : Statement(Keywords::drop_kw), tableName(aName) {}

DropTableStatement::~DropTableStatement() {}

std::string DropTableStatement::getTableName() const { return this->tableName; }

// Describe Table Statement
DescribeTableStatement::DescribeTableStatement(std::string aTableName)
    : Statement(Keywords::describe_kw), tableName(aTableName) {}

DescribeTableStatement::~DescribeTableStatement() {}

std::string DescribeTableStatement::getTableName() const {
    return this->tableName;
}

// Insert table statement
InsertStatement::InsertStatement(std::string aName)
    : Statement(Keywords::insert_kw), tableName(aName) {
    values = {};
    keys = {};
    parseError = Errors::noError;
}

InsertStatement::~InsertStatement() {}

std::string InsertStatement::getTableName() const { return tableName; }
std::vector<std::string> InsertStatement::getKeys() const { return this->keys; }
std::vector<std::vector<std::string>> InsertStatement::getValues() const {
    return this->values;
}

void InsertStatement::parseTokenizer(Tokenizer& aTokenizer) {
    std::vector<Token> tokens;
    aTokenizer.next();

    // add all tokens to vec of tokens
    while(aTokenizer.more()) {
        // std::cout << "pushing: " << aTokenizer.current().data << std::endl;
        tokens.push_back(aTokenizer.current());
        aTokenizer.next();
    }

    // if first token is not (, error
    if(tokens[0].data != "(") {
        return;
    }

    tokens.erase(tokens.begin());  // erase first ( token
    auto iter = tokens.begin();

    while(iter->data != ")") {
        if(iter->data == ",") {
            iter++;
            continue;
        }

        keys.push_back(iter->data);  // get all keys, ignore commas
        iter++;
    }

    iter++;
    if(iter->keyword != Keywords::values_kw) {
        // error
    }

    iter++;

    if(tokens[tokens.size() - 1].data != ")") {
        this->parseError = Errors::punctuationExpected;
        return;
    }

    while(iter != tokens.end()) {
        if(iter->data == "(") {
            iter++;
            std::vector<std::string> newVals;
            while(iter->data != ")") {
                if(iter->data == "(" && (iter + 1)->data == ")") {
                    newVals.push_back("");
                    continue;
                } else if(iter->data == ",") {
                    iter++;
                    continue;
                }

                newVals.push_back(iter->data);
                iter++;
            }

            values.push_back(newVals);
        }
        iter++;
    }
}

// Delete table statement
DeleteStatement::DeleteStatement(std::string aTableName)
    : Statement(Keywords::delete_kw), tableName(aTableName) {}
DeleteStatement::~DeleteStatement() {}

std::string DeleteStatement::getTableName() const { return this->tableName; }

// Select Statement
SelectStatement::SelectStatement() : Statement(Keywords::select_kw) {}
SelectStatement::~SelectStatement() {}
void SelectStatement::addToken(Token& aToken) { tokens.push_back(aToken); }
std::vector<Token> SelectStatement::getTokens() const { return this->tokens; }

// Upate Statement
UpdateStatement::UpdateStatement() : Statement(Keywords::update_kw) {}
UpdateStatement::~UpdateStatement() {}

void UpdateStatement::addToken(Token& aToken) { tokens.push_back(aToken); }
std::vector<Token> UpdateStatement::getTokens() const { return this->tokens; }
std::string UpdateStatement::getName() const { return this->tableName; }

unsigned int UpdateStatement::filterCount() const {
    return this->filter.getCount();
}

StatusResult UpdateStatement::parseTokens() {
    this->tableName = tokens[0].data;

    if(tokens[1].keyword != Keywords::set_kw) {
        return Errors::keywordExpected;
    }

    this->key = tokens[2].data;

    if(tokens[3].data != "=") return Errors::operatorExpected;

    this->newVal = tokens[4].data;

    // auto it = tokens.begin() + 5;

    //    if(it != tokens.end()) {
    buildExpressions();
    //    }

    return Errors::noError;
}

StatusResult UpdateStatement::buildExpressions() {
    auto it = tokens.begin();
    while(it->keyword != Keywords::where_kw) {
        it++;
    }

    while(it != tokens.end()) {
        std::string lhs((++it)->data);
        std::string op((++it)->data);
        std::string rhs((++it)->data);
        ++it;

        if(it->keyword == Keywords::or_kw) {
            filter.setOr(true);
        } else if(it->keyword == Keywords::and_kw) {
            filter.setAnd(true);
        }

        Expression* newExp = new Expression(lhs, op, rhs);
        filter.add(newExp);
    }

    return Errors::noError;
}

std::string& UpdateStatement::getLhs(int aPos) const {
    return this->filter.getLhs(aPos);
}

std::string& UpdateStatement::getRhs(int aPos) const {
    return this->filter.getRhs(aPos);
}

bool UpdateStatement::getOr() const { return this->filter.getOr(); }
bool UpdateStatement::getAnd() const { return this->filter.getAnd(); }

//--------Create Table Statment--------
CreateTableStatement::CreateTableStatement(std::string aName, Schema* schema)
    : Statement(Keywords::create_kw),
      newSchema(schema),
      schemaName(aName),
      errStatus(noError) {}

CreateTableStatement::~CreateTableStatement() {
    if(newSchema != nullptr) {
        // free schema stuff
    }
}

std::string CreateTableStatement::getSchemaName() const {
    return this->schemaName;
}

Errors CreateTableStatement::checkErrStatus() const { return this->errStatus; }

void CreateTableStatement::parseTokenizer(Tokenizer& aTokenizer) {
    std::vector<Token> tokens;

    // add all tokens to vec of tokens
    while(aTokenizer.more()) {
        tokens.push_back(aTokenizer.current());
        aTokenizer.next();
    }

    // if first token is not (, error
    if(tokens[0].data != "(") {
        errStatus = Errors::unknownCommand;
        return;
    }

    tokens.erase(tokens.begin());  // erase first ( token
    auto iter = tokens.begin();

    while(tokens.size() > 0 && this->errStatus == Errors::noError) {
        unsigned int count = 0;
        std::vector<Token> attributes;
        while(iter->data != "," && iter != tokens.end()) {
            attributes.push_back(*iter);
            iter++;
            count++;
        }

        parseAttributes(attributes);  // parse attributes and add to openShcema

        // look for end of statement, else keep going
        if(iter->data == "," && (iter + 1)->data == ")") {
            tokens.clear();
        } else if(iter->data == ")" || iter == tokens.end()) {
            tokens.clear();
        } else {
            iter++;
        }
    }
}

void CreateTableStatement::parseAttributes(std::vector<Token>& attributes) {
    std::vector<ECE141::Keywords> types{
        Keywords::integer_kw, Keywords::float_kw, Keywords::datetime_kw,
        Keywords::varchar_kw, Keywords::boolean_kw};

    // IS THIS THE RIGHT ERR NUM?
    if(attributes.size() < 2) {
        this->errStatus = Errors::unknownAttribute;
        return;
    }

    // get name and type, always need these two
    std::string name = attributes[0].data;
    Keywords typeKW = attributes[1].keyword;

    auto it = std::find(types.begin(), types.end(), typeKW);  // find keyword

    // invalid type kw, error
    if(it == types.end() && attributes[1].data != "TIMESTAMP") {
        this->errStatus = Errors::unknownType;
        return;
    }

    char typeChar;
    if(attributes[1].data == "TIMESTAMP") {
        typeChar = 'D';
    } else {
        typeChar = Helpers::keywordToChar(typeKW);
    }

    // get type char from helper method

    Attribute* newAttr = new Attribute(name, (DataType)typeChar);

    // if data type is a varchar, parse for field length
    int i = 3;
    if(((DataType)typeChar == DataType::varchar_type)) {
        if(attributes[2].data == "(") {
            std::stringstream numStream;

            while(attributes[i].data != ")") {
                numStream << attributes[i].data;
                i++;
            }

            // cast char to int, and save
            double streamHolder;
            numStream >> streamHolder;
            int fieldSize = static_cast<int>(streamHolder);
            newAttr->setFieldLength(fieldSize);
        }
    }

    // look for extras and set them to true if they are set
    if(attributes.size() > 2) {
        if(checkNotNullable(attributes)) newAttr->setNotNullable(true);
        if(checkAutoIncr(attributes)) newAttr->setAutoIncr(true);
        if(checkPrimaryKey(attributes)) newAttr->setPrimKey(true);
        checkDefault(attributes, *newAttr);
    }

    // make new schema if one does not exist
    if(newSchema == nullptr) {
        this->newSchema = new Schema(this->schemaName);
    }

    newSchema->addAttribute(*newAttr);  // add to attribute list
}

// check for not nullable attr
bool CreateTableStatement::checkNotNullable(std::vector<Token>& attributes) {
    auto it = attributes.begin();

    while(it != attributes.end()) {
        if(it->data == "not") {
            if((it + 1)->data == "null") return true;
        }
        it++;
    }

    return false;
}

// check for auto incr attr
bool CreateTableStatement::checkAutoIncr(std::vector<Token>& attributes) {
    auto it = attributes.begin();

    while(it != attributes.end()) {
        if(it->keyword == Keywords::auto_increment_kw) {
            return true;
        }
        it++;
    }

    return false;
}

// check for primary key attr
bool CreateTableStatement::checkPrimaryKey(std::vector<Token>& attributes) {
    auto it = attributes.begin();
    while(it != attributes.end()) {
        if(it->keyword == Keywords::primary_kw) {
            if((it + 1)->keyword == Keywords::key_kw) {
                return true;
            }
        }
        it++;
    }

    return false;
}

bool CreateTableStatement::checkDefault(std::vector<Token>& attributes,
                                        Attribute& attr) {
    auto it = attributes.begin();
    while(it != attributes.end()) {
        if(it->type == TokenType::identifier && it->data == "DEFAULT") {
            attr.setDefValStr((it + 1)->data);
        }
        it++;
    }

    return false;
}

}  // namespace ECE141