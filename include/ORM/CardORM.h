#pragma once

#include <sstream>
#include <iostream>
#include <string>
#include "Utils.h"
#include "AccountORM.h"
#include "CardIssuersORM.h"
#include "IBaseORM.h"

constexpr int CARD_NUMBER_ELEMENTS = 6;

class CardORM : public IBaseORM
{
    std::string cardId; // UUID stored as a string
    std::string cardNumber;
    int accountId;
    std::string cardholderName;
    std::string expirationDate;
    int cardType;

    void _setAccountId(int id) { accountId = id; }   // internal use
    void _setCardType(int type) { cardType = type; } // internal use
public:
    CardORM() : cardId{}, cardNumber{},
                accountId{0}, cardholderName{},
                expirationDate{}, cardType{0}, IBaseORM("Card")
    {
        // Inicializamos los setters
        setters["card_id"] = [this](const std::string &value)
        { this->setCardId(value); };
        setters["card_number"] = [this](const std::string &value)
        { this->setCardNumber(value); };
        setters["account_id"] = [this](const std::string &value)
        { this->_setAccountId(std::stoi(value)); };
        setters["cardholder_name"] = [this](const std::string &value)
        { this->setCardholderName(value); };
        setters["expiration_date"] = [this](const std::string &value)
        { this->setExpirationDate(value); };
        setters["card_type"] = [this](const std::string &value)
        { this->_setCardType(std::stoi(value)); };
    }

    // Métodos getter y setter
    void setCardId(const std::string &id) { cardId = id; }
    std::string getCardId() const { return cardId; }

    void setCardNumber(const std::string &number) { cardNumber = number; }
    std::string getCardNumber() const { return cardNumber; }

    void setAccountId(AccountORM &account) { accountId = account.getId(); }
    int getAccountId() const { return accountId; }

    void setCardholderName(const std::string &name) { cardholderName = name; }
    std::string getCardholderName() const { return cardholderName; }

    void setExpirationDate(const std::string &date) { expirationDate = date; }
    std::string getExpirationDate() const { return expirationDate; }

    void setCardType(CardIssuersORM &type) { cardType = type.getCardType(); }
    int getCardType() const { return cardType; }

    // Implementación del método save()
    const std::string save() override
    {
        setCardId(generateSimpleUUID());
        std::stringstream sql;
        sql << "INSERT INTO Card (card_id, card_number, account_id, cardholder_name, expiration_date, card_type) VALUES ('"
            << cardId << "', '" << cardNumber << "', " << accountId << ", '"
            << cardholderName << "', '" << expirationDate << "', " << cardType << ");";
        return sql.str();
    }

    // Implementación del método load()
    std::string load() override
    {
        std::stringstream sql;
        sql << "SELECT * FROM Card WHERE card_id = '" << cardId << "';";
        return sql.str();
    }

    static const std::string loadAll()
    {
        return "SELECT * FROM Card";
    }

    static const int getElements()
    {
        return CARD_NUMBER_ELEMENTS;
    }

    // Método fromSQL() para parsear los resultados de la consulta
    void fromSQL(const std::string &colName, const std::string &colValue) override
    {
        if (colValue == "NULL")
        {
            return;
        }

        auto it = setters.find(colName);
        if (it != setters.end())
        {
            it->second(colValue); // Llama al setter correspondiente
        }
    }
};
