#pragma once

#include <sstream>
#include <iostream>
#include <string>
#include "IBaseORM.h"
#include "CardORM.h"
#include "MerchantORM.h"

constexpr const std::string_view TRANSTACTION_TABLE_NAME = "TransactionTable";
constexpr int TRANSACTION_NUMBER_ELEMENTS = 5;

class TransactionORM : public IBaseORM
{
    int transactionId;
    std::string cardId;
    int merchantId;
    std::string transactionTime;
    int amount;

    void _setMerchantId(int id) { merchantId = id; } // Internal use
    void _setCardId(const std::string &id) { cardId = id; }

public:
    TransactionORM() : transactionId{0}, cardId{}, merchantId{0}, transactionTime{}, amount{0}, IBaseORM(std::string(TRANSTACTION_TABLE_NAME))
    {
        setters["transaction_id"] = [this](const std::string &value)
        { this->setTransactionId(std::stoi(value)); };
        setters["card_id"] = [this](const std::string &value)
        { this->_setCardId(value); };
        setters["merchant_id"] = [this](const std::string &value)
        { this->_setMerchantId(std::stoi(value)); };
        setters["transaction_time"] = [this](const std::string &value)
        { this->setTransactionTime(value); };
        setters["amount"] = [this](const std::string &value)
        { this->setAmount(std::stoi(value)); };
    }

    void setTransactionId(int id) { transactionId = id; } // Internal use
    int getTransactionId() const { return transactionId; }

    void setCardId(CardORM &card) { cardId = card.getCardId(); }
    std::string getCardId() const { return cardId; }

    void setMerchantId(MerchantORM &merchant) { merchantId = merchant.getMerchantId(); }
    int getMerchantId() const { return merchantId; }

    void setTransactionTime(const std::string &time) { transactionTime = time; }
    std::string getTransactionTime() const { return transactionTime; }

    void setAmount(int amt) { amount = amt; }
    int getAmount() const { return amount; }

    const std::string save() override
    {
        std::stringstream sql;
        sql << "INSERT INTO TransactionTable (card_id, merchant_id, transaction_time, amount) VALUES ('"
            << cardId << "', " << merchantId << ", '" << transactionTime << "', " << amount << ");";
        return sql.str();
    }

    std::string load() override
    {
        std::stringstream sql;
        sql << "SELECT * FROM TransactionTable WHERE transaction_id = " << transactionId << ";";
        return sql.str();
    }

    static const std::string loadAll()
    {
        return selectAllData(std::string(TRANSTACTION_TABLE_NAME));
    }

    static const int getElements()
    {
        return TRANSACTION_NUMBER_ELEMENTS;
    }
};
