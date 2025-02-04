#pragma once
#include <sstream>
#include <iomanip>
#include <random>
#include <chrono>

/**
 * @brief Generates a random UUID
 *
 * @return std::string
 */
std::string generateSimpleUUID();

/**
 * @brief Prepares a SQL query for selecting a specific Element with Int ID
 *
 * @param table to seach in
 * @param colName the name of the column
 * @param id the param to search for
 * @return std::string
 */
std::string selectWithId(const std::string &table, const std::string &colName, const int id);

std::string selectWithId(const std::string &table, const std::string &colName, const std::string &id);

/**
 * @brief Prepares a SQL query for selecting all elements from a table.
 *
 * @param table
 * @return std::string
 */
std::string selectAllData(const std::string &table);

/**
 * @brief Get the Current Time in SQL format
 *
 * @return std::string
 */
std::string getCurrentDateTime();
