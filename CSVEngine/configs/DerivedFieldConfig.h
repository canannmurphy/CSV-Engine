#pragma once
#include "CSVEngine/models.h"
#include "PipelineConfig.h"
#include <unordered_map>

using namespace std;

inline vector<DerivedField> getDerivedFields(Pipeline type)
{

    // STEP 8 (optional)
    switch (type)
    {
    case Pipeline::Pulmonary:
        return {};
    default:
        return {};
    }
}

/*
Example Usage:
vector<string> fields = {"firstName", "lastName", "email"};
vector<vector<string>> records = {
    {"Alice", "Smith", "alice@example.com"},
    {"Bob", "Miller", "bob@example.com"}
};

auto derivedFields = getDerivedFields(Pipeline::Person);
applyDerivedFields(fields, records, derivedFields);

// fields now includes: "fullName", "emailDomain", "profileLine"
// records will be updated with the corresponding computed values
*/