#include <vector>
#include <iostream>
#include <string>
#include "IBaseORM.h"

// Clase collector para manejar la colección de objetos ORM derivados de IBaseORM
template <typename T>
class ORMCollector : public IBaseORM
{
private:
    std::vector<T> vector; // Vector para almacenar objetos del tipo T
    T data;
    int count;     // Contador para las iteraciones
    int pushEvery; // Número de iteraciones antes de hacer push

public:
    ORMCollector(int pushEvery) : count(0), pushEvery(pushEvery) {}

    // Método para agregar un objeto a la colección
    void add(T orm)
    {
        vector.push_back(orm); // Almacena el objeto en el vector
    }

    // Implementación del método fromSQL de IBaseORM
    void fromSQL(const std::string &colName, const std::string &colValue) override
    {
        data.fromSQL(colName, colValue);
        count++;
        // Solo hacer push cada 'pushEvery' iteraciones
        if (count >= pushEvery)
        {
            add(data);
            count = 0; // Reinicia el contador
        }
    }

    // Obtener el tamaño de la colección
    size_t size() const
    {
        return vector.size();
    }

    // Método para acceder a la colección
    const std::vector<T> getCollection() const
    {
        return vector;
    }

    std::string const save() override { return {}; }
    std::string update() override { return {}; }
    std::string remove() override { return {}; }
    std::string load() override { return {}; }
};
