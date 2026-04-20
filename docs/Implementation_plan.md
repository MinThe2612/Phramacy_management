# Pharmacy Management System Implementation Plan

## Goal Description
Develop a lightweight desktop application with a tiered architecture backing a SQLite database and Dear ImGui front-end. The system will handle medicine stocks, category hierarchies, and enforcing business rules around stock expiry.

## Proposed Changes

### Project Foundation
- [NEW] `CMakeLists.txt`
- [NEW] `main.cpp`

### Models
- [NEW] `Medicine.h`
- [NEW] `Category.h`
- [NEW] `Invoice.h`

### Data / Repositories
- [NEW] `DatabaseContext.h`
- [NEW] `MedicineRepository.h`
- [NEW] `MedicineRepository.cpp`

### Core Services
- [NEW] `PharmacyService.h`
- [NEW] `PharmacyService.cpp`

### User Interface
- [NEW] `UIManager.h`
- [NEW] `UIManager.cpp`

## Verification Plan

### Automated Tests
Since this is an Object-Oriented project prioritizing functionality, core logic (like expiry checks and stock reductions) will be tested via embedded test runs in a debug configuration.

### Manual Verification
1. Compile and launch the binary (`PharmacySystem.exe`).
2. Generate dummy data with near-expiry items and verify warnings appear.
3. Run the POS/sales flow and verify stock reduces correctly.
