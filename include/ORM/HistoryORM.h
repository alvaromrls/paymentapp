#pragma once

#include <sstream>
#include <iostream>
#include <string>
#include "IBaseORM.h"
#include "AccountORM.h"
#include "TransactionORM.h"

constexpr std::string_view HISTORY_TABLE_NAME = "AccountHistory";
constexpr int HISTORY_NUMBER_ELEMENTS = 2;

class HistoryORM : public IBaseORM
{
    int accountId;
    int transactionId;

    void _setAccountId(int id) { accountId = id; }         // Internal use
    void _setTransactionId(int id) { transactionId = id; } // Internal use

public:
    HistoryORM() : accountId{0}, transactionId{0}, IBaseORM(std::string(HISTORY_TABLE_NAME))
    {
        setters["account_id"] = [this](const std::string &value)
        { this->_setAccountId(std::stoi(value)); };
        setters["transaction_id"] = [this](const std::string &value)
        { this->_setTransactionId(std::stoi(value)); };
    }

    void setAccountId(AccountORM &account) { accountId = account.getId(); }
    int getAccountId() const { return accountId; }

    void setTransactionId(TransactionORM &transaction) { transactionId = transaction.getTransactionId(); }
    int getTransactionId() const { return transactionId; }

    const std::string save() override
    {
        std::stringstream sql;
        sql << "INSERT INTO AccountHistory (account_id, transaction_id) VALUES ("
            << accountId << ", " << transactionId << ");";
        return sql.str();
    }

    std::string load() override
    {
        std::stringstream sql;
        sql << "SELECT * FROM AccountHistory WHERE account_id = " << accountId;
        return sql.str();
    }

    static const std::string loadAll()
    {
        return selectAllData(std::string(HISTORY_TABLE_NAME));
    }

    static const int getElements()
    {
        return HISTORY_NUMBER_ELEMENTS;
    }
};
