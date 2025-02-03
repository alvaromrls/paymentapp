#pragma once

#include <sstream>
#include <iostream>
#include "IBaseORM.h"

constexpr int MERCHANTS_NUMBER_ELEMENTS = 3;
constexpr const std::string_view MERCHANTS_TABLE_NAME = "Merchant";

// Implementación de la clase MerchantORM
class MerchantORM : public IBaseORM
{
    int merchantId;
    std::string merchant;
    int merchantFee;

public:
    MerchantORM() : merchantId{0}, merchant{}, merchantFee{0}, IBaseORM(std::string(MERCHANTS_TABLE_NAME))
    {
        // Inicializamos los setters
        setters["merchant_id"] = [this](const std::string &value)
        { this->setMerchantId(std::stoi(value)); };
        setters["merchant"] = [this](const std::string &value)
        { this->setMerchant(value); };
        setters["merchant_fee"] = [this](const std::string &value)
        { this->setMerchantFee(std::stoi(value)); };
    }

    // Métodos getter y setter
    void setMerchantId(int id) { merchantId = id; }
    int getMerchantId() const { return merchantId; }

    void setMerchant(const std::string &merchantValue) { merchant = merchantValue; }
    std::string getMerchant() const { return merchant; }

    void setMerchantFee(int fee) { merchantFee = fee; }
    int getMerchantFee() const { return merchantFee; }

    // Implementación del método save()
    const std::string save() override
    {
        std::stringstream sql;
        sql << "INSERT INTO Merchant (merchant, merchant_fee) VALUES ('"
            << merchant << "', " << merchantFee << ");";
        return sql.str();
    }

    // Implementación del método update()
    std::string update() override
    {
        std::stringstream sql;
        sql << "UPDATE Merchant SET merchant = '" << merchant
            << "', merchant_fee = " << merchantFee
            << " WHERE merchant_id = " << merchantId << ";";
        return sql.str();
    }

    // Implementación del método remove()
    std::string remove() override
    {
        std::stringstream sql;
        sql << "DELETE FROM Merchant WHERE merchant_id = " << merchantId << ";";
        return sql.str();
    }

    // Implementación del método load()
    std::string load() override
    {
        std::stringstream sql;
        sql << "SELECT * FROM Merchant WHERE merchant_id = " << merchantId << ";";
        return sql.str();
    }

    static const std::string loadAll()
    {
        return selectAllData(std::string(MERCHANTS_TABLE_NAME));
    }

    static const int getElements()
    {
        return MERCHANTS_NUMBER_ELEMENTS;
    }
};
