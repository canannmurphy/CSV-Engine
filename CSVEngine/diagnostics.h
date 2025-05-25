#pragma once
#include "models.h"
#include "CSVEngine/utils/magic_enum.h"
#include <iostream>
#include <optional>
#include <unordered_set>
#include "CSVEngine/configs/DerivedFieldConfig.h"
using namespace std;
using namespace magic_enum;

namespace diagnostics {

    inline void PipelineSchemas(const PipelineRegistry& registry, optional<Pipeline> filter = nullopt) {
        cout << "\n--- Pipeline Schemas ---\n";
        for (const auto& [type, state] : registry.pipeline) {
            if (filter && type != *filter) continue;
            unordered_set<string> derivedFieldNames;
            for (const auto& field : getDerivedFields(type)) {
                derivedFieldNames.insert(field.fieldName);
            }
            cout << "Pipeline: " << enum_name(type) << "\nFields: ";
            for (const auto& field : state.schema.csvFields) {
                string label = field;
                if (derivedFieldNames.count(field)) label += "*";
                cout << "[" << label << "] ";
            }
            cout << "\n---\n";
        }
    }

    inline void RecordCounts(const PipelineRegistry& registry, optional<Pipeline> filter = nullopt) {
        cout << "\n--- Record Counts ---\n";
        for (const auto& [type, state] : registry.pipeline) {
            if (filter && type != *filter) continue;
            cout << "Pipeline: " << enum_name(type) << " — ";
            cout << (state.records.empty() ? "No records found." : to_string(state.records.size()) + " record(s) loaded.") << "\n";
        }
    }

    inline void ValidRecords(const PipelineRegistry& registry, optional<Pipeline> filter = nullopt) {
        cout << "\n--- Valid Records ---\n";
        for (const auto& [type, state] : registry.pipeline) {
            if (filter && type != *filter) continue;
            unordered_set<string> derivedFieldNames;
            for (const auto& field : getDerivedFields(type)) {
                derivedFieldNames.insert(field.fieldName);
            }
            cout << "Pipeline: " << enum_name(type) << "\n";
            const auto& fields = state.schema.csvFields;
            for (const auto& record : state.records) {
                cout << "Row " << record.rowNumber << ": ";
                for (size_t i = 0; i < fields.size() && i < record.values.size(); ++i) {
                    string label = fields[i];
                    if (derivedFieldNames.count(label)) label += "*";
                    cout << "[" << label << ": " << record.values[i] << "] ";
                }
                cout << "\n";
            }
            cout << "---\n";
        }
    }

    inline void NamedValues(const PipelineRegistry& registry, optional<Pipeline> filter = nullopt) {
        cout << "\n--- Named Values Check ---\n";
        for (const auto& [type, state] : registry.pipeline) {
            if (filter && type != *filter) continue;
            cout << "Pipeline: " << enum_name(type) << "\n";

            unordered_set<string> derivedFieldNames;
            for (const auto& field : getDerivedFields(type)) {
                derivedFieldNames.insert(field.fieldName);
            }

            for (const auto& record : state.records) {
                cout << "Row " << record.rowNumber << " namedValues: ";
                for (const auto& [field, value] : record.namedValues) {
                    string label = field;
                    if (derivedFieldNames.count(field)) label += "*";
                    cout << "[" << label << ": " << value << "] ";
                }
                cout << "\n";
            }
            cout << "---\n";
        }
    }

    inline void InvalidRecords(const PipelineRegistry& registry, optional<Pipeline> filter = nullopt) {
        cout << "\n--- Invalid Records by Pipeline ---\n";
        for (const auto& [type, state] : registry.pipeline) {
            if (filter && type != *filter) continue;
            if (!state.invalidRecords.empty()) {
                unordered_set<string> derivedFieldNames;
                for (const auto& field : getDerivedFields(type)) {
                    derivedFieldNames.insert(field.fieldName);
                }
                cout << "Pipeline: " << enum_name(type) << "\n";
                for (const auto& record : state.invalidRecords) {
                    cout << "Row " << record.rowNumber << " (Source: " << record.sourceType << ")\n";
                    cout << "  Errors: ";
                    for (const auto& error : record.valueErrors) {
                        cout << "[" << enum_name(error) << "] ";
                    }
                    cout << "\n  Details: ";
                    for (const auto& [field, message] : record.errorMessages) {
                        string label = field;
                        if (derivedFieldNames.count(field)) label += "*";
                        cout << "[" << label << ": " << message << "] ";
                    }
                    cout << "\n";
                }
                cout << "---\n";
            }
        }
    }

    inline void CommandRoutingErrors(const PipelineRegistry& registry) {
        cout << "\n--- Invalid Command Routing Errors ---\n";
        for (const auto& record : registry.commandRoutingErrors) {
            cout << "Source: " << record.sourceType << ", Row: " << record.rowNumber << "\n";
            for (const auto& error : record.valueErrors)
                cout << "  Error: [" << enum_name(error) << "]\n";
            for (const auto& [field, msg] : record.errorMessages)
                cout << "  Detail: [" << field << ": " << msg << "]\n";
        }
        cout << "---\n";
    }

} // namespace diagnostics
