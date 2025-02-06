#pragma once // Ensures the file is included only once during compilation.

#include <unordered_map>
#include "Utils.h"
#include <string>
#include <functional>

// Base class for Object-Relational Mapping (ORM).
// Provides a common interface for managing database entities.
class IBaseORM
{
    std::string _tableName; // Stores the name of the corresponding SQL table.

protected:
    // A map of setter functions for dynamically assigning values to object attributes.
    std::unordered_map<std::string, std::function<void(const std::string &)>> setters;

public:
    // Default constructor initializing an empty table name.
    IBaseORM() : _tableName("") {}

    // Constructor to initialize with a specific table name.
    explicit IBaseORM(std::string tableName) : _tableName(tableName) {}

    // Virtual destructor to ensure proper cleanup of derived classes.
    virtual ~IBaseORM() = default;

    // Returns the table name associated with the ORM object.
    std::string getTableName() const { return _tableName; }

    // Pure virtual function to generate an SQL statement for inserting or updating the object.
    virtual const std::string save() = 0;

    // Pure virtual function to generate an SQL query for retrieving an object by its ID.
    virtual std::string load() = 0;

    // Populates object attributes from an SQL query result.
    virtual void fromSQL(const std::string &colName, const std::string &colValue)
    {
        if (colValue == "NULL") // Ignore null values.
        {
            return;
        }

        // Find the corresponding setter function and apply the value.
        auto it = setters.find(colName);
        if (it != setters.end())
        {
            it->second(colValue); // Calls the appropriate setter function.
        }
    }
};
