#pragma once
#include "models/Medicine.h"
#include "IRepository.h"
#include <sqlite3.h>
#include <vector>

class MedicineRepository : public IRepository<Medicine> {
private:
    sqlite3* db;

public:
    explicit MedicineRepository(sqlite3* database) : db(database) {}

    bool Insert(Medicine& item) override;
    bool Update(const Medicine& item) override;
    bool Delete(int id) override;
    Medicine GetById(int id) override;
    std::vector<Medicine> GetAll() override;
};
