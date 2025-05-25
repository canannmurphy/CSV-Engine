#pragma once
#include <unordered_map>
#include <algorithm>
#include <stdexcept>
#include <iostream>

#include "models.h"

using namespace  std;

inline void applyDerivedFields(
    vector<string>& fields,
    vector<CSVRecord>& records,
    const vector<DerivedField>& derivedFields
) {
    for (const auto& derived : derivedFields) {
        fields.push_back(derived.fieldName);
    }

    for (auto& record : records) {
        unordered_map<string, string> recordMap;

        // Load all original values into the map
        for (size_t i = 0; i < fields.size() - derivedFields.size() && i < record.values.size(); ++i) {
            recordMap[fields[i]] = record.values[i];
        }

        for (const auto& derived : derivedFields) {
            try {
                string derivedValue = derived.compute(recordMap);
                recordMap[derived.fieldName] = derivedValue;
                record.values.push_back(derivedValue);
            } catch (const std::out_of_range& e) {
                cerr << "❌ Missing field for derived '" << derived.fieldName << "' on record:\n";
                for (const auto& [key, val] : recordMap) {
                    cerr << key << ": " << val << endl;
                }
                throw; // re-throw to preserve crash for now
            }
        }

        // Update namedValues map with original + derived values
        record.namedValues.clear();
        for (size_t i = 0; i < fields.size() && i < record.values.size(); ++i) {
            record.namedValues[fields[i]] = record.values[i];
        }
    }
}
