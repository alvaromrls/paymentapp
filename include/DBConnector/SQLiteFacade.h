#pragma once

extern "C"
{
#include "sqlite3.h"
}
#include <iostream>

#include "IDabaseFacade.h"

// Database implementation for SQLite engine.
class SQLiteFacade : public IDatabaseFacade
{
    sqlite3 *db;                                                                 // SQLite object
    bool executeQuery(const std::string &query);                                 // internal query
    bool executeQuery(const std::string &query, IBaseORM *orm);                  // internal query for callbacks
    static int loadcallback(void *data, int argc, char **argv, char **colNames); // callbacks for reading data

public:
    SQLiteFacade(const std::string &dbName);
    ~SQLiteFacade() override;

    bool createTables(std::vector<std::string> tables) override;
    bool saveData(IBaseORM &data) override;
    bool load(IBaseORM &data) override;
    bool load(IBaseORM &data, std::string query) override;
    int getLastInsertedId() override;
};