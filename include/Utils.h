#pragma once
#include <sstream>
#include <iomanip>
#include <random>
#include <chrono>

std::string generateSimpleUUID();

std::string selectWithId(const std::string &table, const std::string &colName, const int id);
std::string selectAllData(const std::string &table);

std::string getCurrentDateTime();
