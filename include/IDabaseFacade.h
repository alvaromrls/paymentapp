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
    virtual bool loadData(IBaseORM &data) = 0;
    virtual bool load(IBaseORM &data, std::string query) = 0;
    virtual bool removeData(IBaseORM &data) = 0;
};