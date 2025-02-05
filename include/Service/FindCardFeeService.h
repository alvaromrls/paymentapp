#include <map>

#include "IDabaseFacade.h"

#include "CardIssuersORM.h"
#include "CardORM.h"

#include "Commands.h"

class FindCardFeeDB : public FindCardFee
{
    IDatabaseFacade *db;
    std::map<int, CardIssuersORM> issuersMap;

public:
    FindCardFeeDB(IDatabaseFacade *db,
                  std::map<int, CardIssuersORM> issuersMap)
        : db(db), issuersMap(issuersMap) {};

    int getFee(const std::string &cardNumber) override
    {
        CardORM card;
        card.setCardNumber(cardNumber);
        db->load(card, card.findByCardNumber());
        int feeId = card.getCardType();

        if (feeId == 0)
        {
            return 0;
        }
        else
        {
            return issuersMap.at(feeId).getIssuerFee();
        }
    }
};