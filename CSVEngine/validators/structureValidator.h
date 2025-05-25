#pragma once

#include "../models.h"
#include <optional>

using namespace std;

inline optional<InvalidRecord> validateStructure(const CSVRecord& record, const CSVSchema& schema) {
    if (record.values.size() != schema.csvFields.size()) {
        InvalidRecord invalid;
        invalid.rowNumber = record.rowNumber;
        invalid.sourceType = record.sourceType;
        invalid.valueErrors.insert(ErrorType::FieldCountMismatch);
        invalid.errorMessages["__STRUCTURE__"] = 
            "Expected " + to_string(schema.csvFields.size()) +
            " fields, but got " + to_string(record.values.size()) + ".";
        return invalid;
    }
    return nullopt;
}
