#pragma once // Ensures the file is included only once during compilation.

#include <sstream>
#include <iostream>
#include "IBaseORM.h"

// Constants for the Account ORM
constexpr int ACCOUNT_NUMBER_ELEMENTS = 2;
constexpr const std::string_view ACCOUNT_TABLE_NAME = "Account";

// AccountORM class: Represents the Object-Relational Mapping (ORM) for the "Account" table.
class AccountORM : public IBaseORM
{
    int id;    // Unique identifier for the account.
    int limit; // Account limit (e.g., credit or transaction limit).

public:
    // Getter and setter for the account ID
    int getId() const { return id; }
    void setId(int accountId) { id = accountId; }

    // Getter and setter for the account limit
    int getLimit() const { return limit; }
    void setLimit(int accountLimit) { limit = accountLimit; }

    // Constructor: Initializes the object and sets up the setter functions for ORM mapping.
    AccountORM() : id{0}, limit{0}, IBaseORM(std::string(ACCOUNT_TABLE_NAME))
    {
        // Mapping database columns to member variables using setter functions
        setters["account_id"] = [this](const std::string &value)
        { this->setId(std::stoi(value)); };

        setters["account_limit"] = [this](const std::string &value)
        { this->setLimit(std::stoi(value)); };
    };

    // Overrides the save method to generate an SQL insert statement.
    std::string const save() override
    {
        std::stringstream sql;
        sql << "INSERT INTO Account (account_limit) VALUES (" << limit << ");";
        return sql.str();
    }

    // Overrides the load method to generate an SQL select statement.
    std::string load() override
    {
        std::stringstream sql;
        sql << "SELECT * FROM Account WHERE account_id = " << id << ";";
        return sql.str();
    }
};
