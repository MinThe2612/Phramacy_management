#pragma once
#include <string>

class Medicine {
private:
    int id;
    std::string name;
    double price;
    int quantity;
    std::string expiryDate;
    int categoryId;

public:
    Medicine() : id(0), price(0.0), quantity(0), categoryId(0) {}
    Medicine(int id, std::string n, double p, int q, std::string exp, int catId) 
        : id(id), name(std::move(n)), price(p), quantity(q), expiryDate(std::move(exp)), categoryId(catId) {}

    int GetId() const { return id; }
    void SetId(int v) { id = v; }

    const std::string& GetName() const { return name; }
    void SetName(const std::string& v) { name = v; }

    double GetPrice() const { return price; }
    void SetPrice(double v) { price = v; }

    int GetQuantity() const { return quantity; }
    void SetQuantity(int v) { quantity = v; }

    const std::string& GetExpiryDate() const { return expiryDate; }
    void SetExpiryDate(const std::string& v) { expiryDate = v; }

    int GetCategoryId() const { return categoryId; }
    void SetCategoryId(int v) { categoryId = v; }
};
