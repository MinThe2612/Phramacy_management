#pragma once
#include "imgui.h"
#include "services/PharmacyService.h"
#include "repositories/MedicineRepository.h"
#include "repositories/CategoryRepository.h"
#include "repositories/InvoiceRepository.h"
#include <string>
#include <vector>

class UIManager {
private:
    PharmacyService* service;
    MedicineRepository* medRepo;
    CategoryRepository* catRepo;
    InvoiceRepository* invRepo;

    // Buffers for UI Actions
    char medNameBuf[128] = "";
    double medPrice = 0.0;
    int medQuantity = 0;
    char medExpiryBuf[11] = ""; // YYYY-MM-DD format
    int medCategoryId = 0;
    int selectedMedicineIndex = -1;
    int selectedCategoryIndex = 0;
    char searchBuf[128] = "";

    char catNameBuf[128] = "";
    double totalAmount = 0.0;

    // Sales Buffers
    int posSelectedMedicineId = 0;
    int posQuantity = 0;
    std::string posMessage = "";
    char customerNameBuf[128] = "";

    struct CartItem {
    int medicineId;
    std::string name;
    double price;
    int quantity;
};
    std::vector<CartItem> cart;

    // History tab
    int revenueViewMode = 0; // 0=day, 1=month, 2=quarter

    // Modular Rendering Helpers
    void RenderDashboard();
    void RenderInventory();
    void RenderCategories();
    void RenderPOS();
    void RenderExpiryWarnings();
    void RenderHistory();

public:
    UIManager(PharmacyService* svc, MedicineRepository* mRepo, CategoryRepository* cRepo, InvoiceRepository* iRepo)
        : service(svc), medRepo(mRepo), catRepo(cRepo), invRepo(iRepo) {}

    // Hooked continuously matching graphic frame cycles
    void Render();
};
