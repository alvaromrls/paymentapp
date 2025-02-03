extern "C"
{
#include "sqlite3.h"
}

#include "SQLiteFacade.h"
#include <iostream>
#include <sstream>
#include "AccountORM.h"
#include "CardIssuersORM.h"
#include "ORMCollector.h"
#include "MerchantORM.h"
#include "CardORM.h"

int main()
{
    SQLiteFacade database{"payments.db"};
    database.createTables();

    std::cout << "Welcome! Enter commands (PAY <user> <amount> / HISTORY <user> / EXIT):\n";

    std::string command;

    // AccountORM account;
    // account.setLimit(400);

    // database.saveData(account);

    AccountORM read;
    read.setId(2);
    read.setLimit(0);
    database.loadData(read);

    std::cout << "Data is " << read.getLimit() << std::endl;

    ORMCollector<CardIssuersORM> issuers(CardIssuersORM::getElements());
    database.load(issuers, CardIssuersORM::loadAll());

    for (auto const &i : issuers.getCollection())
    {
        std::cout << i.getIssuer() << " : " << i.getIssuerFee() << "%\n";
    }
    // issuer.setCardType(3);

    // database.removeData(issuer);

    // MerchantORM m;
    // m.setMerchant("Supermarket");
    // m.setMerchantFee(1);
    // database.saveData(m);
    // m.setMerchant("Restaurant");
    // m.setMerchantFee(5);
    // database.saveData(m);
    // m.setMerchant("Leisure ");
    // m.setMerchantFee(3);
    // database.saveData(m);

    ORMCollector<MerchantORM> merchants(MerchantORM::getElements());
    database.load(merchants, MerchantORM::loadAll());

    for (auto const &i : merchants.getCollection())
    {
        std::cout << i.getMerchant() << " : " << i.getMerchantFee() << "%\n";
    }

    for (int i = 0; i < 5; i++)
        std::cout << "Generated UUID: " << generateSimpleUUID() << std::endl;

    CardORM my_card;
    my_card.setExpirationDate("2025-02-04");
    my_card.setCardNumber("123-4546");
    my_card.setAccountId(read);
    my_card.setCardholderName("ALVARO M");
    CardIssuersORM alvaro_issuer = issuers.getCollection()[1];
    my_card.setCardType(alvaro_issuer);
    // database.saveData(my_card);

    return 0;
}
