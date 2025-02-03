#pragma once

#include <sstream>
#include <iostream>
#include "IBaseORM.h"

class AccountORM : public IBaseORM
{
    int id;
    int limit;

public:
    int getId() const { return id; }
    void setId(int accountId) { id = accountId; }

    int getLimit() const { return limit; }
    void setLimit(int accountLimit) { limit = accountLimit; }

    AccountORM() : id{0}, limit{0}
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

    // Implementación del método update()
    std::string update() override
    {
        std::stringstream sql;
        sql << "UPDATE Account SET account_limit = " << limit << " WHERE account_id = " << id << ";";
        return sql.str();
    }

    // Implementación del método remove()
    std::string remove() override
    {
        std::stringstream sql;
        sql << "DELETE FROM Account WHERE account_id = " << id << ";";
        return sql.str();
    }

    // Implementación del método load()
    std::string load() override
    {
        std::stringstream sql;
        sql << "SELECT * FROM Account WHERE account_id = " << id << ";";
        return sql.str();
    }

    // Método fromSQL() para parsear los resultados de la consulta
    void fromSQL(const std::string &colName, const std::string &colValue) override
    {
        if (colValue == "NULL")
        {
            return;
        }
        // Buscar el setter correspondiente y aplicar el valor
        auto it = setters.find(colName);
        if (it != setters.end())
        {
            it->second(colValue); // Llama al setter correspondiente
        }
    }
};