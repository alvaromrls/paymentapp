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
    bool loadData(IBaseORM &data) override;

private:
    sqlite3 *db;
    bool executeQuery(const std::string &query);
    bool executeQuery(const std::string &query, IBaseORM *orm);
    static int loadcallback(void *data, int argc, char **argv, char **colNames)
    {
        std::cout << "Reading data.. \n";
        IBaseORM *orm = static_cast<IBaseORM *>(data); // Convertimos el puntero genérico a IBaseORM
        for (int i = 0; i < argc; i++)
        {
            // Supongamos que tenemos una columna llamada 'account_id' en el resultado
            std::string columnValue = argv[i] ? argv[i] : "NULL";
            std::string colName = colNames[i] ? colNames[i] : "NULL";
            orm->fromSQL(colName, columnValue); // Pasamos cada resultado a fromSQL
        }
        return 0;
    }
};