#pragma once // Ensures the file is included only once during compilation.

#include <sstream>
#include <iostream>
#include "IBaseORM.h"

// Constants defining the Card Issuers table structure.
constexpr int CARD_ISSUERS_NUMBER_ELEMENTS = 3;
constexpr const std::string_view CARD_ISSUERS_TABLE_NAME = "CardIssuers";

// CardIssuersORM class: Represents the Object-Relational Mapping (ORM) for the "CardIssuers" table.
class CardIssuersORM : public IBaseORM
{
    int cardType;       // Type of card associated with the issuer.
    std::string issuer; // Name of the issuing company.
    int issuerFee;      // Fee associated with the issuer.

public:
    // Constructor: Initializes object attributes and ORM field mappings.
    CardIssuersORM() : cardType{0}, issuer{}, issuerFee{0},
                       IBaseORM(std::string(CARD_ISSUERS_TABLE_NAME))
    {
        // Mapping database columns to member variables using setter functions.
        setters["card_type"] = [this](const std::string &value)
        { this->setCardType(std::stoi(value)); };
        setters["issuer"] = [this](const std::string &value)
        { this->setIssuer(value); };
        setters["issuer_fee"] = [this](const std::string &value)
        { this->setIssuerFee(std::stoi(value)); };
    }

    // Getter and setter methods for cardType.
    void setCardType(int type) { cardType = type; }
    int getCardType() const { return cardType; }

    // Getter and setter methods for issuer.
    void setIssuer(const std::string &issuerValue) { issuer = issuerValue; }
    std::string getIssuer() const { return issuer; }

    // Getter and setter methods for issuerFee.
    void setIssuerFee(int fee) { issuerFee = fee; }
    int getIssuerFee() const { return issuerFee; }

    // Overrides the save method to generate an SQL insert statement.
    const std::string save() override
    {
        std::stringstream sql;
        sql << "INSERT INTO CardIssuers (issuer, issuer_fee) VALUES ('"
            << issuer << "', " << issuerFee << ");";
        return sql.str();
    }

    // Overrides the load method to generate an SQL select statement.
    std::string load() override
    {
        return selectWithId(IBaseORM::getTableName(), "card_type", cardType);
    }

    // Static method to generate an SQL query to load all records from CardIssuers.
    static const std::string loadAll()
    {
        return selectAllData("CardIssuers");
    }

    // Static method to return the number of elements in the table schema.
    static const int getElements()
    {
        return CARD_ISSUERS_NUMBER_ELEMENTS;
    }
};
