#pragma once

extern "C"
{
#include "sqlite3.h"
}
#include <iostream>

#include "IDabaseFacade.h"

class SQLiteFacade : public IDatabaseFacade
{
public:
    SQLiteFacade(const std::string &dbName);
    ~SQLiteFacade() override;

    bool createTables() override;
    bool saveData(IBaseORM &data) override;
    bool load(IBaseORM &data) override;
    bool load(IBaseORM &data, std::string query) override;
    int getLastInsertedId() override;

private:
    sqlite3 *db;
    bool executeQuery(const std::string &query);
    bool executeQuery(const std::string &query, IBaseORM *orm);
    static int loadcallback(void *data, int argc, char **argv, char **colNames);
};