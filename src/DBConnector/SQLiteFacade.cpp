#include "SQLiteFacade.h"
#include "SQLTables.h"
#include <iostream>

SQLiteFacade::SQLiteFacade(const std::string &dbName) : db(nullptr)
{
    int rc = sqlite3_open(dbName.c_str(), &db);
    if (rc)
    {
        std::cerr << "Error opening DB: " << sqlite3_errmsg(db) << std::endl;
    }
}

SQLiteFacade::~SQLiteFacade()
{
    if (db)
    {
        sqlite3_close(db);
    }
}

bool SQLiteFacade::createTables()
{
    // Habilitar claves foráneas
    std::string enableForeignKeysQuery = "PRAGMA foreign_keys = ON;";
    if (!executeQuery(enableForeignKeysQuery))
    {
        return false;
    }
    // Crear las tablas (si no existen)
    for (auto const &table : SQLTables::all)
    {
        if (!executeQuery(table))
        {
            return false;
        }
    }
    return true;
}

bool SQLiteFacade::saveData(IBaseORM &data)
{
    return executeQuery(data.save());
}

bool SQLiteFacade::load(IBaseORM &data)
{
    return executeQuery(data.load(), &data);
}

bool SQLiteFacade::load(IBaseORM &data, std::string query)
{
    return executeQuery(query, &data);
}

bool SQLiteFacade::executeQuery(const std::string &query)
{
    char *errMsg = nullptr;
    if (sqlite3_exec(db, query.c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK)
    {
        std::cerr << "SQLite error: " << errMsg << "\n";
        sqlite3_free(errMsg);
        return false;
    }
    return true;
}

bool SQLiteFacade::executeQuery(const std::string &query, IBaseORM *orm)
{
    char *errMsg = nullptr;
    // Llamamos al callback pasándole el objeto ORM
    if (sqlite3_exec(db, query.c_str(), loadcallback, orm, &errMsg) != SQLITE_OK)
    {
        std::cerr << "SQLite error: " << errMsg << "\n";
        sqlite3_free(errMsg);
        return false;
    }
    return true;
}

int SQLiteFacade::loadcallback(void *data, int argc, char **argv, char **colNames)
{
    IBaseORM *orm = static_cast<IBaseORM *>(data); // Convertimos el puntero genérico a IBaseORM
    for (int i = 0; i < argc; i++)
    {
        std::string columnValue = argv[i] ? argv[i] : "NULL";
        std::string colName = colNames[i] ? colNames[i] : "NULL";
        orm->fromSQL(colName, columnValue); // Pasamos cada resultado a fromSQL
    }
    return 0;
}

int SQLiteFacade::getLastInsertedId()
{
    return static_cast<int>(sqlite3_last_insert_rowid(db));
}