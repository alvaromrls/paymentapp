#pragma once

#include <tuple>

#include "SQLiteFacade.h"
#include "AccountORM.h"
#include "CardORM.h"
#include "TransactionORM.h"
#include "HistoryORM.h"

class AddDummyData
{
    bool addDummyAccount()
    {
        bool result = true;
        const int initData[]{910000, 300000, 500000, 10000000, 567540, 12345678};
        for (const auto &data : initData)
        {
            AccountORM account;
            account.setLimit(data);
            result &= database->saveData(account);
        }
        return result;
    }
    bool addDummyCard()
    {
        bool result = true;
        const std::tuple<std::string, int, std::string, std::string, int> initCards[] = {
            {"4532-2830", 1, "John Doe", "2024-02-01", 1},
            {"3056-5954", 1, "John Doe", "2027-09-12", 1},
            {"5425-0990", 1, "John Doe", "2024-08-18", 1},
            {"4916-4231", 2, "Jane Smith", "2023-02-01", 2},
            {"6011-9424", 2, "Jane Smith", "2027-10-08", 2},
            {"4539-2148", 2, "Jane Smith", "2023-09-30", 2},
            {"5254-7899", 3, "Carlos Gomez", "2024-03-15", 3},
            {"5555-4444", 3, "Carlos Gomez", "2024-12-30", 3},
            {"3056-5904", 3, "Carlos Gomez", "2027-07-07", 3},
            {"4539-3646", 4, "Laura Fdz", "2023-05-20", 1},
            {"6011-9590", 4, "Laura Fdz", "2027-03-11", 1},
            {"4111-1111", 4, "Laura Fdz", "2023-11-15", 1},
            {"6011-9596", 5, "Michael Johnson", "2024-06-10", 2},
            {"3742-0126", 5, "Michael Johnson", "2027-04-05", 2},
            {"3793-2306", 5, "Michael Johnson", "2024-05-25", 2},
            {"3793-2316", 6, "Sofia Martinez", "2023-07-25", 3},
            {"4532-8734", 6, "Sofia Martinez", "2027-08-14", 3},
            {"6011-1117", 6, "Sofia Martinez", "2023-06-22", 3},
            {"4916-1123", 1, "Joh Doe", "2027-11-30", 3},
            {"5254-9901", 2, "Jane Smith", "2023-04-02", 2}};

        for (const auto &card : initCards)
        {
            auto [card_number, account_id, name, date, card_issuer] = card;

            CardIssuersORM issuer;
            issuer.setCardType(card_issuer);
            database->loadData(issuer);

            AccountORM account;
            account.setId(account_id);
            database->loadData(account);

            CardORM newCard;
            newCard.setAccountId(account);
            newCard.setCardType(issuer);
            newCard.setExpirationDate(date);
            newCard.setCardholderName(name);
            newCard.setCardNumber(card_number);
            result &= database->saveData(newCard);
        }
        return result;
    }

