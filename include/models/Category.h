#pragma once
#include <string>

class Category {
private:
    int id;
    std::string name;

public:
    Category() : id(0) {}
    Category(int id, std::string n) : id(id), name(std::move(n)) {}

    int GetId() const { return id; }
    void SetId(int v) { id = v; }

    const std::string& GetName() const { return name; }
    void SetName(const std::string& v) { name = v; }
};
