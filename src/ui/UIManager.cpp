#include "ui/UIManager.h"
#include <iostream>

// --- Date format helpers ---
// DB stores YYYY-MM-DD internally, UI shows DD-MM-YYYY
static std::string ToDisplay(const std::string& iso) {
    if (iso.length() >= 10)
        return iso.substr(8,2) + "-" + iso.substr(5,2) + "-" + iso.substr(0,4);
    return iso;
}
static std::string ToISO(const std::string& disp) {
    if (disp.length() >= 10)
        return disp.substr(6,4) + "-" + disp.substr(3,2) + "-" + disp.substr(0,2);
    return disp;
}

void UIManager::Render() {
    ImGuiIO& io = ImGui::GetIO();

    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(io.DisplaySize);

    ImGui::Begin("He Thong Quan Ly Hieu Thuoc", nullptr,
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_MenuBar);

    if (ImGui::BeginTabBar("MainTabs", ImGuiTabBarFlags_Reorderable)) {

        if (ImGui::BeginTabItem("Tong Quan & Canh Bao")) {
            RenderDashboard();
            ImGui::Separator();
            RenderExpiryWarnings();
            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Quan Ly Thuoc")) {
            RenderInventory();
            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Danh Muc")) {
            RenderCategories();
            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Ban Hang")) {
            RenderPOS();
            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Lich Su & Doanh Thu")) {
            RenderHistory();
            ImGui::EndTabItem();
        }

        ImGui::EndTabBar();
    }

    ImGui::End();
}

void UIManager::RenderDashboard() {
    auto meds = medRepo->GetAll();
    auto cats = catRepo->GetAll();

    ImGui::Text("THONG TIN HE THONG");
    ImGui::Separator();

    ImGui::Columns(2, nullptr, false);

    ImGui::Text("Tong so thuoc:");
    ImGui::NextColumn();
    ImGui::Text("%d", (int)meds.size());
    ImGui::NextColumn();

    ImGui::Text("Tong so danh muc:");
    ImGui::NextColumn();
    ImGui::Text("%d", (int)cats.size());

    ImGui::Columns(1);
}

void UIManager::RenderExpiryWarnings() {
    ImGui::Text("CANH BAO HAN SU DUNG");
    ImGui::Separator();

    auto meds = medRepo->GetAll();
    bool found = false;

    for (const auto& m : meds) {
        if (service->isExpired(m.GetExpiryDate())) {
            ImGui::TextColored(ImVec4(1, 0, 0, 1),
                "Het han: %s | %s",
                m.GetName().c_str(),
                ToDisplay(m.GetExpiryDate()).c_str());
            found = true;
        }
        else if (service->isNearExpiry(m.GetExpiryDate(), 30)) {
            ImGui::TextColored(ImVec4(1, 0.5f, 0, 1),
                "Sap het han: %s | %s | SL: %d",
                m.GetName().c_str(),
                ToDisplay(m.GetExpiryDate()).c_str(),
                m.GetQuantity());
            found = true;
        }
    }

    if (!found) {
        ImGui::Text("Khong co thuoc canh bao.");
    }
}

void UIManager::RenderInventory() {
    ImGui::Text("THEM THUOC MOI");
    ImGui::Separator();

    ImGui::PushItemWidth(250);

    ImGui::InputText("Ten thuoc", medNameBuf, IM_ARRAYSIZE(medNameBuf));
    ImGui::InputDouble("Gia", &medPrice);
    ImGui::InputInt("So luong", &medQuantity);
    ImGui::InputText("Han su dung (DD-MM-YYYY)", medExpiryBuf, IM_ARRAYSIZE(medExpiryBuf));
    ImGui::InputInt("Ma danh muc", &medCategoryId);

    if (ImGui::Button("Them thuoc", ImVec2(150, 30))) {
        std::string isoExpiry = ToISO(std::string(medExpiryBuf));
        Medicine m(0, medNameBuf, medPrice, medQuantity, isoExpiry, medCategoryId);
        medRepo->Insert(m);

        medNameBuf[0] = '\0';
        medExpiryBuf[0] = '\0';
    }

    ImGui::PopItemWidth();

    ImGui::Separator();
    ImGui::Text("DANH SACH THUOC");

    auto meds = medRepo->GetAll();

    if (ImGui::BeginTable("med_table", 6,
        ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_Resizable)) {

        ImGui::TableSetupColumn("ID", ImGuiTableColumnFlags_WidthFixed, 50);
        ImGui::TableSetupColumn("Ten");
        ImGui::TableSetupColumn("Gia");
        ImGui::TableSetupColumn("So luong");
        ImGui::TableSetupColumn("Han su dung");
        ImGui::TableSetupColumn("Xoa");

        ImGui::TableHeadersRow();

        for (const auto& m : meds) {
            ImGui::TableNextRow();

            ImGui::TableNextColumn(); ImGui::Text("%d", m.GetId());
            ImGui::TableNextColumn(); ImGui::Text("%s", m.GetName().c_str());
            ImGui::TableNextColumn(); ImGui::Text("%.2f", m.GetPrice());
            ImGui::TableNextColumn(); ImGui::Text("%d", m.GetQuantity());
            ImGui::TableNextColumn(); ImGui::Text("%s", ToDisplay(m.GetExpiryDate()).c_str());

            ImGui::TableNextColumn();
            ImGui::PushID(m.GetId());
            if (ImGui::Button("Xoa")) {
                medRepo->Delete(m.GetId());
            }
            ImGui::PopID();
        }

        ImGui::EndTable();
    }
}

