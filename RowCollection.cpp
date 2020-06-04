#include "RowCollection.hpp"

namespace ECE141 {

// Row Collection
RowCollection::RowCollection(std::string schemaName)
    : owningSchema(schemaName) {
    this->rows = {};
}

RowCollection::~RowCollection() {}

void RowCollection::addRow(Row& aRow) { rows.push_back(aRow); }

RowCollection::RowCollection(const RowCollection& aCollection) {
    this->owningSchema = aCollection.owningSchema;
    this->rows = aCollection.rows;
}

}  // namespace ECE141