#pragma once
#include <unordered_map>
#include "Utils.h"
#include <string>
#include <functional>

// Clase base para el ORM
class IBaseORM
{
    std::string _tableName;

protected:
    // Mapa de seters
    std::unordered_map<std::string, std::function<void(const std::string &)>> setters;

public:
    IBaseORM(std::string tableName) : _tableName(tableName) {};
    virtual ~IBaseORM() = default;

    // Método para obtener el nombre de la tabla SQL
    std::string getTableName() const { return _tableName; }

    // Método para guardar un objeto en la base de datos (devuelve la consulta SQL)
    virtual const std::string save() = 0;

    // Método para obtener un objeto desde la base de datos por su ID (devuelve la consulta SQL)
    virtual std::string load() = 0;

    // Método para establecer los atributos del objeto a partir de una consulta SQL
    virtual void fromSQL(const std::string &colName, const std::string &colValue)
    {
        if (colValue == "NULL")
        {
            return;
        }
        // Buscar el setter correspondiente y aplicar el valor
        auto it = setters.find(colName);
        if (it != setters.end())
        {
            it->second(colValue); // Llama al setter correspondiente
        }
    }
};