void UIManager::RenderCategories() {
    ImGui::Text("THEM DANH MUC");
    ImGui::Separator();

    ImGui::InputText("Ten danh muc", catNameBuf, IM_ARRAYSIZE(catNameBuf));

    if (ImGui::Button("Them danh muc", ImVec2(150, 30))) {
        Category c(0, catNameBuf);
        catRepo->Insert(c);
        catNameBuf[0] = '\0';
    }

    ImGui::Separator();
    ImGui::Text("DANH SACH DANH MUC");

    auto cats = catRepo->GetAll();

    for (const auto& c : cats) {
        ImGui::BulletText("[%d] %s", c.GetId(), c.GetName().c_str());
    }
}

void UIManager::RenderPOS() {
    ImGui::Text("BAN THUOC");
    ImGui::Separator();

    auto meds = medRepo->GetAll();

    ImGui::InputText("Ten khach hang", customerNameBuf, IM_ARRAYSIZE(customerNameBuf));

    // =========================
    // CHỌN THUỐC
    // =========================
    if (ImGui::BeginCombo("Chon thuoc",
        selectedMedicineIndex >= 0 && selectedMedicineIndex < meds.size()
            ? (meds[selectedMedicineIndex].GetName().empty() ? "(Khong ten)" : meds[selectedMedicineIndex].GetName().c_str())
            : "Chon thuoc")) {

        for (int i = 0; i < meds.size(); i++) {
            const auto& m = meds[i];

            bool isExpired = service->isExpired(m.GetExpiryDate());

            if (isExpired) ImGui::BeginDisabled();

            std::string label = m.GetName().empty() ? "(Khong ten)" : m.GetName();
            label += "##" + std::to_string(m.GetId());

            if (ImGui::Selectable(label.c_str(), selectedMedicineIndex == i)) {
                selectedMedicineIndex = i;
            }

            if (isExpired) ImGui::EndDisabled();
        }

        ImGui::EndCombo();
    }

    ImGui::InputInt("So luong", &posQuantity);

    // =========================
    // ADD TO CART
    // =========================
    if (ImGui::Button("Them vao toa", ImVec2(150, 30))) {
        if (selectedMedicineIndex >= 0 && selectedMedicineIndex < (int)meds.size()) {
            auto& m = meds[selectedMedicineIndex];

            if (service->isExpired(m.GetExpiryDate())) {
                posMessage = "Thuoc da het han!";
            } else {
                // Tinh so luong da co trong gio hang
                int alreadyInCart = 0;
                for (const auto& item : cart) {
                    if (item.medicineId == m.GetId()) {
                        alreadyInCart = item.quantity;
                        break;
                    }
                }

                // Kiem tra ton kho
                if (alreadyInCart + posQuantity > m.GetQuantity()) {
                    posMessage = "Khong du hang trong kho! Ton kho: " + std::to_string(m.GetQuantity()) + ", trong gio: " + std::to_string(alreadyInCart);
                } else {
                    bool found = false;
                    for (auto& item : cart) {
                        if (item.medicineId == m.GetId()) {
                            item.quantity += posQuantity;
                            found = true;
                            break;
                        }
                    }
                    if (!found) {
                        cart.push_back({ m.GetId(), m.GetName(), m.GetPrice(), posQuantity });
                    }
                    posMessage = "";
                }
            }
        }
    }

    ImGui::Separator();

    // =========================
    // HIỂN THỊ BILL
    // =========================
    ImGui::Text("HOA DON");

    totalAmount = 0.0;

    if (ImGui::BeginTable("bill_table", 4,
        ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg)) {

        ImGui::TableSetupColumn("Ten");
        ImGui::TableSetupColumn("Gia");
        ImGui::TableSetupColumn("SL");
        ImGui::TableSetupColumn("Thanh tien");
        ImGui::TableHeadersRow();

        for (auto& item : cart) {
            ImGui::TableNextRow();

            ImGui::TableNextColumn(); ImGui::Text("%s", item.name.c_str());
            ImGui::TableNextColumn(); ImGui::Text("%.2f", item.price);
            ImGui::TableNextColumn(); ImGui::Text("%d", item.quantity);

            double subtotal = item.price * item.quantity;
            totalAmount += subtotal;

            ImGui::TableNextColumn(); ImGui::Text("%.2f", subtotal);
        }

        ImGui::EndTable();
    }

    ImGui::Separator();

    ImGui::Text("Tong tien: %.2f", totalAmount);

    // =========================
    // THANH TOAN
    // =========================
    if (ImGui::Button("Thanh toan", ImVec2(200, 40))) {
        if (cart.empty()) {
            posMessage = "Chua co thuoc trong toa.";
        } else {
            bool success = true;

            for (auto& item : cart) {
                std::string msg;
                service->sellMedicine(item.medicineId, item.quantity, std::string(customerNameBuf), msg);

                if (msg.find("fail") != std::string::npos) {
                    success = false;
                    posMessage = msg;
                    break;
                }
            }

            if (success) {
                posMessage = "Thanh toan thanh cong!";
                cart.clear();
                totalAmount = 0.0;
            }
        }
    }

    ImGui::Separator();

    if (!posMessage.empty()) {
        ImGui::TextWrapped("%s", posMessage.c_str());
    }
}

