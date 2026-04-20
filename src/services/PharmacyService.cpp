#include "services/PharmacyService.h"
#include <ctime>
#include <iomanip>
#include <sstream>

std::string PharmacyService::GetCurrentDateString() const {
    std::time_t t = std::time(nullptr);
    std::tm* now = std::localtime(&t);
    std::stringstream ss;
    ss << (now->tm_year + 1900) << '-' 
       << std::setfill('0') << std::setw(2) << (now->tm_mon + 1) << '-'
       << std::setfill('0') << std::setw(2) << now->tm_mday;
    return ss.str();
}

static std::time_t ParseDate(const std::string& str) {
    if (str.length() < 10) return 0;
    try {
        std::tm t = {};
        t.tm_year = std::stoi(str.substr(0, 4)) - 1900;
        t.tm_mon  = std::stoi(str.substr(5, 2)) - 1;
        t.tm_mday = std::stoi(str.substr(8, 2));
        t.tm_isdst = -1; 
        return std::mktime(&t);
    } catch (...) {
        return 0; // Fallback an toàn, tránh việc văng app
    }
}

bool PharmacyService::isExpired(const std::string& expiryDate) const {
    // Iso date formats sort lexicographically trivially! 
    std::string currentStr = GetCurrentDateString();
    return expiryDate < currentStr; 
}

bool PharmacyService::isNearExpiry(const std::string& expiryDate, int daysThreshold) const {
    if (isExpired(expiryDate)) return false;

    std::time_t current = std::time(nullptr);
    std::time_t expiry = ParseDate(expiryDate);
    
    if (expiry == 0) return false;
    
    double diffDays = std::difftime(expiry, current) / (60 * 60 * 24);
    return diffDays <= daysThreshold;
}

bool PharmacyService::sellMedicine(int medicineId, int quantity, const std::string& customerName, std::string& outMessage) {
    // 1. Get medicine
    Medicine med = medRepo->GetById(medicineId);
    if (med.GetId() == 0) {
        outMessage = "Medicine not found.";
        return false;
    }

    // 2. Check stock
    if (med.GetQuantity() < quantity) {
        outMessage = "Insufficient stock available.";
        return false;
    }

    // 3. Check expiry
    if (isExpired(med.GetExpiryDate())) {
        outMessage = "Cannot sell expired medicine!";
        return false;
    }

    // 4. Reduce quantity
    med.SetQuantity(med.GetQuantity() - quantity);
    if (!medRepo->Update(med)) {
        outMessage = "Failed to update medicine inventory.";
        return false;
    }

    // 5. Create invoice
    Invoice newInvoice;
    newInvoice.SetDate(GetCurrentDateString());
    newInvoice.SetTotal(med.GetPrice() * quantity);
    newInvoice.SetCustomerName(customerName);
    
    if (!invRepo->Insert(newInvoice)) {
        outMessage = "Failed to create invoice.";
        return false;
    }

    // 6. Save invoice items
    InvoiceItem item;
    item.SetInvoiceId(newInvoice.GetId());
    item.SetMedicineId(med.GetId());
    item.SetQuantity(quantity);
    item.SetPrice(med.GetPrice());

    if (!invRepo->InsertItem(item)) {
        outMessage = "Partial completion: Invoice created but item log failed.";
        return false;
    }

    outMessage = "Sale completed successfully!";
    return true;
}
