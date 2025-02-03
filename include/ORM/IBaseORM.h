#pragma once
#include <unordered_map>
#include <string>
#include <functional>

// Clase base para el ORM
class IBaseORM
{
protected:
    // Mapa de seters
    std::unordered_map<std::string, std::function<void(const std::string &)>> setters;

public:
    virtual ~IBaseORM() = default;

    // Método para guardar un objeto en la base de datos (devuelve la consulta SQL)
    virtual const std::string save() = 0;

    // Método para actualizar un objeto en la base de datos (devuelve la consulta SQL)
    virtual std::string update() = 0;

    // Método para eliminar un objeto en la base de datos (devuelve la consulta SQL)
    virtual std::string remove() = 0;

    // Método para obtener un objeto desde la base de datos por su ID (devuelve la consulta SQL)
    virtual std::string load() = 0;

    // Método para establecer los atributos del objeto a partir de una consulta SQL
    virtual void fromSQL(const std::string &colName, const std::string &colValue) = 0;
};
