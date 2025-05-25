#pragma once

#include "models.h"
#include "configs/CSVSchemaConfig.h"
#include "configs/PipelineConfig.h"

inline PipelineRegistry initRegistry() {
    PipelineRegistry registry;
    for (int i = 0; i < static_cast<int>(Pipeline::END); i++) {
        Pipeline type = static_cast<Pipeline>(i);
        if (schemaMap.find(type) != schemaMap.end()) {
            registry.pipeline[type].schema = schemaMap.at(type);
        }
    }
    return registry;
}