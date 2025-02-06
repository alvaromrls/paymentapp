#pragma once // Ensures the file is included only once during compilation.

#include <sstream>
#include <iostream>
#include <string>
#include "IBaseORM.h"
#include "AccountORM.h"
#include "TransactionORM.h"

// Constants defining the table name and the number of elements in the history table.
constexpr std::string_view HISTORY_TABLE_NAME = "AccountHistory";
constexpr int HISTORY_NUMBER_ELEMENTS = 2;

// HistoryORM class: Represents the Object-Relational Mapping (ORM) for the "AccountHistory" table.
class HistoryORM : public IBaseORM
{
    int accountId;     // Foreign key referencing an account.
    int transactionId; // Foreign key referencing a transaction.

    // Internal setter methods (used for ORM mapping only).
    void _setAccountId(int id) { accountId = id; }
    void _setTransactionId(int id) { transactionId = id; }

public:
    // Constructor: Initializes object attributes and ORM field mappings.
    HistoryORM() : accountId{0}, transactionId{0}, IBaseORM(std::string(HISTORY_TABLE_NAME))
    {
        // Mapping database columns to member variables using setter functions.
        setters["account_id"] = [this](const std::string &value)
        { this->_setAccountId(std::stoi(value)); };
        setters["transaction_id"] = [this](const std::string &value)
        { this->_setTransactionId(std::stoi(value)); };
    }

    // Getter and setter methods for accountId.
    void setAccountId(AccountORM &account) { accountId = account.getId(); }
    int getAccountId() const { return accountId; }

    // Getter and setter methods for transactionId.
    void setTransactionId(TransactionORM &transaction) { transactionId = transaction.getTransactionId(); }
    int getTransactionId() const { return transactionId; }

    // Overrides the save method to generate an SQL insert statement.
    const std::string save() override
    {
        std::stringstream sql;
        sql << "INSERT INTO AccountHistory (account_id, transaction_id) VALUES ("
            << accountId << ", " << transactionId << ");";
        return sql.str();
    }

    // Overrides the load method to generate an SQL select statement.
    std::string load() override
    {
        std::stringstream sql;
        sql << "SELECT * FROM AccountHistory WHERE account_id = " << accountId;
        return sql.str();
    }

    // Static method to generate an SQL query to load all records from AccountHistory.
    static const std::string loadAll()
    {
        return selectAllData(std::string(HISTORY_TABLE_NAME));
    }

    // Static method to return the number of elements in the table schema.
    static const int getElements()
    {
        return HISTORY_NUMBER_ELEMENTS;
    }
};
