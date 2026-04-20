#pragma once
#include <string>

class InvoiceItem {
private:
    int id;
    int invoiceId;
    int medicineId;
    int quantity;
    double price;

public:
    InvoiceItem() : id(0), invoiceId(0), medicineId(0), quantity(0), price(0.0) {}
    
    int GetId() const { return id; }
    void SetId(int v) { id = v; }

    int GetInvoiceId() const { return invoiceId; }
    void SetInvoiceId(int v) { invoiceId = v; }

    int GetMedicineId() const { return medicineId; }
    void SetMedicineId(int v) { medicineId = v; }

    int GetQuantity() const { return quantity; }
    void SetQuantity(int v) { quantity = v; }

    double GetPrice() const { return price; }
    void SetPrice(double v) { price = v; }
};

class Invoice {
private:
    int id;
    std::string date;
    double total;
    std::string customerName;

public:
    Invoice() : id(0), total(0.0) {}

    int GetId() const { return id; }
    void SetId(int v) { id = v; }

    const std::string& GetDate() const { return date; }
    void SetDate(const std::string& v) { date = v; }

    double GetTotal() const { return total; }
    void SetTotal(double v) { total = v; }

    const std::string& GetCustomerName() const { return customerName; }
    void SetCustomerName(const std::string& v) { customerName = v; }
};