void UIManager::RenderHistory() {
    // === INVOICE HISTORY with inline detail ===
    ImGui::Text("LICH SU GIAO DICH");
    ImGui::Separator();

    auto invoices = invRepo->GetAll();

    for (const auto& inv : invoices) {
        // Header line for each invoice as a collapsible TreeNode
        std::string header = "HD #" + std::to_string(inv.GetId())
            + "  |  " + ToDisplay(inv.GetDate())
            + "  |  " + inv.GetCustomerName()
            + "  |  Tong: " + std::to_string((int)inv.GetTotal());

        ImGui::PushID(inv.GetId());
        if (ImGui::TreeNode(header.c_str())) {
            auto items = invRepo->GetItemsForInvoice(inv.GetId());
            if (ImGui::BeginTable("inv_detail", 3, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg)) {
                ImGui::TableSetupColumn("Ten thuoc");
                ImGui::TableSetupColumn("So luong");
                ImGui::TableSetupColumn("Don gia");
                ImGui::TableHeadersRow();

                for (const auto& it : items) {
                    Medicine med = medRepo->GetById(it.GetMedicineId());
                    ImGui::TableNextRow();
                    ImGui::TableNextColumn(); ImGui::Text("%s", med.GetName().c_str());
                    ImGui::TableNextColumn(); ImGui::Text("%d", it.GetQuantity());
                    ImGui::TableNextColumn(); ImGui::Text("%.2f", it.GetPrice());
                }
                ImGui::EndTable();
            }
            ImGui::TreePop();
        }
        ImGui::PopID();
    }

    ImGui::Separator();

    // === REVENUE STATISTICS ===
    ImGui::Text("THONG KE DOANH THU");
    ImGui::Separator();

    ImGui::RadioButton("Theo ngay", &revenueViewMode, 0); ImGui::SameLine();
    ImGui::RadioButton("Theo thang", &revenueViewMode, 1); ImGui::SameLine();
    ImGui::RadioButton("Theo quy", &revenueViewMode, 2);

    std::vector<std::pair<std::string, double>> stats;
    if (revenueViewMode == 0) stats = invRepo->GetRevenueByDay();
    else if (revenueViewMode == 1) stats = invRepo->GetRevenueByMonth();
    else stats = invRepo->GetRevenueByQuarter();

    if (ImGui::BeginTable("revenue_table", 2,
        ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg)) {

        ImGui::TableSetupColumn("Thoi gian");
        ImGui::TableSetupColumn("Doanh thu");
        ImGui::TableHeadersRow();

        double grandTotal = 0.0;
        for (const auto& s : stats) {
            ImGui::TableNextRow();
            ImGui::TableNextColumn(); ImGui::Text("%s", ToDisplay(s.first).c_str());
            ImGui::TableNextColumn(); ImGui::Text("%.2f", s.second);
            grandTotal += s.second;
        }

        // Total row
        ImGui::TableNextRow();
        ImGui::TableNextColumn(); ImGui::TextColored(ImVec4(0, 1, 1, 1), "TONG CONG");
        ImGui::TableNextColumn(); ImGui::TextColored(ImVec4(0, 1, 1, 1), "%.2f", grandTotal);

        ImGui::EndTable();
    }
}