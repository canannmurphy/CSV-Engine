#pragma once
#include "models.h"
#include "initRegistry.h"
#include "configs/FileLoadConfig.h"
#include "loaders/singleCSVLoader.h"
#include "loaders/multiCSVLoader.h"
#include "loaders/commandCSVLoader.h"
#include "configs/DerivedFieldConfig.h"
#include "csvTransformer.h"
#include "derivedFieldSorter.h"

inline PipelineRegistry CSVEngine() {
    PipelineRegistry registry = initRegistry();
    //STEP 10
    loadFiles(registry, fileLoadConfigs);
    // loadCommandsFromFile("Data/mediaCommands.txt", registry);

    for (auto& [type, state] : registry.pipeline) {
        vector<string> updatedSchema = state.schema.csvFields;
        unordered_set<string> baseFields(updatedSchema.begin(), updatedSchema.end());

        vector<DerivedField> unsorted = getDerivedFields(type);
        vector<DerivedField> sorted = sortDerivedFields(unsorted, baseFields);

        applyDerivedFields(updatedSchema, state.records, sorted);
        state.schema.csvFields = updatedSchema;
    }

    return registry;
}
