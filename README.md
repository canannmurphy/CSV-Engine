# CSV Engine Process
This document outlines the step-by-step configuration and setup of the CSV processing engine.

---

## Step 1: Define Pipeline Types
**File:** `PipelineConfig.h`  
Establish the various pipeline types used in your engine.

---

## Step 2: Define CSV Schemas
**File:** `CSVSchemaConfig.h`  
Specify the fields expected for each pipeline.

---

## Step 3: Configure File Loading
**File:** `FileLoadConfig.h`  
Define the configuration for loading non-clean CSV files:
- **Single-type CSV**: Pass the pipeline directly. Do **not** pass a trigger config.
- **Multi-type CSV**: Do **not** pass a pipeline. A trigger config **must** be provided.

---

## Step 4 (Optional): Multi-Type CSV Trigger
**File:** `MultiCSVTriggerConfig.h`  
Configure:
- The trigger column index (must be the same for all records).
- The value in that column that identifies each pipeline.

---

## Step 5 (Optional): Configure Commands
**File:** `CommandConfig.h`  
Set up:
- The command keywords.
- The corresponding handler functions.

---

## Step 6 (Optional): Implement Command Handlers
**Folder:** `commands/`  
Add the function implementations for each command trigger.

---

## Step 7 (Optional): Insert Custom Preprocessing Logic
Insert custom logic in each loader to preprocess a record **before** it's passed to the processor.

---

## Step 8 (Optional): Define Derived Fields
**File:** `DerivedFieldsConfig.h`  
Specify derived fields and how to compute them.  
**Note:** Circular dependencies will halt the program.

---

## Step 9: Define Validation Rules
**File:** `ValueValidatorConfig.h`  
Provide validation logic for specific fields.

---

## Step 10: Entry Point - Load CSV
**File:** `CSVEngine.h`  
Choose the appropriate function to load data:
- From standard CSV files.
- From command-based CSVs.
