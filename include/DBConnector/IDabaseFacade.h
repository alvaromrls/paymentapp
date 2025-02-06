#pragma once
#include <vector>
#include <string>
#include "IBaseORM.h"

// Class Interface for a DB FACADE: It should give no details about the DB Engine to the client.
class IDatabaseFacade
{
public:
    virtual ~IDatabaseFacade() = default;
    virtual bool createTables(std::vector<std::string> tables) = 0; // create tables into DB.
    virtual bool saveData(IBaseORM &data) = 0;                      // stores a data into DB.
    virtual bool load(IBaseORM &data) = 0;                          // gets a data from DB with main ID.
    virtual bool load(IBaseORM &data, std::string query) = 0;       // gets a data from DB with custom query.
    virtual int getLastInsertedId() = 0;                            // gets last inserted ID.
};