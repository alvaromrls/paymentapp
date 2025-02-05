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
        CardORM card;
        card.setCardNumber(cardNumber);
        db->load(card, card.findByCardNumber());

        MerchantORM merchant;
        auto it = std::find_if(merchantMap.begin(), merchantMap.end(),
                               [&](const auto &pair)
                               { return pair.second.getMerchant() == MerchantType; });
        merchant = it->second;

        TransactionORM transaction;
        transaction.setCardId(card);
        transaction.setMerchantId(merchant);
        transaction.setTransactionTime(getCurrentDateTime());
        transaction.setAmount(static_cast<int>(amount * 100));

        db->saveData(transaction);
        transaction.setTransactionId(db->getLastInsertedId());

        AccountORM account;
        account.setId(card.getAccountId());

        HistoryORM history;
        history.setAccountId(account);
        history.setTransactionId(transaction);
        db->saveData(history);
    }
};