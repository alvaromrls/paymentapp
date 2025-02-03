#pragma once

#include <sstream>
#include <iostream>
#include "IBaseORM.h"

constexpr int CARD_ISSUERS_NUMBER_ELEMENTS = 3;

// Implementación de la clase CardIssuersORM
class CardIssuersORM : public IBaseORM
{
    int cardType;
    std::string issuer;
    int issuerFee;

public:
    CardIssuersORM() : cardType{0}, issuer{}, issuerFee{0}, IBaseORM("CardIssuers")
    {
        // Inicializamos los setters
        setters["card_type"] = [this](const std::string &value)
        { this->setCardType(std::stoi(value)); };
        setters["issuer"] = [this](const std::string &value)
        { this->setIssuer(value); };
        setters["issuer_fee"] = [this](const std::string &value)
        { this->setIssuerFee(std::stoi(value)); };
    }

    // Métodos getter y setter
    void setCardType(int type) { cardType = type; }
    int getCardType() const { return cardType; }

    void setIssuer(const std::string &issuerValue) { issuer = issuerValue; }
    std::string getIssuer() const { return issuer; }

    void setIssuerFee(int fee) { issuerFee = fee; }
    int getIssuerFee() const { return issuerFee; }

    // Implementación del método save()
    const std::string save() override
    {
        std::stringstream sql;
        sql << "INSERT INTO CardIssuers (issuer, issuer_fee) VALUES ('"
            << issuer << "', " << issuerFee << ");";
        return sql.str();
    }

    // Implementación del método load()
    std::string load() override
    {
        return selectWithId(IBaseORM::getTableName(), "card_type", cardType);
    }

    static const std::string loadAll()
    {
        return selectAllData("CardIssuers");
    }

    static const int getElements()
    {
        return CARD_ISSUERS_NUMBER_ELEMENTS;
    }
};