#include <list>
#include <map>

#include "TransactionORM.h"
#include "ORMCollector.h"
#include "CardORM.h"
#include "Commands.h"
#include "MerchantORM.h"
#include "IDabaseFacade.h"
#include "HistoryORM.h"

// Custom ORM for the complex query
class TransactionHistoryFromCard : public ORMCollector<TransactionORM>
{
    int accountId;
    std::string _start;
    std::string _end;

public:
    TransactionHistoryFromCard(CardORM card, const std::string &start, const std::string &end)
        : accountId(card.getAccountId())
    {
        // Conversion from Date to DateTime
        _start = start + " 00:00:00";
        _end = end + " 23:59:59";
    };
    std::string virtual load() override
    {
        std::stringstream sql;

        sql << "SELECT t.* FROM " << TRANSTACTION_TABLE_NAME << " t"
            << " JOIN " << HISTORY_TABLE_NAME << " h ON t.transaction_id = h.transaction_id"
            << " JOIN " << ACCOUNT_TABLE_NAME << " a  ON h.account_id = a.account_id"
            << " WHERE a.account_id = " << accountId
            << " AND transaction_time BETWEEN '" << _start << "' AND '" << _end << "'"
            << " ORDER BY transaction_time DESC";

        return sql.str();
    }
};

// Implements read transaction history: Reads from DB.
class ReadTransactionHistoryDB : public ReadTransactionHistory
{
    IDatabaseFacade *db;
    std::map<int, MerchantORM> merchantMap;

public:
    ReadTransactionHistoryDB(IDatabaseFacade *db,
                             std::map<int, MerchantORM> merchantMap)
        : db(db), merchantMap(merchantMap) {};
    std::vector<TransactionHistory> read(const std::string &cardNumber,
                                         const std::string &startingDate,
                                         const std::string endingDate) override
    {
        std::vector<TransactionHistory> result;

        // Gets the card
        CardORM card;
        card.setCardNumber(cardNumber);
        db->load(card, card.findByCardNumber());
        if (card.getAccountId() == 0)
        {
            std::cout << "Card not found in DB \n\n";
            return result;
        }

        // Gets the transaction history
        TransactionHistoryFromCard history{card, startingDate, endingDate};
        db->load(history);

        // Format the data
        for (const auto &transaction : history.getCollection())
        {
            result.push_back(
                TransactionHistory{float(transaction.getAmount() / 100.f),
                                   merchantMap[transaction.getMerchantId()].getMerchant(),
                                   transaction.getTransactionTime()});
        }

        return result;
    }
};

// 4916-1123