//
//  View.hpp
//  Datatabase4
//
//  Created by rick gessner on 4/17/19.
//  Copyright © 2019 rick gessner. All rights reserved.
//

#ifndef View_h
#define View_h

#include <iostream>
#include <string>

#include "RowCollection.hpp"
#include "Schema.hpp"

#define HEADER_ROW                                                           \
    "+-----------+--------------+------+-----+---------+-------------------" \
    "----------+\n"
#define TABLE_HEADER                                                           \
    "| Field     | Type         | Null | Key | Default | Extra               " \
    "        |\n"
#define NO "NO"
#define YES "YES"
#define NULL_STR "NULL"
#define TRUE "TRUE"
#define FALSE "FALSE"
#define ROW_DELIM "|"
#define LEFT_CELL "| "
#define END_COL "|\n"
#define AUTO_STR "auto_increment"
#define PK_STR "primary key"
#define EXTRA_COL_WIDTH 28
#define WIDE_COL_WIDTH 13
#define MED_COL_WIDTH 10
#define SMALL_COL_WIDTH 4
#define COL_WIDTH 30

namespace ECE141 {

// completely generic view, which you will subclass to show information
class View {
   public:
    virtual ~View() {}
    virtual bool show(std::ostream &aStream) = 0;
};

class TableView {
   public:
    TableView();
    ~TableView();
    bool show(std::ostream &aStream);

    void printTable(const Schema *aTable);
    std::string buildFieldLength(int length);
    void printType(char typeChar, int fieldLength);
    void printNSpaces(int n);
    void printSelectResults(std::vector<Row> &rowsToPrint, double elapsedTime);
};  // namespace ECE141

}  // namespace ECE141

#endif /* View_h */
