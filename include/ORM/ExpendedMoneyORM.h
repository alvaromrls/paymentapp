#pragma once

#include <sstream>
#include <iostream>
#include <string>

#include "Utils.h"
#include "CardORM.h"
#include "HistoryORM.h"

const std::string TOTAL_DEPOSIT__AMOUNT = "amount";

// This class acts as an ORM for getting al the money in the history given a credit card. It can only read data.
class ExpendedMoneyORM : public IBaseORM
{
    int account_id;
    int _amount;
    void setAmount(int amount) { _amount = amount; } // internal setter

public:
    // Constructor: needs a CardORM object. Other constructors may be useful in different cases.
    explicit ExpendedMoneyORM(CardORM userCard) : account_id(userCard.getAccountId())
    {
        setters[TOTAL_DEPOSIT__AMOUNT] = [this](const std::string &value)
        { this->setAmount(std::stoi(value)); };
    }

    // Get the amount
    int getAmount() { return _amount; }

    // Write the query
    std::string load() override
    {
        std::stringstream sql;
        sql << "SELECT SUM(t." << TOTAL_DEPOSIT__AMOUNT << ") AS amount "
            << "FROM " << HISTORY_TABLE_NAME << " ah "
            << "JOIN " << TRANSTACTION_TABLE_NAME << " t ON ah.transaction_id = t.transaction_id "
            << "WHERE ah.account_id = " << account_id << ";";
        return sql.str();
    }

    // Not needed methods
    const std::string save() override { return {}; }
};