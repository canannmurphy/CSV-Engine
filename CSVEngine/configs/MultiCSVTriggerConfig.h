#pragma once
#include <string>
#include "PipelineConfig.h"
#include "CSVEngine/models.h"


inline MultiCSVConfig MultiTypeTriggerConfig() {
    //STEP 4 (optional)
    return {
        0, // Default column index for the trigger
        {
            { "PC", Pipeline::Pulmonary },
            { "HC", Pipeline::Heart },
            { "PSC", Pipeline::Plastic },
        }
    };
}

