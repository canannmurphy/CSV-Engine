#pragma once

#include <fstream>
#include <sstream>
#include <iostream>
#include "CSVEngine/models.h"
#include "CSVEngine/recordProcessor.h"

using namespace std;

inline void loadMultiTypeCSV(
    PipelineRegistry& registry,
    const string& filePath,
    const MultiCSVConfig& config
) {
    ifstream file(filePath);
    if (!file.is_open()) {
        cerr << "Error opening file: " << filePath << endl;
        return;
    }

    string line;
    int rowNumber = 0;

    while (getline(file, line)) {
        stringstream ss(line);
        string cell;
        vector<string> values;

        while (getline(ss, cell, ',')) {
            values.push_back(cell);
        }

        if (values.size() <= config.triggerColumn) {
            cerr << "Row " << rowNumber + 1 << " is too short to contain a trigger column.\n";
            continue;
        }

        string trigger = values[config.triggerColumn];
        auto it = config.triggerMap.find(trigger);
        if (it == config.triggerMap.end()) {
            cerr << "Unknown trigger '" << trigger << "' at row " << rowNumber + 1 << "\n";
            ++rowNumber;
            continue;
        }

        Pipeline type = it->second;
        values.erase(values.begin() + config.triggerColumn); // remove trigger field

      
        //STEP 7 (optional)


        CSVRecord record;
        record.rowNumber = ++rowNumber;
        record.values = values;
        record.sourceType = filePath;
        record.isFromMultiType = true;

        processRecord(registry.pipeline[type], record, type);
    }
}
