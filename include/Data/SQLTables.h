#pragma once

#include "ORM.h"
#include <string>
#include <vector>

// DB Table Definitions
namespace SQLTables
{
    const std::string AccountTable =
        "CREATE TABLE IF NOT EXISTS " + std::string(ACCOUNT_TABLE_NAME) +
        "(account_id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "account_limit INTEGER NOT NULL);";

    const std::string HistoryTable =
        "CREATE TABLE IF NOT EXISTS " + std::string(HISTORY_TABLE_NAME) +
        "(account_id INTEGER NOT NULL, "
        "transaction_id INTEGER NOT NULL, "
        "FOREIGN KEY (account_id) REFERENCES " +
        std::string(ACCOUNT_TABLE_NAME) +
        " (account_id) ON DELETE CASCADE, "
        "FOREIGN KEY (transaction_id) REFERENCES " +
        std::string(TRANSACTION_TABLE_NAME) +
        " (transaction_id) ON DELETE CASCADE);";

    const std::string CardTable =
        "CREATE TABLE IF NOT EXISTS " + std::string(CARD_TABLE_NAME) +
        "("
        "card_id TEXT PRIMARY KEY, "
        "card_number TEXT NOT NULL UNIQUE, "
        "account_id INTEGER, "
        "cardholder_name TEXT NOT NULL, "
        "expiration_date DATE NOT NULL, "
        "card_type INTEGER NOT NULL,"
        "FOREIGN KEY (card_type) REFERENCES " +
        std::string(CARD_ISSUERS_TABLE_NAME) +
        " (card_type) ON DELETE CASCADE, "
        "FOREIGN KEY (account_id) REFERENCES " +
        std::string(ACCOUNT_TABLE_NAME) +
        " (account_id) ON DELETE CASCADE);";

    const std::string CardIssuersTable =
        "CREATE TABLE IF NOT EXISTS " + std::string(CARD_ISSUERS_TABLE_NAME) +
        "("
        "card_type INTEGER PRIMARY KEY AUTOINCREMENT,"
        "issuer TEXT NOT NULL, "
        "issuer_fee INTEGER NOT NULL);";

    const std::string TransactionTable =
        "CREATE TABLE IF NOT EXISTS " + std::string(TRANSACTION_TABLE_NAME) +
        "("
        "transaction_id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "card_id TEXT, "
        "merchant_id INTEGER, "
        "transaction_time DATETIME NOT NULL, "
        "amount INTEGER NOT NULL,"
        "FOREIGN KEY (card_id) REFERENCES " +
        std::string(CARD_TABLE_NAME) +
        " (card_id) ON DELETE CASCADE, "
        "FOREIGN KEY (merchant_id) REFERENCES " +
        std::string(MERCHANTS_TABLE_NAME) +
        " (merchant_id) ON DELETE CASCADE);";

    const std::string MerchantTable =
        "CREATE TABLE IF NOT EXISTS " + std::string(MERCHANTS_TABLE_NAME) +
        "("
        "merchant_id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "merchant TEXT NOT NULL,"
        "merchant_fee INTEGER NOT NULL);";

};