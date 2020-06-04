#include "View.hpp"

namespace ECE141 {
TableView::TableView() {}
TableView::~TableView() {}
bool TableView::show(std::ostream& aStream) { return true; }

void TableView::printTable(const Schema* aTable) {
    unsigned int rows = aTable->getAttrNum();

    std::cout << std::endl << HEADER_ROW << TABLE_HEADER << HEADER_ROW;

    // std::cerr << "b4 for\n";
    // std::cerr << "attr size: " << aTable.getAttrNum() << std::endl;
    for(auto iter : aTable->getAttributes()) {
        std::string field = iter.getName();
        char type = (char)iter.getType();
        bool notNullable = iter.getNotNullable();
        bool key = iter.getPrimKey();
        bool auto_incr = iter.getAutoIncr();
        int fieldLength = iter.getFieldLength();
        bool defValBool = iter.getDefValSwitch();

        // std::cerr << "name\n";
        std::cout << LEFT_CELL << field;
        printNSpaces(MED_COL_WIDTH - field.size());

        // std::cerr << "type\n";
        std::cout << LEFT_CELL;
        printType(type, fieldLength);

        // std::cerr << "null\n";
        std::cout << LEFT_CELL;
        if(notNullable) {
            std::cout << NO << "   ";
        } else {
            std::cout << YES << "  ";
        }

        // std::cerr << "key\n";
        if(key) {
            std::cout << LEFT_CELL << YES << " ";
        } else {
            std::cout << LEFT_CELL << "    ";
        }

        // default val goes here

        if(defValBool) {
            std::cout << LEFT_CELL;
            std::string defValStr = iter.getDefValStr();
            std::cout << defValStr;
            printNSpaces(8 - defValStr.length());
        } else {
            std::cout << "|         ";
        }

        std::cout << LEFT_CELL;
        std::string extraString;

        if(auto_incr) {
            extraString.append(AUTO_STR);
            extraString.append(" ");
        }

        if(key) {
            extraString.append(PK_STR);
            extraString.append(" ");
        }

        std::cout << extraString;
        printNSpaces(EXTRA_COL_WIDTH - extraString.size());
        std::cout << END_COL;
    }

    std::cout << HEADER_ROW;
    std::cout << rows << " rows in set\n";
}

void TableView::printSelectResults(std::vector<Row>& rowsToPrint,
                                   double elapsedTime) {
    if(rowsToPrint.size() == 0) {
        std::cout << "\n0 rows in set\n";
        return;
    }

    KeyValues titlesToPrint = rowsToPrint[0].getPairs();
    std::vector<std::string> keys;

    std::cout << std::endl;

    for(auto& [key, value] : titlesToPrint) {
        std::cout << key;
        printNSpaces(COL_WIDTH - key.size());
        keys.push_back(key);
    }

    std::cout << std::endl;

    for(auto iter : rowsToPrint) {
        for(auto& keyIter : keys) {
            const std::string value(iter.getValue(keyIter));
            std::cout << value;
            printNSpaces(COL_WIDTH - value.size());
        }

        std::cout << std::endl;
    }

    std::cout << std::endl << rowsToPrint.size() << " rows in set";
    std::cout << std::endl << " (" << elapsedTime << ")\n";
}

void TableView::printType(char typeChar, int fieldLength) {
    switch(typeChar) {
        case('N'):
            std::cout << "no type      ";
            break;
        case('B'):
            std::cout << "boolean      ";
            break;
        case('D'):
            std::cout << "date         ";
            break;
        case('F'):
            std::cout << "float        ";
            break;
        case('I'):
            std::cout << "integer      ";
            break;
        case('V'):
            std::cout << "varchar";
            if(fieldLength != -1) {
                std::string length = "(" + std::to_string(fieldLength) + ")";
                std::cout << length;
                printNSpaces(WIDE_COL_WIDTH - 7 - length.size());
            } else {
                printNSpaces(WIDE_COL_WIDTH - 7);
            }
            break;
    }
}

void TableView::printNSpaces(int n) {
    for(int i = 0; i < n; i++) {
        std::cout << " ";
    }
}
}  // namespace ECE141