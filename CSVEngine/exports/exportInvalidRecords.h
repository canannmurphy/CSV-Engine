#pragma once
#include "CSVEngine/models.h"
#include "CSVEngine/utils/magic_enum.h"
#include "CSVEngine/configs/DerivedFieldConfig.h"
#include <fstream>
#include <iostream>
#include <unordered_set>
using namespace std;
using namespace magic_enum;


inline void exportInvalidRecords(const PipelineRegistry& registry, const string& filename) {
    ofstream out(filename);
    if (!out.is_open()) {
        cerr << "Failed to open file for writing: " << filename << endl;
        return;
    }

    out << "--- Invalid Records by Pipeline ---\n";
    for (const auto& [type, state] : registry.pipeline) {
        if (!state.invalidRecords.empty()) {
            unordered_set<string> derivedFieldNames;
            for (const auto& field : getDerivedFields(type)) {
                derivedFieldNames.insert(field.fieldName);
            }
            out << "\nPipeline: " << enum_name(type) << "\n";
            for (const auto& record : state.invalidRecords) {
                out << "Row " << record.rowNumber << " (Source: " << record.sourceType << ")\n";
                out << "  Errors: ";
                for (const auto& error : record.valueErrors) {
                    out << "[" << enum_name(error) << "] ";
                }
                out << "\n  Details: ";
                for (const auto& [field, message] : record.errorMessages) {
                    string label = field;
                    if (derivedFieldNames.count(field)) label += "*";
                    out << "[" << label << ": " << message << "] ";
                }
                out << "\n";
            }
            out << "---\n";
        }
    }
    out.close();
}

