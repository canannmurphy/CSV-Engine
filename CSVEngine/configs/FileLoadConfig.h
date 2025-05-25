#pragma once

#include <string>
#include <optional>
#include "CSVEngine/configs/PipelineConfig.h"
#include "CSVEngine/models.h"
#include "CSVEngine/loaders/singleCSVLoader.h"
#include "CSVEngine/loaders/multiCSVLoader.h"
#include "CSVEngine/configs/MultiCSVTriggerConfig.h"

using namespace std;

inline const vector<FileLoadConfig> fileLoadConfigs = {
    //STEP 3
    
    //Single-type CSVs
    // { "Data/person.csv", false, LoaderType::Single, Pipeline::Person, nullopt },
    
    //Multi-type CSVs
    { "Data/patient.csv", false, LoaderType::Multi, nullopt, MultiTypeTriggerConfig() }
};

inline void loadFiles(PipelineRegistry& registry, const vector<FileLoadConfig>& configs, bool debug = false) {
    for (const auto& config : configs) {
        if(debug == true) {
            cout << "Loading file: " << config.filePath << endl;
        }
            

        if (config.loaderType == LoaderType::Single) {
            if (!config.target.has_value()) {
                cerr << "Error: Single loader requires a target pipeline for " << config.filePath << endl;
                continue;
            }

            loadSingleTypeCSV(registry, config.filePath, config.target.value(), config.hasHeader);
        } else if (config.loaderType == LoaderType::Multi) {
            loadMultiTypeCSV(registry, config.filePath, config.multiConfig.value());
        }
    }
}
