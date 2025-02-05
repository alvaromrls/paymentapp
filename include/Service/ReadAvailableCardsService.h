#include <list>
#include <map>

#include "ORMCollector.h"
#include "CardORM.h"
#include "Utils.h"
#include "Commands.h"
#include "IDabaseFacade.h"

class AvailableCardsORM : public ORMCollector<CardORM>
{
public:
    AvailableCardsORM() : ORMCollector<CardORM>(CardORM::getElements()) {};
    std::string virtual load() override
    {
        return "SELECT * FROM " + std::string(CARD_TABLE_NAME) +
               " WHERE expiration_date >= DATE('now');";
    }
};

// Class to implement dependency injection for reading non-expired cards from the database.
class ReadAvailableCardsDB : public ReadAvailableCards
{
    IDatabaseFacade *db;
    std::map<int, CardIssuersORM> issuerMap;

public:
    ReadAvailableCardsDB(IDatabaseFacade *db,
                         std::map<int, CardIssuersORM> issuerMap) : db(db), issuerMap(issuerMap) {};
    std::vector<std::tuple<std::string, std::string, std::string>> read()
    {
        std::vector<std::tuple<std::string, std::string, std::string>> result;
        AvailableCardsORM dispoCards;
        db->load(dispoCards);
        for (const auto &card : dispoCards.getCollection())
        {
            result.push_back(std::tuple<std::string, std::string, std::string>(
                card.getCardNumber(),
                card.getCardholderName(),
                issuerMap[card.getCardType()].getIssuer()));
        }
        return result;
    }
};