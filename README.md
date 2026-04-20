# Pharmacy Management System

A lightweight, Object-Oriented C++ desktop application built for university-level management of a simple pharmacy inventory and point of sale.

## Features
- **Medicine Inventory (CRUD)**: Add and delete medicines, mapping their essential data such as name, price, stock quantity, and expiry.
- **Category Hierarchy**: Abstract medicines into generic categories.
- **Point of Sale (Sales Flow)**: Secure, atomic updates to inventory, coupled with generating financial Invoices and Item Logs for tracing sales accurately.
- **Expiry Validations**: The system intelligently scans items upon loading and highlights near-expiry items visually. In POS, attempting to sell expired drugs is physically blocked.

## Technologies Used
- **C++17** (Strict OOP Encapsulation, Polymorphism, Abstraction)
- **SQLite3** (C API for database binding)
- **Dear ImGui** (Lightweight windowing UI Layer bindings over GLFW/OpenGL3)

## Project Structure
- **/src** & **/include**: Source and Header distributions spanning `models/`, `repositories/` (SQLite handlers), `services/` (Business Logic), and `ui/` (Views).
- **/database**: Houses `init.sql`, bootstrapping empty databases locally without configuring external dependencies.

## How to Build (CMake Windows)
1. Ensure `CMake` and `Visual Studio Build Tools / MinGW` are installed globally onto your system `PATH`.
2. Open a terminal directly in the project root folder.
3. Execute the CMake configuration and build generation commands:
```bash
mkdir build
cd build
cmake ..
cmake --build . --config Debug
```
*Note: Due to our robust setup hooks, CMake natively pulls down the required GUI tools (GLFW, Dear ImGui) in the background locally! No complex manual unzipping needed.*

## How to Run
Once compiled, run the executable taking care to maintain the root directory as the local workspace (so it natively finds `database/init.sql`).

```bash
# Assuming you are inside the build folder:
cd ..
.\build\Debug\PharmacySystem.exe
```

## How to Test
The UI strictly relies on manual workflow testing against robust service logic constraints. To verify the build is 100% correct across bounds, execute this flow:

1. Launch application and click the **Inventory (Medicines)** tab.
2. Insert a valid medicine (e.g., *Aspirin*, Qty *10*, Expiry *2028-01-01*). Press `Add Medicine`.
3. Insert an intentionally expired medicine (e.g., *Old Tylenol*, Qty *5*, Expiry *2021-01-01*). Press `Add Medicine`.
4. Navigate to the **Dashboard & Warnings** tab:
    - You will automatically see `Old Tylenol` printed out loudly in **Red** asserting it's [EXPIRED].
5. Under the final tab **POS Front Desk**, search for your expired item's ID (*Old Tylenol*, e.g., ID: 2). Input `1` into Quantity, and strike `Execute Sale`.
6. **Output Alert**: Processing blocks successfully and bounces back `Cannot sell expired medicine!`
7. Reset the payload to explicitly sell your unexpired ID (*Aspirin*).
8. **Success Alert**: Output bounces back `Sale completed successfully!`. Checking the Dashboard / Inventory proves Aspirin stock natively drained by 1 item locally!
