#pragma once
#include <vector>
#include <string>
#include "IBaseORM.h"

// Clase abstracta
class IDatabaseFacade
{
public:
    virtual ~IDatabaseFacade() = default;
    virtual bool createTables() = 0;
    virtual bool saveData(IBaseORM &data) = 0;
    virtual bool load(IBaseORM &data) = 0;
    virtual bool load(IBaseORM &data, std::string query) = 0;
    virtual int getLastInsertedId() = 0;
};