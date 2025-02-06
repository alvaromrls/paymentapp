#pragma once // Ensures the file is included only once during compilation.

#include <sstream>
#include <iostream>
#include "IBaseORM.h"

// Number of elements in the Merchant table.
constexpr int MERCHANTS_NUMBER_ELEMENTS = 3;

// Name of the SQL table associated with this ORM.
constexpr const std::string_view MERCHANTS_TABLE_NAME = "Merchant";

// ORM class representing the "Merchant" entity in the database.
class MerchantORM : public IBaseORM
{
    int merchantId;       // Unique identifier for the merchant.
    std::string merchant; // Name of the merchant.
    int merchantFee;      // Transaction fee associated with the merchant.

public:
    // Constructor initializes attributes and sets up dynamic setters for database interaction.
    MerchantORM() : merchantId{0}, merchant{}, merchantFee{0}, IBaseORM(std::string(MERCHANTS_TABLE_NAME))
    {
        // Initialize the map of setters for database column mapping.
        setters["merchant_id"] = [this](const std::string &value)
        { this->setMerchantId(std::stoi(value)); };

        setters["merchant"] = [this](const std::string &value)
        { this->setMerchant(value); };

        setters["merchant_fee"] = [this](const std::string &value)
        { this->setMerchantFee(std::stoi(value)); };
    }

    // Getter and setter methods for merchantId.
    void setMerchantId(int id) { merchantId = id; }
    int getMerchantId() const { return merchantId; }

    // Getter and setter methods for merchant name.
    void setMerchant(const std::string &merchantValue) { merchant = merchantValue; }
    std::string getMerchant() const { return merchant; }

    // Getter and setter methods for merchant fee.
    void setMerchantFee(int fee) { merchantFee = fee; }
    int getMerchantFee() const { return merchantFee; }

    // Generates an SQL statement to insert the merchant into the database.
    const std::string save() override
    {
        std::stringstream sql;
        sql << "INSERT INTO Merchant (merchant, merchant_fee) VALUES ('"
            << merchant << "', " << merchantFee << ");";
        return sql.str();
    }

    // Generates an SQL statement to load a merchant record by its ID.
    std::string load() override
    {
        std::stringstream sql;
        sql << "SELECT * FROM Merchant WHERE merchant_id = " << merchantId << ";";
        return sql.str();
    }

    // Generates an SQL query to retrieve all merchants from the database.
    static const std::string loadAll()
    {
        return selectAllData(std::string(MERCHANTS_TABLE_NAME));
    }

    // Returns the number of elements associated with this ORM.
    static const int getElements()
    {
        return MERCHANTS_NUMBER_ELEMENTS;
    }
};
