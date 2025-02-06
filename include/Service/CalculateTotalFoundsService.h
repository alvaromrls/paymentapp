#include <map>

#include "IDabaseFacade.h"

#include "ExpendedMoneyORM.h"
#include "CardORM.h"

#include "Commands.h"

// Implements Calculate Total Founds: Gets it from DB.
class CalculateTotalFoundsDB : public CalculateTotalFounds
{
    IDatabaseFacade *db;

public:
    CalculateTotalFoundsDB(IDatabaseFacade *db) : db(db) {};

    // Gets founds from credit card number: -1 expired card
    float getFounds(const std::string &cardNumber) override
    {
        // Read card
        CardORM card;
        card.setCardNumber(cardNumber);
        db->load(card, card.findByCardNumber());

        // Checks card is not expired
        if (card.getExpirationDate() < getCurrentDate())
        {
            return -1.f;
        }

        // Calculate expended money
        ExpendedMoneyORM deposit{card};
        db->load(deposit);

        // Reads limit from database
        AccountORM account;
        account.setId(card.getAccountId());
        db->load(account);

        // Calculate founds and convert to Euros.
        return float((account.getLimit() - deposit.getAmount()) / 100.f);
    }
};