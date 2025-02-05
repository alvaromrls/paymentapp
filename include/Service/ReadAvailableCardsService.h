#include <list>
#include <map>

#include "ORMCollector.h"
#include "CardORM.h"
#include "Utils.h"
#include "Commands.h"
#include "IDabaseFacade.h"

// Class to implement dependency injection for reading non-expired cards from the database.
class ReadAvailableCardsDB : public ReadAvailableCards
{
    IDatabaseFacade *db;
    std::map<int, CardIssuersORM> issuerMap;

public:
    ReadAvailableCardsDB(IDatabaseFacade *db,
                         std::map<int, CardIssuersORM> issuerMap)
        : db(db), issuerMap(issuerMap) {};
    std::vector<ReadCards> read()
    {
        std::vector<ReadCards> result;
        ORMCollector<CardORM> dispoCards;
        db->load(dispoCards);
        for (const auto &card : dispoCards.getCollection())
        {
            result.push_back(
                ReadCards{
                    card.getCardNumber(),
                    card.getCardholderName(),
                    issuerMap[card.getCardType()].getIssuer(),
                    card.getExpirationDate() > getCurrentDate() ? " valid " : "expired"});
        }
        return result;
    }
};