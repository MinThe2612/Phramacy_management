#pragma once
#include "models/Invoice.h"
#include "IRepository.h"
#include <sqlite3.h>
#include <vector>
#include <string>
#include <utility>

class InvoiceRepository : public IRepository<Invoice> {
private:
    sqlite3* db;

public:
    explicit InvoiceRepository(sqlite3* database) : db(database) {}

    bool Insert(Invoice& item) override;
    bool Update(const Invoice& item) override;
    bool Delete(int id) override;
    Invoice GetById(int id) override;
    std::vector<Invoice> GetAll() override;

    // Invoice Items
    bool InsertItem(InvoiceItem& item);
    std::vector<InvoiceItem> GetItemsForInvoice(int invoiceId);

    // Revenue statistics
    std::vector<std::pair<std::string, double>> GetRevenueByDay();
    std::vector<std::pair<std::string, double>> GetRevenueByMonth();
    std::vector<std::pair<std::string, double>> GetRevenueByQuarter();
};
