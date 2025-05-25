#pragma once

#include <string>
#include <functional>
#include <vector>

#include "CSVEngine/models.h"
#include "CSVEngine/recordProcessor.h"
#include "CSVEngine/commands/CreateCommand.h"

using namespace std;

void createCommand(const vector<string>& tokens, PipelineRegistry& registry);

inline unordered_map<string, CommandExecutor> getCommandExecutors() {
    //STEP 5 (optional)
    return {
        // { "N", { "CreateRecord", createCommand } },
        // { "A", { "PrintAllMedia", [](const auto&, auto&) {} } },
    };
}