#pragma once

#include <sstream>
#include <iostream>
#include "IBaseORM.h"

constexpr int ACCOUNT_NUMBER_ELEMENTS = 2;
constexpr const std::string_view ACCOUNT_TABLE_NAME = "Account";

class AccountORM : public IBaseORM
{
    int id;
    int limit;

public:
    int getId() const { return id; }
    void setId(int accountId) { id = accountId; }

    int getLimit() const { return limit; }
    void setLimit(int accountLimit) { limit = accountLimit; }

    AccountORM() : id{0}, limit{0}, IBaseORM(std::string(ACCOUNT_TABLE_NAME))
    {
        setters["account_id"] = [this](const std::string &value)
        { this->setId(std::stoi(value)); };
        setters["account_limit"] = [this](const std::string &value)
        { this->setLimit(std::stoi(value)); };
    };

    // Implementación del método save()
    std::string const save() override
    {
        std::stringstream sql;
        sql << "INSERT INTO Account (account_limit) VALUES (" << limit << ");";
        return sql.str();
    }

    // Implementación del método load()
    std::string load() override
    {
        std::stringstream sql;
        sql << "SELECT * FROM Account WHERE account_id = " << id << ";";
        return sql.str();
    }
};