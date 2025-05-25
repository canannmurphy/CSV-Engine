#pragma once

#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include "CSVEngine/models.h"
#include "CSVEngine/recordProcessor.h"

using namespace std;

inline void loadSingleTypeCSV(
    PipelineRegistry& registry,
    const string& filePath,
    Pipeline type,
    bool hasHeader
) {
    ifstream file(filePath);
    if (!file.is_open()) {
        cerr << "Error opening file: " << filePath << endl;
        return;
    }

    string line;
    int rowNumber = 1;
    bool isFirstRow = true;

    while (getline(file, line)) {
        if (hasHeader && isFirstRow) {
            isFirstRow = false;
            continue;
        }

        stringstream ss(line);
        string cell;
        vector<string> values;

        while (getline(ss, cell, ',')) {
            values.push_back(cell);
        }

      
        //STEP 7 (optional)

        CSVRecord record;
        record.rowNumber = ++rowNumber;
        record.values = values;
        record.sourceType = filePath;

        processRecord(registry.pipeline[type], record, type);
    }
}
