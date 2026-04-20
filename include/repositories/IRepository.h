#pragma once
#include <vector>

// IRepository utilizes Type template and Virtual Functions 
// to satisfy Polymorphism & Abstraction requirements.
template <typename T>
class IRepository {
public:
    virtual ~IRepository() = default;

    virtual bool Insert(T& item) = 0;         
    virtual bool Update(const T& item) = 0;
    virtual bool Delete(int id) = 0;
    virtual T GetById(int id) = 0;
    virtual std::vector<T> GetAll() = 0;
};
