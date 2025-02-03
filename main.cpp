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

#include "InitialData.h"

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

    if (issuers.getCollection().empty())
    {
        AddInitialData a(&database);
    }

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

    CardORM my_card;
    my_card.setCardId("02f11529-00db005f0049-0024006d00be-0028003700f3-cc945ebced1a0749abca9eef");
    database.loadData(my_card);
    MerchantORM merchant = merchants.getCollection()[0];

    TransactionORM transaction;
    transaction.setCardId(my_card);
    transaction.setMerchantId(merchant);

    // for (int i = 0; i < 6; i++)
    // {
    //     transaction.setTransactionTime(getCurrentDateTime());
    //     transaction.setAmount(i * 100);
    //     if (database.saveData(transaction))
    //     {
    //         std::cout << database.getLastInsertedId() << std::endl;
    //         transaction.setTransactionId(database.getLastInsertedId());
    //         HistoryORM h;
    //         h.setTransactionId(transaction);
    //         h.setAccountId(read);
    //         database.saveData(h);
    //     }
    //     std::this_thread::sleep_for(std::chrono::seconds(1));
    // }

    // ORMCollector<HistoryORM> ts(HistoryORM::getElements());
    // database.load(ts, HistoryORM::loadAll());

    // for (auto &t : ts.getCollection())
    // {
    //     std::cout << t.getTransactionId() << "\n";
    // }

    // my_card.setExpirationDate("2025-02-04");
    // my_card.setCardNumber("123-4546");
    // my_card.setAccountId(read);
    // my_card.setCardholderName("ALVARO M");
    // CardIssuersORM alvaro_issuer = issuers.getCollection()[1];
    // my_card.setCardType(alvaro_issuer);
    // database.saveData(my_card);

    CardORM checkCard;
    checkCard.setCardNumber("123-4546");
    database.load(checkCard, checkCard.findByCardNumber());

    std::cout << checkCard.getCardholderName() << "\n";

    return 0;
}
