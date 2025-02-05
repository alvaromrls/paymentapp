#include <map>

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

#include "CommandLineParser.h"

#include "ReadAvailableCardsService.h"

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
        database.load(issuers, CardIssuersORM::loadAll());
    }

    std::map<int, CardIssuersORM> issuersMap;
    for (const auto &issuer : issuers.getCollection())
    {
        issuersMap[issuer.getCardType()] = issuer;
        std::cout << issuer.getIssuer() << " : " << issuer.getIssuerFee() << "%\n";
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

    std::cout << "User founds: " << user_deposits.getAmount() << "\n";

    CommandLineParser cli;
    cli.addCommand("PAY", std::make_unique<PayCommand>());
    cli.addCommand("HISTORY", std::make_unique<HistoryCommand>());

    std::unique_ptr<ReadAvailableCardsDB> f = std::make_unique<ReadAvailableCardsDB>(&database, issuersMap);
    cli.addCommand("SHOW DUMMY", std::make_unique<ShowDummyCommand>(std::move(f)));
    cli.startRunning();

    while (cli.running())
    {
        cli.processUserInput();
    }

    return 0;
}
