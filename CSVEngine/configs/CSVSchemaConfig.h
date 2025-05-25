#pragma once

#include <unordered_set>
#include <unordered_map>

#include "PipelineConfig.h"
#include "CSVEngine/models.h"

using namespace std;

static const unordered_map<Pipeline, CSVSchema> schemaMap = {
    //STEP 2
    { Pipeline::Pulmonary, {"firstName", "lastName", "ssn"} },
    { Pipeline::Heart, {"firstName", "lastName", "ssn"} },
    { Pipeline::Plastic, {"firstName", "lastName", "ssn"} }
};

inline vector<string> getFieldsVectorFor(Pipeline type) {
    return schemaMap.at(type).csvFields;
}

inline unordered_set<string> getFieldsSetFor(Pipeline type) {
    return unordered_set<string>(
        schemaMap.at(type).csvFields.begin(), 
        schemaMap.at(type).csvFields.end()
    );
}