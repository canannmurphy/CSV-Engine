#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include <optional>
#include <functional>
#include <set>

#include "configs/PipelineConfig.h"

using namespace std;

enum class LoaderType {
    Single,
    Multi
};

enum class ErrorType {
    InvalidGlobalToken,
    MissingDependencies,
    InvalidFieldValue,
    EmptyNotAllowed,
    DisallowedValue,
    FieldCountMismatch,
    UnknownPipelineTrigger,
    UnknownCommandTrigger
};

struct CSVSchema {
    vector<string> csvFields;
    CSVSchema() = default;
    CSVSchema(initializer_list<string> fields):csvFields(fields) {}
};

struct CSVRecord {
    int rowNumber;
    vector<string> values;
    string sourceType; // Indicates where the record originated
    bool isFromMultiType = false;
    unordered_map<string, string> namedValues;
};

struct MultiCSVConfig {
    int triggerColumn;
    unordered_map<string, Pipeline> triggerMap;
};


struct FileLoadConfig {
    string filePath;
    bool hasHeader;
    LoaderType loaderType;     // enum: Single or Multi
    optional<Pipeline> target; // only needed for SingleType
    optional<MultiCSVConfig> multiConfig; // only needed for MultiType
};

struct FieldTest {
    function<bool(const string&)> test;
    string errorMessage;
};

struct ValueValidationRule {
    string fieldName;
    vector<FieldTest> validationFunctions;
    bool allowEmpty;
    vector<string> disallowedValues;
};

struct InvalidRecord {
    int rowNumber;
    set<ErrorType> valueErrors;   // Type of error (structure or value validation)
    unordered_map<string, string> errorMessages;  // Error details for each invalid field
    string sourceType; // Indicates where the record originated
};

struct DerivedField {
    string fieldName;
    vector<string> dependencies;
    function<string(const unordered_map<string, string>&)> compute;
};


struct PipelineState {
    CSVSchema schema;
    vector<CSVRecord> records;
    vector<InvalidRecord> invalidRecords;
};

struct PipelineRegistry {
    unordered_map<Pipeline, PipelineState> pipeline;
    vector<InvalidRecord> commandRoutingErrors; 
};

struct CommandExecutor {
    string name;
    function<void(const vector<string>&, PipelineRegistry&)> handler;
    Pipeline targetPipeline;
 
};
