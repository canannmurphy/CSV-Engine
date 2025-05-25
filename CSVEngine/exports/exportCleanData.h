#pragma once
#include "../models.h"
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <string>
#include "CSVEngine/utils/magic_enum.h"

void exportCleanData(const PipelineRegistry& registry, const std::string& basePath) {
    for (const auto& [pipeline, state] : registry.pipeline) {
        std::string filename = "../backend/data/"+ basePath + "_" + std::string(magic_enum::enum_name(pipeline)) + ".csv";
        std::ofstream outFile(filename);
        if (!outFile.is_open()) continue;

        bool headerWritten = false;

        for (const auto& record : state.records) {
            if (!headerWritten) {
                outFile << "pipeline,rowNumber,sourceType";
                for (const auto& [field, value] : record.namedValues) {
                    outFile << "," << field;
                }
                outFile << "\n";
                headerWritten = true;
            }

            outFile << magic_enum::enum_name(pipeline)
                    << "," << record.rowNumber
                    << "," << record.sourceType;
            for (const auto& [field, value] : record.namedValues) {
                outFile << "," << value;
            }
            outFile << "\n";
        }

        outFile.close();
    }
}
