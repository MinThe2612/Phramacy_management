#include "repositories/InvoiceRepository.h"

bool InvoiceRepository::Insert(Invoice& item) {
    std::string sql = "INSERT INTO invoices (date, total, customer_name) VALUES (?, ?, ?);";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) return false;

    sqlite3_bind_text(stmt, 1, item.GetDate().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_double(stmt, 2, item.GetTotal());
    sqlite3_bind_text(stmt, 3, item.GetCustomerName().c_str(), -1, SQLITE_TRANSIENT);
    bool result = (sqlite3_step(stmt) == SQLITE_DONE);
    if (result) item.SetId((int)sqlite3_last_insert_rowid(db));
    sqlite3_finalize(stmt);
    return result;
}

bool InvoiceRepository::Update(const Invoice& item) {
    std::string sql = "UPDATE invoices SET date=?, total=?, customer_name=? WHERE id=?;";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) return false;

    sqlite3_bind_text(stmt, 1, item.GetDate().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_double(stmt, 2, item.GetTotal());
    sqlite3_bind_text(stmt, 3, item.GetCustomerName().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 4, item.GetId());
    bool result = (sqlite3_step(stmt) == SQLITE_DONE);
    sqlite3_finalize(stmt);
    return result;
}

bool InvoiceRepository::Delete(int id) {
    std::string sql = "DELETE FROM invoices WHERE id=?;";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) return false;

    sqlite3_bind_int(stmt, 1, id);
    bool result = (sqlite3_step(stmt) == SQLITE_DONE);
    sqlite3_finalize(stmt);
    return result;
}

Invoice InvoiceRepository::GetById(int id) {
    Invoice inv;
    std::string sql = "SELECT id, date, total, customer_name FROM invoices WHERE id=?;";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, id);
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            inv.SetId(sqlite3_column_int(stmt, 0));
            const char* d = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            inv.SetDate(d ? d : "");
            inv.SetTotal(sqlite3_column_double(stmt, 2));
            const char* cn = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
            inv.SetCustomerName(cn ? cn : "");
        }
    }
    sqlite3_finalize(stmt);
    return inv;
}

std::vector<Invoice> InvoiceRepository::GetAll() {
    std::vector<Invoice> list;
    std::string sql = "SELECT id, date, total, customer_name FROM invoices ORDER BY id DESC;";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            Invoice inv;
            inv.SetId(sqlite3_column_int(stmt, 0));
            const char* d = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            inv.SetDate(d ? d : "");
            inv.SetTotal(sqlite3_column_double(stmt, 2));
            const char* cn = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
            inv.SetCustomerName(cn ? cn : "");
            list.push_back(inv);
        }
    }
    sqlite3_finalize(stmt);
    return list;
}

bool InvoiceRepository::InsertItem(InvoiceItem& item) {
    std::string sql = "INSERT INTO invoice_items (invoice_id, medicine_id, quantity, price) VALUES (?, ?, ?, ?);";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) return false;

    sqlite3_bind_int(stmt, 1, item.GetInvoiceId());
    sqlite3_bind_int(stmt, 2, item.GetMedicineId());
    sqlite3_bind_int(stmt, 3, item.GetQuantity());
    sqlite3_bind_double(stmt, 4, item.GetPrice());
    
    bool result = (sqlite3_step(stmt) == SQLITE_DONE);
    if (result) item.SetId((int)sqlite3_last_insert_rowid(db));
    sqlite3_finalize(stmt);
    return result;
}

std::vector<InvoiceItem> InvoiceRepository::GetItemsForInvoice(int invoiceId) {
    std::vector<InvoiceItem> list;
    std::string sql = "SELECT id, invoice_id, medicine_id, quantity, price FROM invoice_items WHERE invoice_id=?;";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, invoiceId);
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            InvoiceItem item;
            item.SetId(sqlite3_column_int(stmt, 0));
            item.SetInvoiceId(sqlite3_column_int(stmt, 1));
            item.SetMedicineId(sqlite3_column_int(stmt, 2));
            item.SetQuantity(sqlite3_column_int(stmt, 3));
            item.SetPrice(sqlite3_column_double(stmt, 4));
            list.push_back(item);
        }
    }
    sqlite3_finalize(stmt);
    return list;
}

// --- Revenue Statistics ---

static std::vector<std::pair<std::string, double>> RunRevenueQuery(sqlite3* db, const std::string& sql) {
    std::vector<std::pair<std::string, double>> result;
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            const char* label = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
            double total = sqlite3_column_double(stmt, 1);
            result.push_back({ label ? label : "", total });
        }
    }
    sqlite3_finalize(stmt);
    return result;
}

std::vector<std::pair<std::string, double>> InvoiceRepository::GetRevenueByDay() {
    return RunRevenueQuery(db,
        "SELECT date, SUM(total) FROM invoices GROUP BY date ORDER BY date DESC;");
}

std::vector<std::pair<std::string, double>> InvoiceRepository::GetRevenueByMonth() {
    return RunRevenueQuery(db,
        "SELECT substr(date,1,7) AS month, SUM(total) FROM invoices GROUP BY month ORDER BY month DESC;");
}

std::vector<std::pair<std::string, double>> InvoiceRepository::GetRevenueByQuarter() {
    return RunRevenueQuery(db,
        "SELECT substr(date,1,4) || '-Q' || ((CAST(substr(date,6,2) AS INTEGER)-1)/3 + 1) AS quarter, "
        "SUM(total) FROM invoices GROUP BY quarter ORDER BY quarter DESC;");
}
