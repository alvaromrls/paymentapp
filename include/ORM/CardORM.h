#pragma once // Ensures the file is included only once during compilation.

#include <sstream>
#include <iostream>
#include <string>

#include "Utils.h"
#include "AccountORM.h"
#include "CardIssuersORM.h"
#include "IBaseORM.h"

// Constants defining the Card table structure.
constexpr int CARD_NUMBER_ELEMENTS = 6;
constexpr std::string_view CARD_TABLE_NAME = "Card";

// CardORM class: Represents the Object-Relational Mapping (ORM) for the "Card" table.
class CardORM : public IBaseORM
{
    std::string cardId;         // Unique card identifier (UUID stored as a string).
    std::string cardNumber;     // Card number stored as a string.
    int accountId;              // Foreign key referencing an account.
    std::string cardholderName; // Name of the cardholder.
    std::string expirationDate; // Expiration date of the card.
    int cardType;               // Type of card (linked to CardIssuers).

    // Internal setter methods (used for ORM mapping only).
    void _setAccountId(int id) { accountId = id; }
    void _setCardType(int type) { cardType = type; }

public:
    // Constructor: Initializes object attributes and ORM field mappings.
    CardORM() : cardId{}, cardNumber{},
                accountId{0}, cardholderName{},
                expirationDate{}, cardType{0}, IBaseORM(std::string(CARD_TABLE_NAME))
    {
        // Mapping database columns to member variables using setter functions.
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

    // Getter and setter methods for cardId.
    void setCardId(const std::string &id) { cardId = id; }
    std::string getCardId() const { return cardId; }

    // Getter and setter methods for cardNumber.
    void setCardNumber(const std::string &number) { cardNumber = number; }
    std::string getCardNumber() const { return cardNumber; }

    // Getter and setter methods for accountId.
    void setAccountId(AccountORM &account) { accountId = account.getId(); }
    int getAccountId() const { return accountId; }

    // Getter and setter methods for cardholderName.
    void setCardholderName(const std::string &name) { cardholderName = name; }
    std::string getCardholderName() const { return cardholderName; }

    // Getter and setter methods for expirationDate.
    void setExpirationDate(const std::string &date) { expirationDate = date; }
    std::string getExpirationDate() const { return expirationDate; }

    // Getter and setter methods for cardType.
    void setCardType(CardIssuersORM &type) { cardType = type.getCardType(); }
    int getCardType() const { return cardType; }

    // Overrides the save method to generate an SQL insert statement.
    const std::string save() override
    {
        setCardId(generateSimpleUUID()); // Generate a UUID before saving.
        std::stringstream sql;
        sql << "INSERT INTO " << CARD_TABLE_NAME << " (card_id, card_number, account_id, cardholder_name, expiration_date, card_type) VALUES ('"
            << cardId << "', '" << cardNumber << "', " << accountId << ", '"
            << cardholderName << "', '" << expirationDate << "', " << cardType << ");";
        return sql.str();
    }

    // Overrides the load method to generate an SQL select statement.
    std::string load() override
    {
        std::stringstream sql;
        sql << "SELECT * FROM " << CARD_TABLE_NAME << " WHERE card_id = '" << cardId << "';";
        return sql.str();
    }

    // Static method to generate an SQL query to load all records from Card.
    static const std::string loadAll()
    {
        return selectAllData(std::string(CARD_TABLE_NAME));
    }

    // Static method to return the number of elements in the table schema.
    static const int getElements()
    {
        return CARD_NUMBER_ELEMENTS;
    }

    // Generates an SQL query to find a card by its number.
    const std::string findByCardNumber()
    {
        return selectWithId(IBaseORM::getTableName(), "card_number", cardNumber);
    }
};
