#include "CSVEngine/CSVEngine.h"
#include "CSVEngine/diagnostics.h"
#include "CSVEngine/exports/exportCleanData.h"
#include "CSVEngine/exports/exportInvalidRecords.h"

int main() {
    PipelineRegistry registry = CSVEngine();
    exportCleanData(registry, "cleanData");
    exportInvalidRecords(registry, "../backend/reports/invalidRecordReport.txt");
    // diagnostics::InvalidRecords(registry);
    return 0;
}