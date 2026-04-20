#pragma once

#include <sqlite3.h>
#include <string>

// Opens the database file and executes the SQL initialization script.
// Populates db handle successfully on SQLITE_OK.
bool InitDatabase(sqlite3** db, const std::string& dbPath, const std::string& scriptPath);

// Utility to execute an arbitrary SQL statement string.
bool ExecuteSQL(sqlite3* db, const std::string& sql);
