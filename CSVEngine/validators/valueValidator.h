#pragma once

#include "CSVEngine/models.h"
#include "CSVEngine/configs/ValueValidatorConfig.h"

 // Function to validate a single value for a field
 pair<vector<ErrorType>, string> validateValue(
    const string& field, 
    const string& value, 
    Pipeline pipelineType) {
    // Get the global invalid tokens
    const auto& globalInvalidTokens = getGlobalInvalidTokens();

        // Check for globally invalid tokens
        if (globalInvalidTokens.find(value) != globalInvalidTokens.end()) {
            return { { ErrorType::InvalidGlobalToken }, "Value is globally invalid" };
        }

        // Get the validation rules for the field based on pipeline type
        auto validators = getValueValidators(pipelineType);  // Use dynamic pipelineType here

        // Iterate through the validation rules for the given field
        for (const auto& rule : validators) {
            if (field == rule.fieldName) {
                // Check if value is empty and not allowed to be
                if (!rule.allowEmpty && value.empty()) {
                    return { { ErrorType::EmptyNotAllowed }, "Cannot be empty" };
                }

                // Check if the value matches any disallowed values
                for (const string& disallowed : rule.disallowedValues) {
                    if (value == disallowed) {
                        return { { ErrorType::DisallowedValue }, "This value is disallowed" };  // Disallowed value found
                    }
                }

                // Apply the validation functions for this field
                for (const auto& test : rule.validationFunctions) {
                    if (!test.test(value)) {
                        return { { ErrorType::InvalidFieldValue }, test.errorMessage };  // Validation failed
                    }
                }

                // All tests passed for this field
                return { {}, "" };
            }
        }

    // If no validation rules found, consider the value valid (no specific validation rules)
    return { {}, "" };
}

// Function to validate a record's fields and return whether the entire record is valid
inline bool validateRecordFields(
    const CSVRecord& record,
    Pipeline pipelineType,
    InvalidRecord& outInvalid) {

    bool isValid = true;

    const auto& validators = getValueValidators(pipelineType);

    for (const auto& rule : validators) {
        auto it = record.namedValues.find(rule.fieldName);
        if (it == record.namedValues.end()) {
            continue;
        }

        const string& value = it->second;

        auto errors = validateValue(rule.fieldName, value, pipelineType);
        if (!errors.first.empty()) {
            isValid = false;
            outInvalid.valueErrors.insert(errors.first.begin(), errors.first.end());
            outInvalid.errorMessages[rule.fieldName] = errors.second;
        }
    }

    return isValid;
}