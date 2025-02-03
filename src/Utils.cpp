#include "Utils.h"
#include <ctime>

std::string generateSimpleUUID()
{
    // Use the current time (in milliseconds) as a seed
    auto now = std::chrono::steady_clock::now();
    auto time_since_epoch = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();

    // Generate a random part (16 bytes)
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, 255);

    std::stringstream uuid;

    // Format like a UUID: 8-4-4-4-12
    uuid << std::hex << std::setw(8) << std::setfill('0') << time_since_epoch;
    uuid << "-";
    for (int i = 0; i < 3; ++i)
    {
        uuid << std::setw(4) << std::setfill('0') << dist(gen);
    }
    uuid << "-";
    for (int i = 0; i < 3; ++i)
    {
        uuid << std::setw(4) << std::setfill('0') << dist(gen);
    }
    uuid << "-";
    for (int i = 0; i < 3; ++i)
    {
        uuid << std::setw(4) << std::setfill('0') << dist(gen);
    }
    uuid << "-";
    for (int i = 0; i < 12; ++i)
    {
        uuid << std::setw(2) << std::setfill('0') << dist(gen);
    }

    return uuid.str();
}

std::string selectWithId(const std::string &table, const std::string &colName, const int id)
{
    return selectWithId(table, colName, std::to_string(id));
}

std::string selectWithId(const std::string &table, const std::string &colName, const std::string &id)
{
    std::stringstream sql;
    sql << "SELECT * FROM " << table << " WHERE " << colName << " = " << id << ";";
    return sql.str();
}

std::string selectAllData(const std::string &table)
{
    std::stringstream sql;
    sql << "SELECT * FROM " << table << ";";
    return sql.str();
}

std::string getCurrentDateTime()
{
    std::time_t now = std::time(nullptr);
    std::tm tmStruct{};
    localtime_s(&tmStruct, &now);
    std::ostringstream oss;
    oss << std::put_time(&tmStruct, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}
