#pragma once
#include <algorithm>
#include <unordered_set>
#include "CSVEngine/validators/valueValidator.h"
#include "PipelineConfig.h"
#include "CSVEngine/models.h"

using namespace std;


//STEP 9

inline const unordered_set<string>& getGlobalInvalidTokens() {
    static const unordered_set<string> globalInvalidTokens = {
        "err", "N/A", "-", "UNKNOWN", "ERROR", "ERR"
    };
    return globalInvalidTokens;
}

inline vector<ValueValidationRule> getValueValidators(Pipeline type) {
    if (type == Pipeline::Pulmonary || type == Pipeline::Heart || type == Pipeline::Plastic) {
        return {
            {
                "ssn",
                {
                    {
                        [](const string& val) {
                            return all_of(val.begin(), val.end(), ::isdigit);
                        },
                        "ssn must be a number"
                    },
                },
                false,
                {}
            },
            {
                "firstName",
                {
         
                },
                false,
                {}
            },
            {
                "lastName",
                {
            
                },
                false,
                {}
            },
        };
    }
    
    return {}; // default empty rule list
}