#include <map>
#include <iostream>
#include <thread>
#include <chrono>
#include <sstream>

#include "SQLiteFacade.h"

#include "ORM.h"

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

    // Load card issuers
    ORMCollector<CardIssuersORM> issuers(CardIssuersORM::getElements());
    database.load(issuers, CardIssuersORM::loadAll());

    // Database is empty
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

    // Load Merchants
    ORMCollector<MerchantORM> merchants(MerchantORM::getElements());
    database.load(merchants, MerchantORM::loadAll());

    std::map<int, MerchantORM> merchantMap;
    std::map<std::string, int> merchantTaxMap;
    for (auto const &i : merchants.getCollection())
    {
        merchantMap[i.getMerchantId()] = i;
        merchantTaxMap[i.getMerchant()] = i.getMerchantFee();
    }

    // Create Command Line Parser
    CommandLineParser cli;

    // Add different commands to Command Line parser

    // 1. Pay command
    std::unique_ptr<CalculateTotalFoundsDB> findFounds = std::make_unique<CalculateTotalFoundsDB>(&database);
    std::unique_ptr<FindCardFeeDB> findFee = std::make_unique<FindCardFeeDB>(&database, issuersMap);
    cli.addCommand("PAY", std::make_unique<PayCommand>(merchantTaxMap, std::move(findFounds), std::move(findFee)));

    // 2. History Command
    std::unique_ptr<ReadTransactionHistoryDB> historyService = std::make_unique<ReadTransactionHistoryDB>(&database, merchantMap);
    cli.addCommand("HISTORY", std::make_unique<HistoryCommand>(std::move(historyService)));

    // 3. Show dummy command (for testing)
    std::unique_ptr<ReadAvailableCardsDB> readCardsService = std::make_unique<ReadAvailableCardsDB>(&database, issuersMap);
    cli.addCommand("SHOW DUMMY", std::make_unique<ShowDummyCommand>(std::move(readCardsService)));
    cli.startRunning();

    // App main loop
    while (cli.running())
    {
        cli.processUserInput();
    }

    return 0;
}
