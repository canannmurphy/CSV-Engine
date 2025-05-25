#pragma once
 
#include "CSVEngine/models.h"
#include "CSVEngine/recordProcessor.h"
#include <unordered_map>
#include <algorithm>

using namespace std;



inline void createCommand(const vector<string>& tokens, PipelineRegistry& registry) {
    unordered_map<string, Pipeline> triggerMap = {
        //STEP 6 (optional)
        // { "M", Pipeline::Movie },

    };

    auto it = find_if(tokens.begin(), tokens.end(), [&](const string& token) {
        return triggerMap.find(token) != triggerMap.end();
    });
    
    if (it == tokens.end()) {
        InvalidRecord invalid;
        invalid.rowNumber = -1;
        invalid.sourceType = "command";
        invalid.valueErrors.insert(ErrorType::UnknownPipelineTrigger);
        string originalCommand;
        for (const auto& token : tokens) {
            originalCommand += token + ",";
        }
        if (!originalCommand.empty()) originalCommand.pop_back(); // Remove trailing comma

        string unrecognizedTrigger = tokens.size() > 1 ? tokens[1] : "N/A";

        invalid.errorMessages["TriggerToken"] = "Unrecognized trigger token: " + unrecognizedTrigger;
        invalid.errorMessages["Trigger"] = "Unrecognized pipeline trigger in command-> " + originalCommand;
        registry.commandRoutingErrors.push_back(invalid);
        return;
    }

    Pipeline type = triggerMap.at(*it);
    size_t triggerIndex = distance(tokens.begin(), it);

    CSVRecord record;
    record.rowNumber = -1;
    record.sourceType = "command";
    record.values.assign(tokens.begin() + triggerIndex + 1, tokens.end());

    processRecord(registry.pipeline[type], record, type);
}