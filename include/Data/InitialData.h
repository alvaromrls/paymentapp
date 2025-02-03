#pragma once

#include "SQLiteFacade.h"
#include "MerchantORM.h"
#include "CardIssuersORM.h"
#include <tuple>

class AddInitialData
{
    bool addDefaultMerchants()
    {
        bool result = true;
        const std::tuple<std::string, int> initData[]{
            {"SuperMarket", 1},
            {"Restaurant", 5},
            {"Leisure", 3}};
        for (const auto &data : initData)
        {
            auto [name, fee] = data;
            MerchantORM merchant;
            merchant.setMerchantFee(fee);
            merchant.setMerchant(name);
            result &= database->saveData(merchant);
        }
        return result;
    }
    bool addDefaultCardIssuers()
    {
        bool result = true;
        const std::tuple<std::string, int> initData[]{
            {"Visa", 2},
            {"MC", 2},
            {"AMEX", 5}};
        for (const auto &data : initData)
        {
            auto [name, fee] = data;
            CardIssuersORM issuer;
            issuer.setIssuerFee(fee);
            issuer.setIssuer(name);
            result &= database->saveData(issuer);
        }
        return result;
    }
    SQLiteFacade *database;

public:
    explicit AddInitialData(SQLiteFacade *database) : database(database)
    {
        std::cout << "Writing in DB INITIAL DATA \n";
        bool result = addDefaultMerchants() && addDefaultCardIssuers();
        if (!result)
        {
            std::cerr << "Error writing initial data in DB " << std::endl;
        }
    }
};