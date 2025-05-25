#pragma once

#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>

#include "CSVEngine/models.h"
#include "CSVEngine/configs/CommandConfig.h"

inline void loadCommandsFromFile(const string& path, PipelineRegistry& registry) {
    ifstream file(path);
    if (!file.is_open()) {
        cerr << "Failed to open command file: " << path << endl;
        return;
    }

    string line;
    auto executors = getCommandExecutors();
    
    while (getline(file, line)) {
        istringstream ss(line);
        string token;
        vector<string> tokens;

        while (getline(ss, token, ',')) {
            tokens.push_back(token);
        }

        if (tokens.empty()) continue;

        string command = tokens[0];
        auto it = executors.find(command);
        if (it != executors.end()) {
            const auto& executor = it->second;

            //STEP 7 (optional)


            executor.handler(tokens, registry);
        } else {
            InvalidRecord invalid;
            invalid.rowNumber = -1;
            invalid.sourceType = "command";
            invalid.valueErrors.insert(ErrorType::UnknownCommandTrigger);
            invalid.errorMessages["CommandTrigger"] = "Unrecognized command trigger-> " + command;

            // Temporarily route to the Plastic pipeline for logging
            registry.pipeline[Pipeline::Plastic].invalidRecords.push_back(invalid);
        }
    }

    file.close();
}