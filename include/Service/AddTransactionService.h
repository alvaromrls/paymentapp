#include <map>
#include "Commands.h"
#include "ORM.h"
#include "IDabaseFacade.h"

// Implements Add New Transaction: Stores it in DB.
class AddNewTransactionDB : public AddNewTransaction
{
    IDatabaseFacade *db;
    const std::map<int, MerchantORM> &merchantMap;

public:
    AddNewTransactionDB(IDatabaseFacade *db,
                        const std::map<int, MerchantORM> &merchantORM)
        : db(db), merchantMap(merchantORM) {};

    // We asume at this point Card Number and Merchant exist.
    void addTransaction(const std::string &cardNumber,
                        const std::string MerchantType,
                        float amount) override
    {
        // 1. Read card from DB
        CardORM card;
        card.setCardNumber(cardNumber);
        db->load(card, card.findByCardNumber());

        // 2. Finds correct Merchant object
        MerchantORM merchant;
        auto it = std::find_if(merchantMap.begin(), merchantMap.end(),
                               [&](const auto &pair)
                               { return pair.second.getMerchant() == MerchantType; });
        merchant = it->second;

        // 3. Creates the transaction object
        TransactionORM transaction;
        transaction.setCardId(card);
        transaction.setMerchantId(merchant);
        transaction.setTransactionTime(getCurrentDateTime());
        transaction.setAmount(static_cast<int>(amount * 100)); // convert to cents (int)

        // 3.1 Stores the transaction
        db->saveData(transaction);
        transaction.setTransactionId(db->getLastInsertedId()); // get inserted id

        // 4. Get the account
        AccountORM account;
        account.setId(card.getAccountId());

        // 5. Creates and stores the history record
        HistoryORM history;
        history.setAccountId(account);
        history.setTransactionId(transaction);
        db->saveData(history);
    }
};