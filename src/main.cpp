extern "C"
{
#include "sqlite3.h"
}

#include "SQLiteFacade.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <sstream>
#include "AccountORM.h"
#include "CardIssuersORM.h"
#include "ORMCollector.h"
#include "MerchantORM.h"
#include "CardORM.h"
#include "TransactionORM.h"
#include "HistoryORM.h"
#include "TotalDepositORM.h"

#include "InitialData.h"
#include "DummyData.h"

int main()
{
    SQLiteFacade database{"payments.db"};
    database.createTables();

    ORMCollector<CardIssuersORM> issuers(CardIssuersORM::getElements());
    database.load(issuers, CardIssuersORM::loadAll());

    if (issuers.getCollection().empty())
    {
        AddInitialData a(&database);
        AddDummyData d(&database);
    }

    for (auto const &i : issuers.getCollection())
    {
        std::cout << i.getIssuer() << " : " << i.getIssuerFee() << "%\n";
    }

    ORMCollector<MerchantORM> merchants(MerchantORM::getElements());
    database.load(merchants, MerchantORM::loadAll());

    for (auto const &i : merchants.getCollection())
    {
        std::cout << i.getMerchant() << " : " << i.getMerchantFee() << "%\n";
    }

    CardORM user;
    user.setCardNumber("4532-2830");
    database.load(user, user.findByCardNumber());

    TotalDepositORM user_deposits(user);
    database.loadData(user_deposits);

    std::cout << "Query: " << user_deposits.load() << "\n";

    std::cout << "User founds: " << user_deposits.getAmount() << "\n";

    return 0;
}
