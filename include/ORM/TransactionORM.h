#pragma once

#include <sstream>
#include <iostream>
#include <string>
#include "IBaseORM.h"
#include "CardORM.h"
#include "MerchantORM.h"

// Constants for table name and number of expected elements per row
constexpr const std::string_view TRANSACTION_TABLE_NAME = "TransactionTable";
constexpr int TRANSACTION_NUMBER_ELEMENTS = 5;

// ORM class representing transactions in the database
class TransactionORM : public IBaseORM
{
    int transactionId;           // Unique transaction identifier
    std::string cardId;          // ID of the card used in the transaction
    int merchantId;              // ID of the merchant involved in the transaction
    std::string transactionTime; // Timestamp of the transaction
    int amount;                  // Transaction amount

    // Internal setters used in mapping database fields
    void _setMerchantId(int id) { merchantId = id; }
    void _setCardId(const std::string &id) { cardId = id; }

public:
    // Constructor initializes fields and maps column names to setter functions
    TransactionORM()
        : transactionId{0}, cardId{}, merchantId{0}, transactionTime{}, amount{0},
          IBaseORM(std::string(TRANSACTION_TABLE_NAME))
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

    // Getters and setters for transaction attributes
    void setTransactionId(int id) { transactionId = id; }
    int getTransactionId() const { return transactionId; }

    void setCardId(CardORM &card) { cardId = card.getCardId(); }
    std::string getCardId() const { return cardId; }

    void setMerchantId(MerchantORM &merchant) { merchantId = merchant.getMerchantId(); }
    int getMerchantId() const { return merchantId; }

    void setTransactionTime(const std::string &time) { transactionTime = time; }
    std::string getTransactionTime() const { return transactionTime; }

    void setAmount(int amt) { amount = amt; }
    int getAmount() const { return amount; }

    // Generates an SQL query to insert a new transaction into the database
    const std::string save() override
    {
        std::stringstream sql;
        sql << "INSERT INTO TransactionTable (card_id, merchant_id, transaction_time, amount) VALUES ('"
            << cardId << "', " << merchantId << ", '" << transactionTime << "', " << amount << ");";
        return sql.str();
    }

    // Generates an SQL query to load transaction data based on transaction ID
    std::string load() override
    {
        std::stringstream sql;
        sql << "SELECT * FROM TransactionTable WHERE transaction_id = " << transactionId << ";";
        return sql.str();
    }

    // Static method to generate an SQL query to load all transactions
    static const std::string loadAll()
    {
        return selectAllData(std::string(TRANSACTION_TABLE_NAME));
    }

    // Returns the number of elements expected in a transaction row
    static const int getElements()
    {
        return TRANSACTION_NUMBER_ELEMENTS;
    }
};
