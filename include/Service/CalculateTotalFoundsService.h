#include <map>

#include "IDabaseFacade.h"

#include "ExpendedMoneyORM.h"
#include "CardORM.h"

#include "Commands.h"

class CalculateTotalFoundsDB : public CalculateTotalFounds
{
    IDatabaseFacade *db;

public:
    CalculateTotalFoundsDB(IDatabaseFacade *db) : db(db) {};
    float getFounds(const std::string &cardNumber) override
    {
        CardORM card;
        card.setCardNumber(cardNumber);
        db->load(card, card.findByCardNumber());

        if (card.getExpirationDate() < getCurrentDate())
        {
            return -1.f;
        }

        ExpendedMoneyORM deposit{card};
        db->load(deposit);

        AccountORM account;
        account.setId(card.getAccountId());
        db->load(account);

        std::cout << "Expended " << deposit.getAmount() << " from "
                  << account.getLimit() << std::endl;

        return float((account.getLimit() - deposit.getAmount()) / 100.f);
    }
};