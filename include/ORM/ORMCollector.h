#pragma once

#include <vector>
#include <iostream>
#include <string>
#include "IBaseORM.h"

// Collector class for managing ORM objects derived from IBaseORM
template <typename T>
class ORMCollector : public IBaseORM
{
private:
    std::vector<T> vector; // Stores objects of type T
    T data;                // Temporary storage for parsing SQL data
    int count;             // Counter for tracking parsed elements
    int pushEvery;         // Number of elements before pushing an object

public:
    // Constructor allowing customization of push frequency
    ORMCollector(int pushEvery) : count(0), pushEvery(pushEvery), IBaseORM("") {}

    // Default constructor uses `T::getElements()` as push frequency
    ORMCollector() : count(0), pushEvery(T::getElements()) {}

    virtual ~ORMCollector() = default;

    // Adds an ORM object to the collection
    void add(const T &orm)
    {
        vector.push_back(orm);
    }

    // Processes SQL data and adds objects to the collection when complete
    void fromSQL(const std::string &colName, const std::string &colValue) override
    {
        data.fromSQL(colName, colValue); // Set attribute in temporary object
        count++;

        // Push object into the collection once all elements are set
        if (count >= pushEvery)
        {
            add(data);
            count = 0;  // Reset counter
            data = T(); // Reset temporary data object for the next entry
        }
    }

    // Returns the number of stored ORM objects
    size_t size() const
    {
        return vector.size();
    }

    // Returns the collection of ORM objects
    const std::vector<T> &getCollection() const
    {
        return vector;
    }

    // No-op save method, as ORMCollector doesn't persist directly
    const std::string save() override { return {}; }

    // Generates SQL query to load all objects of type T from the database
    std::string load() override
    {
        return selectAllData(data.getTableName());
    }
};
