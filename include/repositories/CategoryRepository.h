#pragma once
#include "models/Category.h"
#include "IRepository.h"
#include <sqlite3.h>
#include <vector>

class CategoryRepository : public IRepository<Category> {
private:
    sqlite3* db;

public:
    explicit CategoryRepository(sqlite3* database) : db(database) {}

    bool Insert(Category& item) override;
    bool Update(const Category& item) override;
    bool Delete(int id) override;
    Category GetById(int id) override;
    std::vector<Category> GetAll() override;
};
