#pragma once

#include <sstream>
#include <iostream>
#include <string>

#include "Utils.h"
#include "CardORM.h"

class TotalDepositORM : public IBaseORM
{
    CardORM userCard;
    int _amount;
    void setAmount(int amount) { _amount = amount; }

public:
    explicit TotalDepositORM(CardORM userCard) : userCard(userCard)
    {
        setters["amount"] = [this](const std::string &value)
        { this->setAmount(std::stoi(value)); };
    }

    int getAmount() { return _amount; }

    std::string load() override
    {
        std::stringstream sql;
        sql << "SELECT SUM(t.amount) AS amount "
            << "FROM AccountHistory ah "
            << "JOIN TransactionTable t ON ah.transaction_id = t.transaction_id "
            << "WHERE ah.account_id = " << userCard.getAccountId() << ";";
        return sql.str();
    }

    // Not needed methods
    const std::string save() override { return {}; }
};