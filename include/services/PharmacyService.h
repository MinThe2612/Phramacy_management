#pragma once
#include <string>
#include <vector>
#include "repositories/MedicineRepository.h"
#include "repositories/InvoiceRepository.h"

class PharmacyService {
private:
    MedicineRepository* medRepo;
    InvoiceRepository* invRepo;

    // Helper utility to get the current unformatted system time cleanly as YYYY-MM-DD
    std::string GetCurrentDateString() const;

public:
    // Takes ownership bounds by references passed from standard instantiations
    PharmacyService(MedicineRepository* mRepo, InvoiceRepository* iRepo) 
        : medRepo(mRepo), invRepo(iRepo) {}

    // Expiry Check Logic
    bool isExpired(const std::string& expiryDate) const;
    bool isNearExpiry(const std::string& expiryDate, int daysThreshold = 30) const;

    // Core Interlocking Business System
    // Returns boolean indicating success and populates outMessage detailing the rejection reasoning cleanly
    bool sellMedicine(int medicineId, int quantity, const std::string& customerName, std::string& outMessage);
};
