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
#include "ExpendedMoneyORM.h"

#include "InitialData.h"
#include "DummyData.h"

#include "CommandLineParser.h"

#include "ReadAvailableCardsService.h"
#include "ReadTransactionHistoryService.h"
#include "CalculateTotalFoundsService.h"
#include "FindCardFeeService.h"

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
    }

    ORMCollector<MerchantORM> merchants(MerchantORM::getElements());
    database.load(merchants, MerchantORM::loadAll());

    std::map<int, MerchantORM> merchantMap;
    std::map<std::string, int> merchantTaxMap;
    for (auto const &i : merchants.getCollection())
    {
        merchantMap[i.getMerchantId()] = i;
        merchantTaxMap[i.getMerchant()] = i.getMerchantFee();
    }

    CommandLineParser cli;

    std::unique_ptr<CalculateTotalFoundsDB> findFounds = std::make_unique<CalculateTotalFoundsDB>(&database);
    std::unique_ptr<FindCardFeeDB> findFee = std::make_unique<FindCardFeeDB>(&database, issuersMap);
    cli.addCommand("PAY", std::make_unique<PayCommand>(merchantTaxMap, std::move(findFounds), std::move(findFee)));

    std::unique_ptr<ReadTransactionHistoryDB> historyService = std::make_unique<ReadTransactionHistoryDB>(&database, merchantMap);
    cli.addCommand("HISTORY", std::make_unique<HistoryCommand>(std::move(historyService)));

    std::unique_ptr<ReadAvailableCardsDB> readCardsService = std::make_unique<ReadAvailableCardsDB>(&database, issuersMap);
    cli.addCommand("SHOW DUMMY", std::make_unique<ShowDummyCommand>(std::move(readCardsService)));
    cli.startRunning();

    while (cli.running())
    {
        cli.processUserInput();
    }

    return 0;
}