    bool addDummyTransaction()
    {
        bool result = true;
        const std::tuple<std::string, int, std::string, int> initTransactions[] = {
            {"4532-2830", 1, "2025-01-15 14:23:10", 450},
            {"4916-4231", 2, "2025-01-10 08:45:32", 3200},
            {"5254-7899", 3, "2025-01-05 18:32:21", 700},
            {"4539-3646", 1, "2025-01-20 12:10:10", 2900},
            {"6011-9596", 2, "2025-01-22 16:55:01", 1500},
            {"3793-2306", 3, "2025-01-02 20:12:45", 4800},
            {"3056-5904", 1, "2025-01-08 09:30:59", 900},
            {"6011-9424", 2, "2025-01-12 11:21:05", 1200},
            {"5555-4444", 3, "2025-01-25 17:40:14", 2300},
            {"4111-1111", 1, "2025-01-03 22:10:55", 600},
            {"3742-0126", 2, "2025-01-07 07:25:30", 3500},
            {"6011-1117", 3, "2025-01-18 10:12:50", 2000},
            {"5425-0990", 1, "2025-01-28 15:40:05", 290},
            {"4539-2148", 2, "2025-01-06 13:15:42", 4200},
            {"3056-5904", 3, "2025-01-14 19:08:37", 1800},
            {"6011-9596", 1, "2025-01-21 16:55:20", 950},
            {"3793-2306", 2, "2025-01-09 12:30:48", 5000},
            {"4532-8734", 3, "2025-01-19 08:14:22", 750},
            {"4916-1123", 1, "2025-01-04 14:59:30", 2600},
            {"5254-9901", 2, "2025-01-11 21:05:15", 1300},
            {"4532-2830", 3, "2025-01-26 18:35:12", 3400},
            {"4916-4231", 1, "2025-01-16 09:40:45", 280},
            {"5254-7899", 2, "2025-01-29 07:50:30", 4700},
            {"4539-3646", 3, "2025-01-23 11:45:17", 900},
            {"6011-9596", 1, "2025-01-27 10:15:58", 3100},
            {"3793-2306", 2, "2025-01-30 13:12:22", 2200},
            {"3056-5904", 3, "2025-01-17 06:40:14", 450},
            {"6011-9424", 1, "2025-01-24 15:20:40", 2800},
            {"5555-4444", 2, "2025-01-13 19:59:48", 1900},
            {"4111-1111", 3, "2025-01-01 08:30:05", 320},
            {"3742-0126", 1, "2025-01-31 20:40:12", 5000},
            {"6011-1117", 2, "2025-01-12 14:25:36", 1500},
            {"5425-0990", 3, "2025-01-09 09:05:47", 3800},
            {"4539-2148", 1, "2025-01-20 11:50:30", 600},
            {"3056-5904", 2, "2025-01-05 22:10:20", 2700},
            {"6011-9596", 3, "2025-01-07 17:40:08", 4500},
            {"3793-2306", 1, "2025-01-22 13:55:41", 1100},
            {"4532-8734", 2, "2025-01-14 15:15:50", 3200},
            {"4916-1123", 3, "2025-01-18 07:30:28", 240},
            {"5254-9901", 1, "2025-01-28 10:45:33", 3600},
            {"4532-2830", 2, "2025-01-25 12:20:14", 700},
            {"4916-4231", 3, "2025-01-21 19:55:44", 2100},
            {"5254-7899", 1, "2025-01-02 16:10:05", 480},
            {"4539-3646", 2, "2025-01-06 08:45:17", 4300},
            {"6011-9596", 3, "2025-01-15 14:05:12", 950},
            {"3793-2306", 1, "2025-01-08 20:30:58", 2750},
            {"3056-5904", 2, "2025-01-09 17:20:25", 3900}};
        for (const auto &transaction : initTransactions)
        {
            auto [card_number, merchant_id, datetime, ammount] = transaction;

            CardORM card;
            card.setCardNumber(card_number);
            database->load(card, card.findByCardNumber());

            MerchantORM merchant;
            merchant.setMerchantId(merchant_id);
            database->loadData(merchant);

            TransactionORM newTransaction;
            newTransaction.setAmount(ammount);
            newTransaction.setTransactionTime(datetime);
            newTransaction.setCardId(card);
            newTransaction.setMerchantId(merchant);

            AccountORM main_account;
            main_account.setId(card.getAccountId());
            database->loadData(main_account);

            if (database->saveData(newTransaction))
            {
                newTransaction.setTransactionId(database->getLastInsertedId());
                HistoryORM history;
                history.setAccountId(main_account);
                history.setTransactionId(newTransaction);
                result &= database->saveData(history);
            }
            else
            {
                result = false;
                std::cout << "A transaction for card: " << card_number << " went wrong \n";
                std::cout << "MERCHANT: " << merchant.getMerchantId() << " CARD " << card.getCardId() << std::endl;
            }
        }
        return result;
    }

    SQLiteFacade *database;

public:
    explicit AddDummyData(SQLiteFacade *database) : database(database)
    {
        std::cout << "Writing in DB DUMMY DATA \n";
        bool result = true;
        result &= addDummyAccount();
        result &= addDummyCard();
        result &= addDummyTransaction();
    }
};