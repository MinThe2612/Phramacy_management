#include "repositories/CategoryRepository.h"
#include <iostream>

bool CategoryRepository::Insert(Category& item) {
    std::string sql = "INSERT INTO categories (name) VALUES (?);";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) return false;

    sqlite3_bind_text(stmt, 1, item.GetName().c_str(), -1, SQLITE_TRANSIENT);
    bool result = (sqlite3_step(stmt) == SQLITE_DONE);
    if (result) item.SetId((int)sqlite3_last_insert_rowid(db));
    sqlite3_finalize(stmt);
    return result;
}

bool CategoryRepository::Update(const Category& item) {
    std::string sql = "UPDATE categories SET name=? WHERE id=?;";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) return false;

    sqlite3_bind_text(stmt, 1, item.GetName().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 2, item.GetId());
    bool result = (sqlite3_step(stmt) == SQLITE_DONE);
    sqlite3_finalize(stmt);
    return result;
}

bool CategoryRepository::Delete(int id) {
    std::string sql = "DELETE FROM categories WHERE id=?;";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) return false;

    sqlite3_bind_int(stmt, 1, id);
    bool result = (sqlite3_step(stmt) == SQLITE_DONE);
    sqlite3_finalize(stmt);
    return result;
}

Category CategoryRepository::GetById(int id) {
    Category cat;
    std::string sql = "SELECT id, name FROM categories WHERE id=?;";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, id);
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            cat.SetId(sqlite3_column_int(stmt, 0));
            const char* name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            cat.SetName(name ? name : "");
        }
    }
    sqlite3_finalize(stmt);
    return cat;
}

std::vector<Category> CategoryRepository::GetAll() {
    std::vector<Category> list;
    std::string sql = "SELECT id, name FROM categories;";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            Category cat;
            cat.SetId(sqlite3_column_int(stmt, 0));
            const char* name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            cat.SetName(name ? name : "");
            list.push_back(cat);
        }
    }
    sqlite3_finalize(stmt);
    return list;
}
