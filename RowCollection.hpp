
#ifndef RowCollection_hpp
#define RowCollection_hpp

#include "Row.hpp"

namespace ECE141 {

class RowCollection {
   public:
    RowCollection(std::string schemaName);
    ~RowCollection();

    RowCollection(const RowCollection& aCollection);

    void addRow(Row& aRow);
    std::string getParentName() { return this->owningSchema; }
    int getRowCount() { return this->rows.size(); }
    std::vector<Row>& getRows() {return this->rows;}
    // std::vector& getRows() { return this->rows; }

   private:
    std::string owningSchema;
    std::vector<Row> rows;
};

}  // namespace ECE141

#endif