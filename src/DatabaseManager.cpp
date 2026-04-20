#include "DatabaseManager.h"
#include <fstream>
#include <sstream>
#include <iostream>

bool ExecuteSQL(sqlite3* db, const std::string& sql) {
    char* errMsg = nullptr;
    int rc = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL Error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
        return false;
    }
    return true;
}

bool InitDatabase(sqlite3** db, const std::string& dbPath, const std::string& scriptPath) {
    // 1. Open or create database
    int rc = sqlite3_open(dbPath.c_str(), db);
    if (rc != SQLITE_OK) {
        std::cerr << "Cannot open database: " << sqlite3_errmsg(*db) << std::endl;
        return false;
    }

    // 2. Read initialization script
    std::ifstream file(scriptPath);
    if (!file.is_open()) {
        // Try common build folder relative paths as fallback
        file.open("../../" + scriptPath);
        if (!file.is_open()) {
            file.open("../" + scriptPath);
            if (!file.is_open()) {
                std::cerr << "Failed to open SQL script: " << scriptPath << std::endl;
                return false;
            }
        }
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string sqlScript = buffer.str();

    // 3. Execute script
    std::cout << "Executing database initialization script..." << std::endl;
    return ExecuteSQL(*db, sqlScript);
}
