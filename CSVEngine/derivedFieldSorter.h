#pragma once
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <stdexcept>
#include <iostream>
#include "CSVEngine/models.h"

using namespace std;

inline vector<DerivedField> sortDerivedFields(
    const vector<DerivedField>& fields,
    const unordered_set<string>& baseFields,
    bool debug = false
) {
    if (fields.empty()) return {};

    unordered_map<string, vector<string>> dependencyGraph;
    unordered_map<string, int> inDegree;
    unordered_map<string, DerivedField> fieldMap;

    for (const auto& field : fields) {
        if (fieldMap.count(field.fieldName)) {
            throw runtime_error("Duplicate derived field name: " + field.fieldName);
        }
        fieldMap[field.fieldName] = field;
        if (!inDegree.count(field.fieldName)) {
            inDegree[field.fieldName] = 0;
        }
        for (const auto& dep : field.dependencies) {
            dependencyGraph[dep].push_back(field.fieldName);
            inDegree[field.fieldName]++;
        }
    }

    for (const auto& base : baseFields) {
        if (!inDegree.count(base)) {
            inDegree[base] = 0;
        }
    }

    queue<string> q;
    for (const auto& [name, degree] : inDegree) {
        if (degree == 0) q.push(name);
    }

    vector<DerivedField> sorted;
    while (!q.empty()) {
        string current = q.front();
        q.pop();
        if (fieldMap.count(current)) {
            sorted.push_back(fieldMap[current]);
        }
        for (const auto& neighbor : dependencyGraph[current]) {
            if (--inDegree[neighbor] == 0) {
                q.push(neighbor);
            }
        }
    }

    if (sorted.size() != fieldMap.size()) {
        if (debug) {
            cerr << "⚠ Unresolved derived fields due to possible cycle or missing dependencies:\n";
            for (const auto& [name, degree] : inDegree) {
                if (fieldMap.count(name) && degree > 0) {
                    cerr << "- " << name << " (in-degree: " << degree << ")\n";
                }
            }
        }
        throw runtime_error("Derived field dependency cycle detected or missing base field.");
    }

    return sorted;
}
