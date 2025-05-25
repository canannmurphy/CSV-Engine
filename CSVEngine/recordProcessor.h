#pragma once

#include "models.h"
#include "CSVEngine/validators/structureValidator.h"
#include "CSVEngine/validators/valueValidator.h"
#include <iostream>

void processRecord(PipelineState& state, CSVRecord record, Pipeline pipelineType) {
    if (auto invalid = validateStructure(record, state.schema)) {
        state.invalidRecords.push_back(*invalid);
        return;
    }

    const auto& fieldNames = state.schema.csvFields;
    for (size_t i = 0; i < fieldNames.size() && i < record.values.size(); ++i) {
        record.namedValues[fieldNames[i]] = record.values[i];
    }

    InvalidRecord valueInvalid;
    valueInvalid.rowNumber = record.rowNumber;
    valueInvalid.sourceType = record.sourceType;

    if (!validateRecordFields(record, pipelineType, valueInvalid)) {
        state.invalidRecords.push_back(valueInvalid);
        return;
    }

    state.records.push_back(record);
}