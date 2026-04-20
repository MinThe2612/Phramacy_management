#include "repositories/MedicineRepository.h"
#include <iostream>

bool MedicineRepository::Insert(Medicine& item) {
    std::string sql = "INSERT INTO medicines (name, price, quantity, expiry_date, category_id) VALUES (?, ?, ?, ?, ?);";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) return false;

    sqlite3_bind_text(stmt, 1, item.GetName().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_double(stmt, 2, item.GetPrice());
    sqlite3_bind_int(stmt, 3, item.GetQuantity());
    sqlite3_bind_text(stmt, 4, item.GetExpiryDate().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 5, item.GetCategoryId());

    bool result = (sqlite3_step(stmt) == SQLITE_DONE);
    if (result) {
        item.SetId((int)sqlite3_last_insert_rowid(db));
    }
    sqlite3_finalize(stmt);
    return result;
}

bool MedicineRepository::Update(const Medicine& item) {
    std::string sql = "UPDATE medicines SET name=?, price=?, quantity=?, expiry_date=?, category_id=? WHERE id=?;";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) return false;

    sqlite3_bind_text(stmt, 1, item.GetName().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_double(stmt, 2, item.GetPrice());
    sqlite3_bind_int(stmt, 3, item.GetQuantity());
    sqlite3_bind_text(stmt, 4, item.GetExpiryDate().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 5, item.GetCategoryId());
    sqlite3_bind_int(stmt, 6, item.GetId());

    bool result = (sqlite3_step(stmt) == SQLITE_DONE);
    sqlite3_finalize(stmt);
    return result;
}

bool MedicineRepository::Delete(int id) {
    std::string sql = "DELETE FROM medicines WHERE id=?;";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) return false;

    sqlite3_bind_int(stmt, 1, id);
    bool result = (sqlite3_step(stmt) == SQLITE_DONE);
    sqlite3_finalize(stmt);
    return result;
}

Medicine MedicineRepository::GetById(int id) {
    Medicine med;
    std::string sql = "SELECT id, name, price, quantity, expiry_date, category_id FROM medicines WHERE id=?;";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, id);
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            med.SetId(sqlite3_column_int(stmt, 0));
            const char* name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            med.SetName(name ? name : "");
            med.SetPrice(sqlite3_column_double(stmt, 2));
            med.SetQuantity(sqlite3_column_int(stmt, 3));
            const char* expiry = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
            med.SetExpiryDate(expiry ? expiry : "");
            med.SetCategoryId(sqlite3_column_int(stmt, 5));
        }
    }
    sqlite3_finalize(stmt);
    return med;
}

std::vector<Medicine> MedicineRepository::GetAll() {
    std::vector<Medicine> list;
    std::string sql = "SELECT id, name, price, quantity, expiry_date, category_id FROM medicines;";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            Medicine med;
            med.SetId(sqlite3_column_int(stmt, 0));
            const char* name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            med.SetName(name ? name : "");
            med.SetPrice(sqlite3_column_double(stmt, 2));
            med.SetQuantity(sqlite3_column_int(stmt, 3));
            const char* expiry = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
            med.SetExpiryDate(expiry ? expiry : "");
            med.SetCategoryId(sqlite3_column_int(stmt, 5));
            list.push_back(med);
        }
    }
    sqlite3_finalize(stmt);
    return list;
}
